#include "ui/mainwindow.h"

#include <QMainWindow>
#include <QApplication>

#include <qapplication.h>
#include <thread>
#include <chrono>
#include <random>
#include <csignal>
#include <functional>

#include "utils/config.hpp"                                     // Config <-> FastBleed layer
#include "properties.hpp"                                       // Constants
#include "ui/feedback.hpp"                                      // ui::info/warn/error
#include "utils/args.hpp"                                       // utils::parse_args()
#include "utils/timings.hpp"                                    // utils::calculate_timings()
#include "platform/platform.hpp"                                // platform::init() returns platform-non-specifically abstraction
#include "runtime.hpp"                                          // Flags, constants and shared points


/// Load hardcoded vars
float cps                       = c_cps;
float relation                  = c_relation;
unsigned int entropy_variation  = c_entropy_variation;
unsigned int actions_cooldown   = c_actions_cooldown;
std::string config_path         = c_config_path;

// Runtime flags
bool use_gui            = c_use_gui;
bool override_wayland   = false;
bool override_xorg      = false;
bool be_verbose         = false;

void signal_handler(int signum);
void handle_actions(std::shared_ptr<platform::control_impl> control, utils::t_timings timings, s_event_decl *actions);
void handler_wrapper(std::shared_ptr<platform::control_impl> control, s_event_decl* arg) { control->handle_events(arg); }

int main(int argc, char* argv[]) {
    utils::parse_args(argc, argv);
    utils::c_config config {config_path};

    // Gui
    QApplication a {argc, argv};
    MainWindow w;
    if (use_gui)
        w.show();

    // Pick platform-non-specifically abstraction
    std::shared_ptr<platform::control_impl> control = platform::get_platform();
    control->init();

    // Calculate delays around CPS/Relation value
    utils::t_timings timings = utils::calculate_timings(cps, relation, entropy_variation);
    
    // Read config -> declare events
    s_event_decl events_decl = config.parse();

    // Start actions handler thread
    std::thread actions_thread(handle_actions, control, timings, &events_decl);
    actions_thread.detach();

    // Call events handler
    signal(SIGINT, signal_handler);
    ui::msg("Starting action handler");
    std::thread handler_thread(handler_wrapper, control, &events_decl);
    handler_thread.detach();
    //handler_thread.join();

    return a.exec();
}

void signal_handler(int signum) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";
   exit(signum);
}

void handle_actions(std::shared_ptr<platform::control_impl> control, utils::t_timings timings, s_event_decl *actions) {
    bool cooldown = false; // If nothing needs to be done

    std::random_device rd; std::mt19937 gen_seed(rd());
    std::uniform_int_distribution<> entropy(
       -static_cast<int>(timings.entropy_variation),
        static_cast<int>(timings.entropy_variation)
    );
    // ^ Introduces randomness in the timings of pressing. Make clicks more natural. ^

    while (true) {
        
        //for (auto& macro : *actions) {
        for (unsigned int i = 0; i < actions->size(); i++) {
            if (actions->at(i)->is_active()) {
                cooldown = true;

                for (auto& action : actions->at(i)->actions) {
                    switch (action.action) {
                        case ACT_CLICK:
                            control->action_button(action.param , true);
                            break;
                        
                        case ACT_RELEASE:
                            control->action_button(action.param , false);
                            break;

                        case ACT_CLICKER:
                            control->action_button(action.param , true);
                            std::this_thread::sleep_for(std::chrono::milliseconds(timings.hold_time + entropy(gen_seed)));
                            control->action_button(action.param , false);
                            std::this_thread::sleep_for(std::chrono::milliseconds(timings.release_time + entropy(gen_seed)));
                            break;
                        
                        case ACT_DELAY:
                            std::this_thread::sleep_for(std::chrono::milliseconds(action.param));
                            cooldown = false;
                            break;
                        
                        case ACT_MOVE:
                            // Not implemented
                            break;
                        
                        case ACT_SYS_EXEC:
                            // It's also dangerous to use
                            control->exec("/bin/sh -c 'touch ./test'");
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