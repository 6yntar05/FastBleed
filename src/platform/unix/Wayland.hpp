#include <vector>

#define USE_WAYLAND

#ifdef USE_WAYLAND
    #include <wayland-client.h>
    #include <wayland-server.h>
    #include <wayland-client-protocol.h>
#endif

struct s_event_decl {
    unsigned int count;
    unsigned int *ev_button;
    bool *flag;
    unsigned int *act_button;
};

namespace cirno {

class control_impl {
public:
    virtual ~control_impl()                                     = default;
    virtual int init()                                          = 0;
    virtual int action_button(int keysym, bool pressing)        = 0;
    virtual int handle_events(struct s_event_decl *events_decl) = 0;
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
    int init() override;
    int action_button(int keysym, bool pressing) override;
    int handle_events(struct s_event_decl *events_decl) override;
};

}