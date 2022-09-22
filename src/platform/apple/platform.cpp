#include <iostream>
#include <vector>
#include <memory>
#include "platform/apple/platform.hpp"
#include "ui/feedback.hpp"
#include "runtime.hpp"

namespace platform {

/*********************[  class control_impl {  ]**********************/

control_impl::~control_impl() {
    
}

int control_impl::init() {
    return 0;
}

void control_impl::action_button(int keysym, bool pressing) {

}

void control_impl::handle_events(s_event_decl *events_decl) {
    
}

/*********************[  } class control_impl  ]**********************/
/// Returns shared_ptr to implementation class
std::shared_ptr<control_impl> get_platform() {
    return std::make_shared<control_impl>();
}

} // namespace platform
