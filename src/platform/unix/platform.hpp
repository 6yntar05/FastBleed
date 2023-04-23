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
    #include <wayland-client.h>
    #include <wayland-server.h>
    #include <wayland-client-protocol.h>
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