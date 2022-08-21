#include <iostream>
#include <vector>
#include <memory>
#ifdef USE_X11
    #include <X11/Xlib.h>
#endif

struct s_event_decl;

namespace cirno {
class control_impl {
public:
    virtual ~control_impl()                                     = default;
    virtual int init()                                          = 0;
    virtual int action_button(int keysym, bool pressing)        = 0;
    virtual int handle_events(struct s_event_decl *events_decl) = 0;
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
    int handle_events(struct s_event_decl *events_decl);
};

class wayland_windowing : public control_impl {
public:
    ~wayland_windowing();
    int init();
    int action_button(int keysym, bool pressing);
    int handle_events(struct s_event_decl *events_decl);
};

class user_windowing {
public:
    std::shared_ptr<control_impl> make_api(int picked_api);
};

std::shared_ptr<control_impl> get_platform();

}//namespace cirno