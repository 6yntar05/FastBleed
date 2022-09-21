#include "platform/winapi/platform.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "ui/feedback.hpp"

namespace cirno {

/*********************[  class control_impl {  ]**********************/

control_impl::~control_impl() {
    
}

int control_impl::init() {
    return 0;
}

int control_impl::action_button(int keysym, bool pressing) {
    return 0;
}

int control_impl::handle_events(struct s_event_decl *events_decl) {
    return 0;
}

/*********************[  } class control_impl  ]**********************/

std::shared_ptr<control_impl> get_platform() {
    return std::make_shared<control_impl>();
}

}//namespace cirno