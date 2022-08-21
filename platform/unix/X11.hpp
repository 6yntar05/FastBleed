// Imported in X11.cpp. USE_X11 taken from there
#include <vector>

#ifdef USE_X11
    #include <X11/Xlib.h>
    #include <X11/extensions/XTest.h>
#endif

namespace cirno {
class control_impl {
public:
    virtual ~control_impl()                                 = default;
    virtual int init()                                      = 0;
    virtual int action_button(int keysym, bool pressing)    = 0;
    virtual int handle_events(int keysym, bool intercept)   = 0;
};

class x11_windowing : public control_impl {
private:
    #ifdef USE_X11
        Display *lclDisplay;
        Display *recDisplay;
        int lclScreen;
        Window rootWindow;
    #endif
public:
    ~x11_windowing();
    int init();
    int action_button(int keysym, bool pressing);
    int handle_events(int keysym, bool intercept);
};
}