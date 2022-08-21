#include <cmath>
#include <chrono>
#include <random>
#include <thread>
#include <iostream>
#include <functional>
#include <boost/program_options.hpp>
#include "ui/event.hpp"                           // cirno::error()
#include "properties.hpp"                         // Default values
#include "platform/platform_picker.hpp"           // cirno::init() returns platform-non-specifically abstraction

float cps               = c_cps;                  // Click Per Second
float relation          = c_relation;             // Hold time / Release time
unsigned int entropy_variation = c_entropy_variation;    // Introduces randomness in the timings of pressing
// = Default values from "properties.hpp"

bool sigint_flag(false);

extern bool glob_a_enabled;
bool glob_a_enabled = false;
extern bool glob_b_enabled;
bool glob_b_enabled = false;

namespace po = boost::program_options;
int  parse_args(int argc, char* argv[]);

struct s_event_decl {
    unsigned int button;
    bool *flag;
};

typedef struct s_timings {
    int hold_time;
    int release_time;
    unsigned int entropy_variation;
} t_timings;
t_timings calculate_timings(float cps, float relation, unsigned int entropy_variation);

void do_click(std::shared_ptr<cirno::control_impl> control, int keysym, t_timings timings);

#ifdef DEBUG
    void debug_bench(std::shared_ptr<cirno::control_impl> control, int button);
#endif

int main(int argc, char* argv[]) {
    parse_args(argc, argv);

    std::shared_ptr<cirno::control_impl> control = cirno::get_platform();           // Pick platform-non-specifically abstraction

    int status = control->init();
    switch (status) {
        case -100:
            std::cerr << "No displays found.\n";
            break;
        case -102:
            std::cerr << "This build completed without X11 support.\n";
            break;
        case -101:
            std::cerr << "This build completed without Wayland support.\n";
            break;
    }; if (status < 0) exit(1);

    #ifdef DEBUG
        //debug_bench(control, 1);
    #endif

    t_timings timings = calculate_timings(cps, relation, entropy_variation);                           // Calculate delays around CPS/Relation value
    // Declare events
    /*
    s_event_decl events_decl[2];
    events_decl[0].button = 8;
    events_decl[0].flag = &glob_a_enabled;

    events_decl[1].button = 9;
    events_decl[1].flag = &glob_b_enabled;
    */
    std::thread click_a_thread(do_click, control, 3, timings);
    click_a_thread.detach();

    std::cerr << control->handle_events(8, true/*, std::function...*/) << std::endl;

    return 0;
}

int parse_args(int argc, char* argv[]) {
    po::options_description desc("Usage: gofra [ options ... ]\n\tWhere options");
    desc.add_options()
        ("help,h", "Help page")
        ("cps,c", po::value<float>(&cps), "Clicks Per Second: float (0.0:500.0)")
        ("relation,r", po::value<float>(&relation), "'Hold time'/'Release time' relation: float (0.0:500/cps)")
        //("entropy,e", po::value<unsigned int>(&entropy_variation), "Entropy range (value+-delays): uint [0:?)")
    ;

    po::variables_map args;
    po::store(po::parse_command_line(argc, argv, desc), args);
    po::notify(args);

    if (args.count("help")) {
        std::cout << desc << "\n";
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
    ret.hold_time    = total_click_time / 2;    
    ret.release_time = total_click_time / 2;

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
                ret.hold_time    = x;
                ret.release_time = y;
            }// else {break;}
        }
    }

    if (best == total_click_time) {
        cirno::warn("Failed to calculate timings! Fallback values are used.");
        return calculate_timings(c_cps, c_relation, c_entropy_variation);    // Incorrect build-in values can lead to looping recursion
    }

    #ifdef DEBUG
        std::cerr << "[DEBUG] calculate_timings() => " << ret.hold_time << "/" << ret.release_time << std::endl;
    #endif
    return ret;
}

void do_click(std::shared_ptr<cirno::control_impl> control, int keysym, t_timings timings) {
    std::random_device rd; std::mt19937 gen_seed(rd());                    // !!
    std::uniform_int_distribution<> entropy(-timings.entropy_variation, timings.entropy_variation); // Introduces randomness in the timings of pressing
    while (true) {
        if (glob_a_enabled) {
            control->action_button(3, true);
            std::this_thread::sleep_for(std::chrono::milliseconds(timings.hold_time + entropy(gen_seed)));
            control->action_button(3, false);
            std::this_thread::sleep_for(std::chrono::milliseconds(timings.release_time + entropy(gen_seed)));
        }
        if (glob_b_enabled) {
            control->action_button(1, true);
            std::this_thread::sleep_for(std::chrono::milliseconds(timings.hold_time + entropy(gen_seed)));
            control->action_button(1, false);
            std::this_thread::sleep_for(std::chrono::milliseconds(timings.release_time + entropy(gen_seed) ));
        }
        if (!glob_a_enabled || !glob_b_enabled) {
            std::this_thread::sleep_for(std::chrono::milliseconds(timings.hold_time));
        }
    }
}

#ifdef DEBUG
    void debug_bench(std::shared_ptr<cirno::control_impl> control, int button) {
        auto start_time = std::chrono::steady_clock::now();
        control->action_button(button, true);
        auto stop_time = std::chrono::steady_clock::now();
        control->action_button(button, false);
        auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time);
        std::cout << "[DEBUG] Benchmark click time: " << elapsed_ns.count() << " ns\n";
    }
#endif