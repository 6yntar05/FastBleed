#include <iostream>
#include <vector>
#include <memory>
#include "runtime.hpp"

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

#ifdef USE_WAYLAND
    #include <libinput.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

enum e_windowings { X11, Wayland, Empty };

namespace platform {

class control_impl {
public:
    virtual ~control_impl() = default;
    virtual void init() = 0;
    virtual void handle_events(s_event_decl *events_decl) = 0;
    // Actions
    virtual void action_button(int keysym, bool pressing) const = 0;
};

class x11_windowing : public control_impl {
private:
    #ifdef USE_X11
        Display *lclDisplay;
        Display *recDisplay;
        int lclScreen;
        Window rootWindow;
        static void eventCallback(XPointer xheap, XRecordInterceptData *data);
    #endif
public:
    ~x11_windowing();
    void init();
    void handle_events(s_event_decl *events_decl);
    void action_button(int keysym, bool pressing) const;
};

class wayland_windowing : public control_impl {
private:
    #ifdef USE_WAYLAND
        static int open_restricted(const char *path, int flags, void *user_data) {
            int fd = open(path, flags);
            return fd < 0 ? -errno : fd;
        }
        
        static void close_restricted(int fd, void *user_data) {
            close(fd);
        }
        
        constexpr static libinput_interface interface = {
            .open_restricted = open_restricted,
            .close_restricted = close_restricted,
        };

        libinput* li;
    #endif
public:
    ~wayland_windowing();
    void init();
    void handle_events(s_event_decl *events_decl);
    void action_button(int keysym, bool pressing) const;
};

class user_windowing {
public:
    std::shared_ptr<control_impl> make_api(e_windowings picked_api = Empty);
};

std::shared_ptr<control_impl> get_platform();

} // namespace platform