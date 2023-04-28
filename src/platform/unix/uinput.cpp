#include "properties.hpp"
#include "platform/unix/platform.hpp"
#include "ui/feedback.hpp"
#include "runtime.hpp"
#include "excepts.hpp"
#include <thread>
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_UINPUT
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

#ifdef USE_UINPUT

static constexpr int EVDEV_MOUSEKEYS = BTN_LEFT - 1; 

static void setup_uinput_device(int& fd) {
    uinput_setup usetup {{}, "Generic USB Mouse", {}};
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
    //ioctl(fd, UI_SET_EVBIT, EV_REL);
    //ioctl(fd, UI_SET_RELBIT, REL_X);
    //ioctl(fd, UI_SET_RELBIT, REL_Y);

    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);
}

/*********************[  class uinput_control : control_impl {  ]**********************/
    uinput_control::~uinput_control() {
        libinput_unref(li);
        ioctl(fd, UI_DEV_DESTROY);
        close(fd);
    }

    void uinput_control::init() {
        /// LIBINPUT ///
        // Just use libinput instead uinput client lol
        this->li = libinput_udev_create_context(&this->interface, NULL, udev_new());
        if (this->li == nullptr)
            throw excepts::error("Libinput is null", "uinput.cpp");

        libinput_udev_assign_seat(li, "seat0");

        /// UINPUT ///
        setup_uinput_device(this->fd);
    }

    void uinput_control::handle_events(s_event_decl* events_decl) {
        // todo: get rid while true
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(c_actions_cooldown));
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
                        std::cout << button_index << '\n';
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

    void uinput_control::action_button(int keysym, bool pressing) const {
        std::cout << keysym << ":" << pressing << '\n';
        input_event ie { {0, 0}, EV_KEY,
            static_cast<ushort>(EVDEV_MOUSEKEYS + keysym),
            pressing
        };
        write(fd, &ie, sizeof(ie));

        ie.code = EV_SYN;
        ie.type = SYN_REPORT;
        ie.value = 0;
        write(fd, &ie, sizeof(ie));
    }

/*********************[ }; //class uinput_control : control_impl ]*********************/

#else /* If this build completed whithout uinput support */

/*********************[  class uinput_control : control_impl {  ]**********************/
    uinput_control::~uinput_control() {}

    void uinput_control::init() {
        throw excepts::error("This build completed without uinput support");
    }

    void uinput_control::handle_events(s_event_decl *events_decl) {
        throw excepts::error("This build completed without uinput support");
    }
    
    void uinput_control::action_button(int keysym, bool pressing) const {
        throw excepts::error("This build completed without uinput support");
    }
/*********************[ }; //class uinput_control : control_impl ]*********************/

#endif

} // namespace platform
