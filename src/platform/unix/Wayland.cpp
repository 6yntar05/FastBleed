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
    #include <libinput.h>

    #if defined (LINUX) || defined(__linux__)  || defined(ANDROID)
        #include <linux/input-event-codes.h>
    #else //if defined(__FreeBSD__) || defined(__BSD__)
        #include <dev/evdev/input-event-codes.h>
        // TODO: check events for BSD
    #endif
#endif

namespace platform {

#ifdef USE_WAYLAND
/*********************[  class wayland_windowing : control_impl {  ]**********************/
    wayland_windowing::~wayland_windowing() {
        libinput_unref(li);
    }

    void wayland_windowing::init() {
        // Just use libinput instead wayland client lol
        this->li = libinput_udev_create_context(&this->interface, NULL, udev_new());
        if (this->li == nullptr)
            throw excepts::error("Libinput is null", "Wayland.cpp");

        libinput_udev_assign_seat(li, "seat0");
    }

    void wayland_windowing::handle_events(s_event_decl* events_decl) {
        // todo: get rid while true
        while (true) {
            int rc = libinput_dispatch(li);
            if (rc != 0) { std::cerr << "dispatcher errors detected\n" << std::flush; break; }
            
            libinput_event* event = libinput_get_event(li);
            if (!event) continue;

            // Catch the event!
            switch (libinput_event_get_type(event)) {
                case LIBINPUT_EVENT_POINTER_BUTTON:
                    uint32_t button_index;
                    bool button_state;
                    libinput_event_pointer* ptrev;

                    // Handle event
                    ptrev = libinput_event_get_pointer_event(event);
                    button_index = libinput_event_pointer_get_button(ptrev);
                    button_state = libinput_event_pointer_get_button_state(ptrev);
                    //std::cout << button_index << "|" << button_state << '\n';

                    // Match with triggers
                    for (auto& match_decl : *events_decl) {
                        if (!match_decl->was_mouse) break;
                        if (button_index == 0x110 + match_decl->ev_button - 5)
                            match_decl->set_active(button_state);
                    }
                    break;
                
                default: break; // Not implemented
            }
            std::cout << std::flush;
            libinput_event_destroy(event);
        }
    }

    void wayland_windowing::action_button(int keysym, bool pressing) const {
        throw excepts::error("Not implemented", "Wayland.cpp");
    }

/*********************[ }; //class wayland_windowing : control_impl ]*********************/

#else /* If this build completed whithout Wayland support */

/*********************[  class wayland_windowing : control_impl {  ]**********************/
    wayland_windowing::~wayland_windowing() {}

    void wayland_windowing::init() {
        throw excepts::error("This build completed without Wayland support");
    }

    void wayland_windowing::handle_events(s_event_decl *events_decl) {
        throw excepts::error("This build completed without Wayland support");
    }
    
    void wayland_windowing::action_button(int keysym, bool pressing) const {
        throw excepts::error("This build completed without Wayland support");
    }
/*********************[ }; //class wayland_windowing : control_impl ]*********************/

#endif

} // namespace platform
