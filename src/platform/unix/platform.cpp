#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include "platform/unix/platform.hpp"
#include "ui/feedback.hpp"
#include "runtime.hpp"
#include "excepts.hpp"

namespace platform {

std::shared_ptr<control_impl> user_windowing::make_api(e_windowings picked_api) {
    switch (picked_api){
        case X11:
            return std::make_shared<x11_windowing>();
            break;
        case Wayland:
            return std::make_shared<wayland_windowing>();
            break;
        default:
            class none_windowing : public control_impl{
            public:
                ~none_windowing()                                   {}
                void init()                                          override {
                    throw excepts::error("Unix platform chooser returns emply implementations", "platform.cpp");
                }
                void action_button(int keysym, bool pressing)       override {}
                void handle_events(s_event_decl *events_decl)       override {}
            };
            return std::make_shared<none_windowing>();
    };
};

/// Returns shared_ptr to implementation class
std::shared_ptr<control_impl> get_platform() {
    user_windowing windowing;
    if ((std::getenv("WAYLAND_DISPLAY") || override_wayland) && (!override_xorg)) {
        #ifdef USE_WAYLAND
            return windowing.make_api(Wayland);
        #else
            ui::warn("This build completed without Wayland support. Overriding X11.");
            return windowing.make_api(X11);
        #endif

    } else if (std::getenv("DISPLAY") || override_xorg) {
        return windowing.make_api(X11);
    } else {
        return windowing.make_api(Placeholder);
    }
}

} // namespace platform