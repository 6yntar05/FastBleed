#include <iostream>
#include <vector>
#include <memory>
#include "config.hpp"

#ifdef USE_X11
    #include <X11/Xlib.h>
    #include <X11/extensions/XTest.h>
#endif

#ifdef USE_WAYLAND
    #include <wayland-client.h>
    #include <wayland-server.h>
    #include <wayland-client-protocol.h>
#endif

extern bool override_wayland, override_xorg;
enum e_windowings { X11, Wayland, Placeholder };

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
private:
    #ifdef USE_WAYLAND
        wl_display *display = NULL;
        wl_registry *registry = NULL;
        wl_seat *seat = NULL;
        wl_resource *resource = NULL;

        /*
        size_t keymap_len;
        struct keymap_entry *keymap;

        uint32_t mod_status;
        size_t command_count;
        struct wtype_command *commands;
        */
    #endif
public:
    ~wayland_windowing();
    int init();
    int action_button(int keysym, bool pressing);
    int handle_events(struct s_event_decl *events_decl);
};

class user_windowing {
public:
    std::shared_ptr<control_impl> make_api(e_windowings picked_api);
};

std::shared_ptr<control_impl> get_platform();

}//namespace cirno