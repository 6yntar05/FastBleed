#include <iostream>
#include <vector>
#include <string.h>    //depr.
#include <chrono>
#include <thread>
#include <memory>
#ifdef USE_WAYLAND
    #include <X11/Xlib.h>
    #include <X11/extensions/XTest.h>
#else
    #define Display void
#endif

namespace cirno {
class control_impl {
public:
    virtual ~control_impl()                            = default;
    virtual int status()                               = 0;
    virtual int button(int keysym, int pressing)       = 0;
    virtual std::vector<int> get_buttons()             = 0;
};

class wayland_windowing : public control_impl {
public:
    Display *display;
    ~wayland_windowing();
    int status();
    int button(int keysym, int pressing);
    std::vector<int> get_buttons();
};
}