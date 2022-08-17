// Imported in X11.cpp. USE_X11 taken from there
#include <vector>

#ifdef USE_X11
    #include <X11/Xlib.h>
    #include <X11/extensions/XTest.h>
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
}