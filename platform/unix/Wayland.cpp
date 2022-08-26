#include "../../properties.hpp"
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Wayland.hpp"
#include "../../ui/feedback.hpp"

#ifdef USE_WAYLAND
    #include <wayland-client.h>
    #include <wayland-server.h>
    #include <wayland-client-protocol.h>
#endif

namespace cirno {

#ifdef USE_WAYLAND

    wl_compositor *compositor = NULL;

    void pointer_register_handler(
            void *data, struct wl_registry *registry, uint32_t id,
            const char *interface, uint32_t version) {

        uint32_t registry_pointer_id;

        if (static_cast<std::string>(interface) == "zwlr_virtual_pointer_manager_v1") {
            std::cerr << "Found interface: " << interface << "; ID: " << id << std::endl;
            registry_pointer_id = id;
        }
    }

    struct wl_registry_listener registry_listener = {
        pointer_register_handler
    };

/*********************[  class wayland_windowing : control_impl {  ]**********************/
    wayland_windowing::~wayland_windowing() {
        wl_display_disconnect(display);
    }

    int wayland_windowing::init() {
        display = wl_display_connect(NULL);
        if (display == NULL) {
            return -1;
        }

        registry = wl_display_get_registry(display);
        if (registry == NULL) {
            return -2;
        }

        wl_registry_add_listener(registry, &registry_listener, NULL);
        
        wl_display_dispatch(display);
        wl_display_roundtrip(display);

        wl_registry_destroy(registry);

        return 0;
    }

    int wayland_windowing::action_button(int keysym, bool pressing) {
        display = wl_display_connect(NULL);
        std::cerr << "Not implemented for now!\n";
        return -1;
    }

    int wayland_windowing::handle_events(struct s_event_decl *events_decl) {
        display = wl_display_connect(NULL);
        std::cerr << "Not implemented for now!\n";
        return -1;
    }
/*********************[ }; //class wayland_windowing : control_impl ]*********************/

#else /* If this build completed whithout Wayland support */

/*********************[  class wayland_windowing : control_impl {  ]**********************/
    wayland_windowing::~wayland_windowing()                                 {}
    int wayland_windowing::init()                                           {return -202;}
    int wayland_windowing::action_button(int keysym, bool pressing)         {return -1;}
    int wayland_windowing::handle_events(struct s_event_decl *events_decl)  {return -1;}
/*********************[ }; //class wayland_windowing : control_impl ]*********************/

#endif

}//cirno
