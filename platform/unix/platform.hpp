#include <iostream>
#include <vector>
#include <string.h>    //depr.
#include <chrono>
#include <thread>
#include <memory>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

namespace cirno {
class control_impl {
public:
    virtual ~control_impl()                            = default;
    virtual int status()                               = 0;
    virtual int button(int keysym, int pressing)       = 0;
    virtual std::vector<int> get_buttons()             = 0;
};

class x11_windowing : public control_impl {
public:
    Display *display;
    ~x11_windowing();
    int status();
    int button(int keysym, int pressing);
    std::vector<int> get_buttons();
};

class wayland_windowing : public control_impl {
public:
    Display *display;
    ~wayland_windowing();
    int status();
    int button(int keysym, int pressing);
    std::vector<int> get_buttons();
};

class user_windowing {
public:
    static std::shared_ptr<control_impl> make_api(int picked_api) {
        switch (picked_api){
            case 1:
                //return std::make_shared<wayland_windowing>();
                return std::make_shared<x11_windowing>();
                break;
            case 2:
                return std::make_shared<wayland_windowing>();
                break;
            default:
                class plug_windowing : public control_impl{
                public:
                    ~plug_windowing()                          {}
                    int status()                               override {return -100;}
                    int button(int keysym, int pressing)       override {return -1;}
                    std::vector<int> get_buttons()             override {return {};}
                };
                return std::make_shared<plug_windowing>();
        }
    }
};

std::shared_ptr<control_impl> init() {
    user_windowing bebra;
    if(std::getenv("WAYLAND_DISPLAY")) { 
        return bebra.make_api(2);
    } else if(std::getenv("DISPLAY")) {
        return bebra.make_api(1);
    } else {
        return bebra.make_api(0);
    }
};

}