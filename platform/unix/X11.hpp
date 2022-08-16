#include <iostream>
#include <vector>
#include <string.h>    //depr.
#include <chrono>
#include <thread>
#include <memory>
#ifdef USE_X11
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

class x11_windowing : public control_impl {
public:
    Display *display;
    ~x11_windowing();
    int status();
    int button(int keysym, int pressing);
    std::vector<int> get_buttons();
};
}