#include "platform.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "../../ui/feedback.hpp"

namespace cirno {

std::shared_ptr<control_impl> user_windowing::make_api(e_windowings picked_api) {
    std::shared_ptr<control_impl> debug;
    switch (picked_api){
        case X11:
            debug = std::make_shared<x11_windowing>();
            break;
        case Wayland:
            debug = std::make_shared<wayland_windowing>();
            break;
        default:
            class plug_windowing : public control_impl{
            public:
                ~plug_windowing()                                   {}
                int init()                                          override {return -100;}
                int action_button(int keysym, bool pressing)        override {return -1;}
                int handle_events(struct s_event_decl *events_decl) override {return -1;}
            };
            debug = std::make_shared<plug_windowing>();
    };

    return debug;
};

std::shared_ptr<control_impl> get_platform() {
    user_windowing windowing;
    if ((std::getenv("WAYLAND_DISPLAY") || override_wayland) && (!override_xorg)) {
        #ifdef USE_WAYLAND
            return windowing.make_api(Wayland);
        #else
            warn("This build completed without Wayland support. Overriding X11.");
            return windowing.make_api(X11);
        #endif

    } else if (std::getenv("DISPLAY") || override_xorg) {
        return windowing.make_api(X11);
    } else {
        return windowing.make_api(Placeholder);
    }
}

}//namespace cirno