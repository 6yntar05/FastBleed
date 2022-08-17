#include "../../properties.hpp"
#include <vector>
#include "Wayland.hpp"

#ifdef USE_WAYLAND
    // Wayland libs
#endif

namespace cirno {

#ifdef USE_WAYLAND

//class wayland_windowing : control_impl {
wayland_windowing::~wayland_windowing() {}
int wayland_windowing::init()                           {return -101;}
int wayland_windowing::button(int keysym, int pressing) {return -1;}
std::vector<int> wayland_windowing::get_buttons()       {return {};}
//} class wayland_windowing : control_impl

#else /* If this build completed whithout Wayland support */

    //class wayland_windowing : control_impl {
    wayland_windowing::~wayland_windowing()                 {}
    int wayland_windowing::init()                           {return -101;}
    int wayland_windowing::button(int keysym, int pressing) {return -1;}
    std::vector<int> wayland_windowing::get_buttons()       {return {};}
    //} class wayland_windowing : control_impl

#endif

}//cirno
