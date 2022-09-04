#include <cmath>
#include <chrono>
#include <random>
#include <thread>
#include <iostream>
#include <functional>
#include <boost/program_options.hpp>
#include "ui/feedback.hpp"                                      // cirno::error()/warn()
#include "properties.hpp"                                       // Default values; Build properties
#include "platform/platform.hpp"                                // cirno::init() returns platform-non-specifically abstraction
#include "config/config.hpp"                                    // Config <-> FastBleed layer

/*************[Default values]*************/
static float cps                        = c_cps;                // Click Per Second
static float relation                   = c_relation;           // Hold time / Release time
static unsigned int entropy_variation   = c_entropy_variation;  // Introduces randomness in the timings of pressing
static unsigned int actions_cooldown    = c_actions_cooldown;
static bool be_verbose                  = false;
/*/*/

bool override_wayland, override_xorg;
namespace po = boost::program_options;
int parse_args(int argc, char* argv[]);

typedef struct s_timings {
    unsigned int hold_time;
    unsigned int release_time;
    unsigned int entropy_variation;
} t_timings;
t_timings calculate_timings(float cps, float relation, unsigned int entropy_variation);

void handle_actions(std::shared_ptr<cirno::control_impl> control, t_timings timings, struct s_event_decl actions);

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    cirno::c_config config {"./config.json"};

    std::shared_ptr<cirno::control_impl> control = cirno::get_platform();           // Pick platform-non-specifically abstraction
    int status = control->init();                                                   // Initialize implementation
    switch (status) {
        // Generic:
        case -100:
            std::cerr << "No displays found.\n";
            break;

        // Unix:
        case -202:
            std::cerr << "This build completed without X11 support.\n";
            break;
        case -201:
            std::cerr << "This build completed without Wayland support.\n";
            break;
    }; if (status < 0) {
        cirno::error("Failed to initialize implementation");
        exit(1);
    }

    t_timings timings = calculate_timings(cps, relation, entropy_variation);        // Calculate delays around CPS/Relation value
    
    // Read config -> declare events
    s_event_decl events_decl = config.parse();
    std::vector<e_actions> vec_action_script;
    std::vector<unsigned int> vec_action_params;

    // Start actions handler thread
    std::thread actions_thread(handle_actions, control, timings, events_decl);
    actions_thread.detach();

    // Call events handler
    cirno::general_state(true);
    if (control->handle_events(&events_decl) != 0) {
        cirno::error("Unable to handling events!");
        exit(1);
    }
    
    return 0;
}

int parse_args(int argc, char* argv[]) {
    po::options_description desc("Usage: gofra [ options ... ]\n\tWhere options");
    desc.add_options()
        ("help,h", "Help page")
        ("verbose,v", "Be verbose")
        ("cps,c", po::value<float>(&cps), "Clicks Per Second: float (0.0:500.0)")
        ("relation,r", po::value<float>(&relation), "'Hold time'/'Release time' relation: float (0.0:500/cps)")
        //("entropy,e", po::value<unsigned int>(&entropy_variation), "Entropy range (value+-delays): uint [0:?)")
        #if defined __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
            ("xorg,x", "Override Xorg")
            ("wayland,w", "Override Wayland")
        #endif
    ;
    po::variables_map args;
    po::store(po::parse_command_line(argc, argv, desc), args);
    po::notify(args);

    #if defined __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
        if (args.count("xorg")) {
            override_xorg = true;
        }
        if (args.count("wayland")) {
            override_wayland = true;
        }
    #endif

    if (args.count("verbose")) {
        be_verbose = true;
    }
    if (args.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }

    return 0;
}

t_timings calculate_timings(float cps, float relation, unsigned int entropy_variation) {
    t_timings ret;
    ret.entropy_variation = entropy_variation;

    if (relation<=0) {
        relation = c_relation;
        cirno::warn("CPS<=0! Built-in cps("+std::to_string(cps)+") value are used.");
    }

    if (cps<=0) {
        cps = c_cps;
        cirno::warn("CPS<=0! Built-in cps("+std::to_string(cps)+") value are used.");
    }
    float total_click_time = 1000.0f / cps;

    // Fallback values:
    ret.hold_time    = static_cast<unsigned int>(total_click_time) / 2;    
    ret.release_time = static_cast<unsigned int>(total_click_time) / 2;

    // An auxiliary number that helps determine the most approximate value; = biggest possible value :
    float best       = total_click_time;

    float x, y; // System of equations:
    // /x + t = total_click_time
    // \x / y = relation

    for (x = 1; x < total_click_time - 1; x++) {
        y = ceil(total_click_time - x);
        if (fabs((y+x) - total_click_time) < 1) {
            if (fabs((x/y) - relation) < fabs(best - relation)) {
                best = x/y;
                ret.hold_time    = static_cast<unsigned int>(x);
                ret.release_time = static_cast<unsigned int>(y);
            }
        }
    }

    if (best == total_click_time) {
        cirno::warn("Failed to calculate timings! Fallback values are used.");
        return calculate_timings(c_cps, c_relation, c_entropy_variation);    // Incorrect build-in values can lead to looping recursion
    }

    if (be_verbose)
        std::cerr << "[INFO ] Calculated timings => "
            << ret.hold_time << "ms /" << ret.release_time << "ms" << std::endl;

    return ret;
}

void handle_actions(std::shared_ptr<cirno::control_impl> control, t_timings timings, struct s_event_decl actions) {
    bool cooldown = false; // If nothing needs to be done

    std::random_device rd; std::mt19937 gen_seed(rd());
    std::uniform_int_distribution<> entropy(-timings.entropy_variation, timings.entropy_variation);
    // ^ Introduces randomness in the timings of pressing. Makes clicks more natural. ^

    while (true) {
        for (unsigned int i=0; i<actions.count; i++) {

            if (actions.flag[i]) {
                cooldown = true;
                for (int a = 0; a < static_cast<int>(actions.action->size()); a++) {

                    switch (actions.action->at(a)) {
                        case ACT_CLICK:
                            control->action_button( actions.action_param[i].at(a) , true);
                            break;
                        
                        case ACT_RELEASE:
                            control->action_button(actions.action_param[i].at(a) , false);
                            break;

                        case ACT_CLICKER:
                            control->action_button(actions.action_param[i].at(a) , true);
                            std::this_thread::sleep_for(std::chrono::milliseconds(timings.hold_time + entropy(gen_seed)));
                            control->action_button(actions.action_param[i].at(a) , false);
                            std::this_thread::sleep_for(std::chrono::milliseconds(timings.release_time + entropy(gen_seed)));
                            break;
                        
                        case ACT_DELAY:
                            std::this_thread::sleep_for(std::chrono::milliseconds(actions.action_param[i].at(a)));
                            cooldown = false;
                            break;
                        
                        case ACT_MOVE:
                            break;
                        
                        case ACT_SYS_EXEC:
                            break;
                        
                        case ACT_TEXT_TYPE:
                            break;
                        
                        default:
                            break;
                    };
                }
            }

        }
        if (!cooldown) {
            std::this_thread::sleep_for(std::chrono::milliseconds(actions_cooldown));
        }
        cooldown = false;
    }
}
