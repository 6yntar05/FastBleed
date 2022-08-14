#include <cstdlib>
#include <memory>
#include <functional>
#include "../../ui/event.h"     // cirno::raise()
#include "../../properties.h"   // defined USE_X11, USE_WAYLAND

namespace cirno {
class control_impl {
public:
    virtual int status()                               = 0;
    virtual int button(int keysym, int pressing) = 0;
    virtual std::vector<int> get_buttons()             = 0;
};
}//namespace cirno

#ifdef USE_WAYLAND
    #include "Wayland.cpp"
#else
    class wayland_windowing : public cirno::control_impl {
    public:
        int status()                               override {return -101;}
        int button(int keysym, int pressing) override {return -1;}
        std::vector<int> get_buttons()             override {return {};}
    }; // This build completed whithout Wayland support
#endif

#ifdef USE_X11
    #include "X11.cpp"
#else
    class x11_windowing : public cirno::control_impl {
    public:
        int status()                               override {return -102;}
        int button(int keysym, int pressing) override {return -1;}
        std::vector<int> get_buttons()             override {return {};}
    }; // This build completed whithout X11 support
#endif

namespace cirno {
class user_windowing {
public:
    static std::shared_ptr<control_impl> make_api(int picked_api) {
        switch (picked_api){
            case 1:
                return std::make_shared<wayland_windowing>();
                break;
            case 2:
                return std::make_shared<x11_windowing>();
                break;
            default:
                class plug_windowing : public control_impl{
                    int status()                               override {return -100;}
                    int button(int keysym, int pressing)  override {return -1;}
                    std::vector<int> get_buttons()             override {return {};}
                };
                return std::make_shared<plug_windowing>();
        }
    }
};

std::shared_ptr<control_impl> init() {
    user_windowing bebra;
    if(std::getenv("WAYLAND_DISPLAY")) { 
        return bebra.make_api(1);
    } else if(std::getenv("DISPLAY")) {
        return bebra.make_api(2);
    } else {
        return bebra.make_api(0);
    }
};
}//namespace cirno