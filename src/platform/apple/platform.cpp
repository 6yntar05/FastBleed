#include <iostream>
#include <vector>
#include <memory>
#include "platform/apple/platform.hpp"
#include "ui/feedback.hpp"
#include "runtime.hpp"
#include <excepts.hpp>

namespace platform {

/*********************[  class control_impl {  ]**********************/

control_impl::~control_impl() {}

void control_impl::init() {
    throw excepts::error("Not implemented", "platform.cpp");
}

void control_impl::action_button(int keysym, bool pressing) {
    throw excepts::error("Not implemented", "platform.cpp");
}

void control_impl::handle_events(s_event_decl *events_decl) {
    throw excepts::error("Not implemented", "platform.cpp");
}

/*********************[  } class control_impl  ]**********************/
/// Returns shared_ptr to implementation class
std::shared_ptr<control_impl> get_platform() {
    return std::make_shared<control_impl>();
}

} // namespace platform
