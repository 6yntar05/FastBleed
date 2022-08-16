#include <vector>
#include "X11.hpp"
#include "../../properties.hpp"

#ifdef USE_X11
    #include <X11/Xlib.h>
    #include <X11/extensions/XTest.h>
#endif

namespace cirno {

#ifdef USE_X11

//class x11_windowing : control_impl {
x11_windowing::~x11_windowing() {
    XCloseDisplay(x11_windowing::display);
}

int x11_windowing::init() {
    x11_windowing::display = XOpenDisplay(NULL); //Take out!
    if (x11_windowing::display == NULL){
        return -1;
    };
    return 0;
}

int x11_windowing::button(int keysym, int pressing) {
    XTestFakeButtonEvent(x11_windowing::display, keysym, pressing, CurrentTime);
    XFlush(x11_windowing::display);
    return 0;
}

std::vector<int> x11_windowing::get_buttons() {
    std::vector<int> res;
    res.push_back(2);
    return res;
}
//} class x11_windowing : control_impl

#else /* If this build completed whithout X11 support */

    //class x11_windowing : control_impl {
    x11_windowing::~x11_windowing()                     {}
    int x11_windowing::init()                         {return -102;}
    int x11_windowing::button(int keysym, int pressing) {return -1;}
    std::vector<int> x11_windowing::get_buttons()       {return {};}
    //} class x11_windowing : control_impl

#endif

}//cirno
