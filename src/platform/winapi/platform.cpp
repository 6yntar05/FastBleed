#include "platform.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include "../../ui/feedback.hpp"

namespace cirno {
// Pattern "FACTORY" ^.^
std::shared_ptr<windows_windowing> user_windowing::make_api() {
    return std::make_shared<windows_windowing>();
};

std::shared_ptr<windows_windowing> get_platform() {
    user_windowing windowing;
    return windowing.make_api();
}

}//namespace cirno