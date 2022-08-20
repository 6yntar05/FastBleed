#include "../../properties.hpp"
#include <vector>
#include "Wayland.hpp"

#ifdef USE_WAYLAND
    // Wayland libs
#endif

namespace cirno {

#ifdef USE_WAYLAND

    //class wayland_windowing : control_impl {
    wayland_windowing::~wayland_windowing()                             {}
    int wayland_windowing::init()                                       {return -101;}
    int wayland_windowing::action_button(int keysym, bool pressing)     {return -1;}
    int wayland_windowing::handle_button(int keysym, bool intercept)    {return -1;}
    //} class wayland_windowing : control_impl

#else /* If this build completed whithout Wayland support */

    //class wayland_windowing : control_impl {
    wayland_windowing::~wayland_windowing()                             {}
    int wayland_windowing::init()                                       {return -101;}
    int wayland_windowing::action_button(int keysym, bool pressing)     {return -1;}
    int wayland_windowing::handle_button(int keysym, bool intercept)    {return -1;}
    //} class wayland_windowing : control_impl

#endif

}//cirno
