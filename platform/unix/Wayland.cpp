#include "../../properties.hpp"
#include <vector>
#include <memory>
#include "Wayland.hpp"
#ifdef USE_WAYLAND
    #include <X11/Xlib.h>
    #include <X11/extensions/XTest.h>
#else
    // empty
#endif

namespace cirno {

#ifdef USE_WAYLAND

//class wayland_windowing : control_impl {
wayland_windowing::~wayland_windowing() {
    XCloseDisplay(wayland_windowing::display);
}

int wayland_windowing::status() {
    wayland_windowing::display = XOpenDisplay(NULL); //Take out!
    if (wayland_windowing::display == NULL){
        return -1;
    };
    return 0;
}

int wayland_windowing::button(int keysym, int pressing) {
    XTestFakeButtonEvent(wayland_windowing::display, keysym, pressing, CurrentTime);
    XFlush(wayland_windowing::display);
    return 0;
}

std::vector<int> wayland_windowing::get_buttons() {
    std::vector<int> res;
    res.push_back(2);
    return res;
}
//} class wayland_windowing : control_impl

#else /* If this build completed whithout X11 support */

    //class wayland_windowing : control_impl {
    wayland_windowing::~wayland_windowing()                     {}
    int wayland_windowing::status()                         {return -101;}
    int wayland_windowing::button(int keysym, int pressing) {return -1;}
    std::vector<int> wayland_windowing::get_buttons()       {return {};}
    //} class wayland_windowing : control_impl

#endif

}//cirno
