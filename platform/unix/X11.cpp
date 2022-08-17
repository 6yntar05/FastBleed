#include "../../properties.hpp"     // Keep this import first
#include <vector>
#include <cstdint>
#include <iostream>
#include "X11.hpp"

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
    Window window = XDefaultRootWindow(display);
    Window curFocus;
    KeySym ks;
    XEvent KeyEvent;

    int len;
    int revert;
    while (true) {
        //while(XPending(display)) { //Repeats until all events are computed
            std::cout << ">Pending\n";
            XGetInputFocus (display, &curFocus, &revert);
            //XSelectInput(display, curFocus, KeyPressMask|KeyReleaseMask);
            XTestGrabControl(display, 0);
            XNextEvent(display, &KeyEvent); //Gets exactly one event
            //XGrabButton(display, 1, 2048, window, 1, KeyPressMask|KeyReleaseMask, 0, 0, 0, 0);
            std::cout << "<Pending\n";
            if(KeyEvent.type==KeyPress) {
                uint32_t KeyEventCode=KeyEvent.xkey.keycode; //Gets the key code, NOT HIS CHAR EQUIVALENT
                std::cout << KeyEventCode << '\n'; //Displays the key code

                /* Code handling a Keypress event */

            } else if(KeyEvent.type==KeyRelease) {
                uint32_t KeyEventCode=KeyEvent.xkey.keycode;
                std::cout << KeyEventCode << '\n'; //Displays the key code

                /* Code handling a KeyRelease event */

            } else {
                std::cout << "KeyEvent.type is " << KeyEvent.type << std::endl;
            }
        //}
    }

    res.push_back(2);
    return res;
}
//} class x11_windowing : control_impl

#else /* If this build completed whithout X11 support */

    //class x11_windowing : control_impl {
    x11_windowing::~x11_windowing()                     {}
    int x11_windowing::init()                           {return -102;}
    int x11_windowing::button(int keysym, int pressing) {return -1;}
    std::vector<int> x11_windowing::get_buttons()       {return {};}
    //} class x11_windowing : control_impl

#endif

}//cirno
