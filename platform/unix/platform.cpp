#include <iostream>
#include <vector>
#include <memory>
#include "platform.hpp"

namespace cirno {

// Pattern "FACTORY" ^.^
std::shared_ptr<control_impl> user_windowing::make_api(int picked_api) {
    switch (picked_api){
        case 1:
            return std::make_shared<x11_windowing>();
            break;
        case 2:
            return std::make_shared<wayland_windowing>();
            break;
        default:
            class plug_windowing : public control_impl{
            public:
                ~plug_windowing()                               {}
                int init()                                      override {return -100;}
                int action_button(int keysym, bool pressing)    override {return -1;}
                int handle_events(int keysym, bool intercept)   override {return -1;}
            };
            return std::make_shared<plug_windowing>();
    };
};

std::shared_ptr<control_impl> get_platform() {
    user_windowing windowing;
    if (std::getenv("WAYLAND_DISPLAY")) {
        return windowing.make_api(2);
    } else if(std::getenv("DISPLAY")) {
        return windowing.make_api(1);
    } else {
        return windowing.make_api(0);
    }
}

}//namespace cirno