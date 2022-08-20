#include "../../properties.hpp"
#include <vector>
#include <cstdint>
#include <cmath>
#include <csignal>
#include <chrono>
#include <random>
#include <thread>
#include <iostream>
#include <functional>
#include "X11.hpp"

#ifdef USE_X11
    #include <X11/X.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/extensions/XTest.h>
    #include <X11/extensions/XInput2.h>
#endif

namespace cirno {

const float Scale = 1.0;
int scale (const int Coordinate) {

  // perform the scaling, all in one ugly line
  return (int)( (float)Coordinate * Scale );
}

#ifdef USE_X11

//class x11_windowing : control_impl {
x11_windowing::~x11_windowing() {
    if (x11_windowing::display) {
        // XCloseDisplay(x11_windowing::display); // Double free (?)
    }
}

int x11_windowing::init() {
    x11_windowing::display = XOpenDisplay(NULL); //Take out!
    if (x11_windowing::display == NULL){
        return -1;
    };
    return 0;
}

int x11_windowing::action_button(int keysym, bool pressing) {
    XTestFakeButtonEvent(x11_windowing::display, keysym, pressing, CurrentTime);
    XFlush(x11_windowing::display);
    return 0;
}

int x11_windowing::handle_button(int keysym, bool intercept) {
    Display *display = x11_windowing::display;
    std::cerr << "intercept: " << intercept << "; keysym: " << keysym << std::endl;
    return 0;
}
//} class x11_windowing : control_impl

#else /* If this build completed whithout X11 support */

    //class x11_windowing : control_impl {
    x11_windowing::~x11_windowing()                                 {}
    int x11_windowing::init()                                       {return -102;}
    int x11_windowing::action_button(int keysym, bool pressing)     {return -1;}
    int x11_windowing::handle_button(int keysym, bool intercept)    {return -1;}
    //} class x11_windowing : control_impl

#endif

}//cirno
