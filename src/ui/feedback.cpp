#include "ui/feedback.hpp"
#include <iostream>

namespace ui {

void error(std::string_view msg) {
    std::cerr << "[" <<RED<<BOLD<<"ERROR"<<CLR<< "] "
        << BOLD<<msg<<CLR 
        << std::endl;
}

void warn(std::string_view msg) {
    std::cerr << "[" <<YEL<<BOLD<<"WARN!"<<CLR<< "] "
        << BOLD<<msg<<CLR
        << std::endl;
}

void info(std::string_view msg) {
    std::cout << "[INFO_] " << msg << std::endl;
}

void msg(std::string_view msg) {
    std::cout << msg << std::endl;
}

} // namespace ui
