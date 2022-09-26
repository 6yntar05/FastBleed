#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "platform/unix/platform.hpp"
#include "ui/feedback.hpp"
#include "runtime.hpp"
#include <excepts.hpp>

#ifdef USE_WAYLAND
    #include <wayland-client.h>
    #include <wayland-server.h>
    #include <wayland-client-protocol.h>
    #include <wayland-server-protocol.h>
#endif

namespace platform {

#ifdef USE_WAYLAND

    wl_compositor *compositor = NULL;

    void pointer_register_handler(
            void *data, struct wl_registry *registry, uint32_t id,
            const char *interface, uint32_t version) {

        //uint32_t registry_pointer_id;

        if (static_cast<std::string>(interface) == "zwlr_virtual_pointer_manager_v1") {
            std::cerr << "Found interface: " << interface << "; ID: " << id << std::endl;
            //registry_pointer_id = id;
        }
    }

    struct wl_registry_listener registry_listener = {
        pointer_register_handler,
        {}
    };

/*********************[  class wayland_windowing : control_impl {  ]**********************/
    wayland_windowing::~wayland_windowing() {
        wl_display_disconnect(display);
    }

    void wayland_windowing::init() {
        if (this->display = wl_display_connect(NULL); this->display == NULL) {
            throw excepts::error("Display is null", "Wayland.cpp");
        }

        if (this->registry = wl_display_get_registry(this->display); this->registry == NULL) {
            throw excepts::error("Display registry is null", "Wayland.cpp");
        }

        wl_registry_add_listener(this->registry, &registry_listener, NULL);
        
        wl_display_dispatch(this->display);
        wl_display_roundtrip(this->display);

        //wl_keyboard *keyboard = wl_seat_get_keyboard(this->seat);
        //wl_pointer *pointer = wl_seat_get_pointer(this->seat);
        //wl_pointer_send_button(this->resource, 0, 0, 1, 1);

        wl_registry_destroy(this->registry);
    }

    void wayland_windowing::action_button(int keysym, bool pressing) {
        //display = wl_display_connect(NULL);
        std::cerr << "Not implemented for now!\n";
        throw excepts::error("Not implemented", "Wayland.cpp");
    }

    void wayland_windowing::handle_events(struct s_event_decl *events_decl) {
        //display = wl_display_connect(NULL);
        throw excepts::error("Not implemented", "Wayland.cpp");
    }
/*********************[ }; //class wayland_windowing : control_impl ]*********************/

#else /* If this build completed whithout Wayland support */

/*********************[  class wayland_windowing : control_impl {  ]**********************/
    wayland_windowing::~wayland_windowing() {}

    void wayland_windowing::init() {
        throw excepts::error("This build completed without Wayland support");
    }

    void wayland_windowing::action_button(int keysym, bool pressing) {
        throw excepts::error("This build completed without Wayland support");
    }

    void wayland_windowing::handle_events(struct s_event_decl *events_decl) {
        throw excepts::error("This build completed without Wayland support");
    }
/*********************[ }; //class wayland_windowing : control_impl ]*********************/

#endif

} // namespace platform
