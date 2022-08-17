#include <iostream>
#include <vector>
#include <memory>
#ifdef USE_X11
    #include <X11/Xlib.h>
#endif

namespace cirno {
class control_impl {
public:
    virtual ~control_impl()                         = default;
    virtual int init()                              = 0;
    virtual int button(int keysym, int pressing)    = 0;
    virtual std::vector<int> get_buttons()          = 0;
};

class x11_windowing : public control_impl {
public:
    #ifdef USE_X11
        Display *display;
    #endif
    ~x11_windowing();
    int init();
    int button(int keysym, int pressing);
    std::vector<int> get_buttons();
};

class wayland_windowing : public control_impl {
public:
    ~wayland_windowing();
    int init();
    int button(int keysym, int pressing);
    std::vector<int> get_buttons();
};

class user_windowing {
public:
    std::shared_ptr<control_impl> make_api(int picked_api);
};

std::shared_ptr<control_impl> get_platform();

}//namespace cirno