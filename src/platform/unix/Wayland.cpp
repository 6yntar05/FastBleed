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

    //#if defined (LINUX) || defined(__linux__)  || defined(ANDROID)
        #include <cstring>
        #include <linux/input-event-codes.h>
        #include <linux/input.h>
        #include <linux/uinput.h>
        #include <sys/types.h>
        #include <sys/stat.h>
        #include <fcntl.h>
        #include <unistd.h>
    //#else
    //    #error uinput available only for linux kernel
    //#endif
#endif

namespace platform {

#ifdef USE_WAYLAND
void emit(int fd, int type, int code, int val) {
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;
   /* timestamp values below are ignored */
   ie.time.tv_sec = 0;
   ie.time.tv_usec = 0;

   write(fd, &ie, sizeof(ie));
}

static constexpr int EVDEV_MOUSEKEYS = 0x110 - 1;

/*********************[  class wayland_windowing : control_impl {  ]**********************/
    wayland_windowing::~wayland_windowing() {
        libinput_unref(li);
        ioctl(fd, UI_DEV_DESTROY);
        close(fd);
    }

    void wayland_windowing::init() {
        /// LIBINPUT ///
        // Just use libinput instead wayland client lol
        this->li = libinput_udev_create_context(&this->interface, NULL, udev_new());
        if (this->li == nullptr)
            throw excepts::error("Libinput is null", "Wayland.cpp");

        libinput_udev_assign_seat(li, "seat0");

        /// UINPUT ///
        struct uinput_setup usetup;
        this->fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

        /* enable mouse button left and relative events */
        ioctl(fd, UI_SET_EVBIT, EV_KEY);
        ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
        ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);

        ioctl(fd, UI_SET_EVBIT, EV_REL);
        ioctl(fd, UI_SET_RELBIT, REL_X);
        ioctl(fd, UI_SET_RELBIT, REL_Y);

        memset(&usetup, 0, sizeof(usetup));
        usetup.id.bustype = BUS_USB;
        usetup.id.vendor = 0x1234; /* sample vendor */
        usetup.id.product = 0x5678; /* sample product */
        strcpy(usetup.name, "Generic USB Mouse");

        ioctl(fd, UI_DEV_SETUP, &usetup);
        ioctl(fd, UI_DEV_CREATE);
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
                        if (button_index == EVDEV_MOUSEKEYS + match_decl->ev_button - 4)
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
        emit(fd, EV_KEY, EVDEV_MOUSEKEYS + keysym, pressing);
        emit(fd, EV_SYN, SYN_REPORT, 0);
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
