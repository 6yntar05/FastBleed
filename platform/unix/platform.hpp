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
    std::shared_ptr<control_impl> make_api(int picked_api);
};

std::shared_ptr<control_impl> init();

}//namespace cirno