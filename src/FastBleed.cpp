#include <thread>
#include <chrono>
#include <random>
#include <csignal>
#include <functional>

#include "utils/config.hpp"         // Config <-> FastBleed layer
#include "ui/feedback.hpp"          // ui::info/warn/error
#include "utils/args.hpp"           // utils::parse_args()
#include "utils/timings.hpp"        // utils::calculate_timings()
#include "platform/platform.hpp"    // platform::init() returns platform-non-specifically abstraction
#include "properties.hpp"           // Constants
#include "runtime.hpp"              // Flags, constants and shared points

// Load hardcoded vars
float cps                       = c_cps;
float relation                  = c_relation;
unsigned int entropy_variation  = c_entropy_variation;
unsigned int actions_cooldown   = c_actions_cooldown;
std::string config_path         = c_config_path;

// Runtime flags
bool override_uinput   = false;
bool override_xorg      = false;
bool be_verbose         = false;

using control_ptr = const std::shared_ptr<platform::control_impl>;
void signal_handler(int signum) { exit(signum); }
void handle_actions(control_ptr control, utils::t_timings timings, s_event_decl *actions);
void handler_wrapper(control_ptr control, s_event_decl* arg) { control->handle_events(arg); }

int main(int argc, char* argv[]) {
    utils::parse_args(argc, argv);
    utils::c_config config {config_path};

    // Pick platform-non-specifically abstraction
    control_ptr control = platform::get_platform();
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
    handler_thread.join();

    return 0;
}

void handle_actions(control_ptr control, utils::t_timings timings, s_event_decl *actions) {
    bool cooldown = false; // If nothing needs to be done

    std::random_device rd; std::mt19937 gen_seed(rd());
    std::uniform_int_distribution<> entropy(
       -static_cast<int>(timings.entropy_variation),
        static_cast<int>(timings.entropy_variation)
    );
    // ^ Introduces randomness in the timings of pressing. Make clicks more natural. ^

    while (true) {
        for (const auto& macro : *actions) {
            if (!macro->is_active()) continue; // If macro not triggered
            cooldown = true;
            for (const auto& action : macro->actions) switch (action.action) {
                case ACT_CLICK: control->action_button(action.param , true); break;
                case ACT_RELEASE: control->action_button(action.param , false);  break;

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

                default:
                    // ACT_MOVE and ACT_TEXT_TYPE not implemented for now
                    break;
            };
        }
        if (!cooldown)
            std::this_thread::sleep_for(std::chrono::milliseconds(actions_cooldown));
        cooldown = false;
    }
}