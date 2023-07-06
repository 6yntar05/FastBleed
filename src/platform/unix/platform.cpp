#include "platform/unix/platform.hpp"
#include "ui/feedback.hpp"

#include "runtime.hpp"
#include "excepts.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

namespace platform {

std::shared_ptr<control_impl> user_control::make_api(e_controls picked_api) {
    switch (picked_api) {
        case uinput:
            return std::make_shared<uinput_control>();
            break;
        case X11:
            return std::make_shared<x11_control>();
            break;
        default:
            class none_control : public control_impl{
            public:
                ~none_control() {}
                void init() override {
                    throw excepts::error("Unix platform chooser returns empty implementations", "platform.cpp", "", "Is Xorg running (if overrided)?");
                }
                void handle_events(s_event_decl *events_decl) override {}
                void action_button(int keysym, bool pressing) const override {}
            };
            return std::make_shared<none_control>();
    };
};

/// Returns shared_ptr to implementation class
std::shared_ptr<control_impl> get_platform() {
    user_control windowing;
    if (!override_xorg) {
        #ifdef USE_UINPUT
            return windowing.make_api(uinput);
        #else
            ui::warn("This build completed without uinput support. Overriding X11.");
            return windowing.make_api(X11);
        #endif

    } else if (std::getenv("DISPLAY") || override_xorg)
        return windowing.make_api(X11);
    else {
        ui::warn("env DISPLAY not set");
        return windowing.make_api(Empty);
    }
}

} // namespace platform