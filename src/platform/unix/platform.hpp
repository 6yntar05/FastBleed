#include <iostream>
#include <vector>
#include <memory>
#include "runtime.hpp"

#ifdef USE_UINPUT
    #include <libinput.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

#ifdef USE_X11
    #include <X11/Xlib.h>
    #include <X11/extensions/XTest.h>
    #include <X11/extensions/record.h>

    typedef struct {
        Display *lclDisplay, *recDisplay;
        XRecordContext context;
        s_event_decl* events_decl;
    } s_XHeap;
#endif

enum e_controls { uinput, X11, Empty };

namespace platform {

class control_impl {
public:
    virtual ~control_impl() = default;
    virtual void init() = 0;
    virtual void handle_events(s_event_decl *events_decl) = 0;
    // Actions
    virtual void action_button(int keysym, bool pressing) const = 0;
};

class uinput_control : public control_impl {
private:
    #ifdef USE_UINPUT
        libinput* li;

        static int open_restricted(const char *path, int flags, void *user_data) {
            int fd = open(path, flags);
            return fd < 0 ? -errno : fd;
        }
        
        static void close_restricted(int fd, void *user_data) {
            close(fd);
        }
        
        constexpr static libinput_interface interface = {
            .open_restricted  = open_restricted,
            .close_restricted = close_restricted,
        };


        int fd;
    #endif
public:
    ~uinput_control();
    void init();
    void handle_events(s_event_decl *events_decl);
    void action_button(int keysym, bool pressing) const;
};

class x11_control : public control_impl {
private:
    #ifdef USE_X11
        Display *lclDisplay;
        Display *recDisplay;
        int lclScreen;
        Window rootWindow;
        static void eventCallback(XPointer xheap, XRecordInterceptData *data);
    #endif
public:
    ~x11_control();
    void init();
    void handle_events(s_event_decl *events_decl);
    void action_button(int keysym, bool pressing) const;
};

class user_control {
public:
    std::shared_ptr<control_impl> make_api(e_controls picked_api = Empty);
};

std::shared_ptr<control_impl> get_platform();

} // namespace platform