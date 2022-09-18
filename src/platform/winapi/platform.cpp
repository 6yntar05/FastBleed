#include "platform.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "../../ui/feedback.hpp"

namespace cirno {

std::shared_ptr<control_impl> user_windowing::make_api() {
    return std::make_shared<control_impl>();
};

std::shared_ptr<control_impl> get_platform() {
    user_windowing windowing;
    return windowing.make_api();
}

}//namespace cirno