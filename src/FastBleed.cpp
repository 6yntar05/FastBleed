#include <thread>
#include <iostream>
#include <functional>

#include "properties.hpp"                                       // Default values; Build properties
#include "config.hpp"                                           // Config <-> FastBleed layer
#include "ui/feedback.hpp"                                      // cirno::error()/warn()
#include "utils/args.hpp"                                       // parse_args()
#include "utils/timings.hpp"                                    // calculate_timings()
#include "platform/platform.hpp"                                // cirno::init() returns platform-non-specifically abstraction

/*************[Default values]*************/
float cps                        = c_cps;                // Click Per Second
float relation                   = c_relation;           // Hold time / Release time
unsigned int entropy_variation   = c_entropy_variation;  // Introduces randomness in the timings of pressing
unsigned int actions_cooldown    = c_actions_cooldown;
std::string config_path          = c_config_path;
bool override_wayland, override_xorg, use_gui = false, be_verbose = false;

void handle_actions(std::shared_ptr<cirno::control_impl> control, t_timings timings, s_event_decl *actions);

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    cirno::c_config config {config_path};

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
        exit(status);
    }

    t_timings timings = calculate_timings(cps, relation, entropy_variation);        // Calculate delays around CPS/Relation value
    
    // Read config -> declare events
    s_event_decl events_decl = config.parse();
    std::vector<e_actions> vec_action_script;
    std::vector<unsigned int> vec_action_params;

    // Start actions handler thread
    std::thread actions_thread(handle_actions, control, timings, &events_decl);
    actions_thread.detach();

    // Call events handler
    cirno::general_state(true);
    if (control->handle_events(&events_decl) != 0) {
        cirno::error("Unable to handling events!");
        exit(1);
    }
    
    return 0;
}

void handle_actions(std::shared_ptr<cirno::control_impl> control, t_timings timings, s_event_decl *actions) {
    bool cooldown = false; // If nothing needs to be done

    std::random_device rd; std::mt19937 gen_seed(rd());
    std::uniform_int_distribution<> entropy(-timings.entropy_variation, timings.entropy_variation);
    // ^ Introduces randomness in the timings of pressing. Makes clicks more natural. ^

    while (true) {
        for (unsigned int i = 0; i < actions->count; i++) {

            if (actions->flag[i]) {
                cooldown = true;
                for (int a = 0; a < static_cast<int>(actions->action->size()); a++) {

                    switch (actions->action->at(a)) {
                        case ACT_CLICK:
                            control->action_button(actions->action_param[i].at(a) , true);
                            break;
                        
                        case ACT_RELEASE:
                            control->action_button(actions->action_param[i].at(a) , false);
                            break;

                        case ACT_CLICKER:
                            control->action_button(actions->action_param[i].at(a) , true);
                            std::this_thread::sleep_for(std::chrono::milliseconds(timings.hold_time + entropy(gen_seed)));
                            control->action_button(actions->action_param[i].at(a) , false);
                            std::this_thread::sleep_for(std::chrono::milliseconds(timings.release_time + entropy(gen_seed)));
                            break;
                        
                        case ACT_DELAY:
                            std::this_thread::sleep_for(std::chrono::milliseconds(actions->action_param[i].at(a)));
                            cooldown = false;
                            break;
                        
                        case ACT_MOVE:
                            // Not implemented
                            break;
                        
                        case ACT_SYS_EXEC:
                            // Not implemented
                            break;
                        
                        case ACT_TEXT_TYPE:
                            // Not implemented
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