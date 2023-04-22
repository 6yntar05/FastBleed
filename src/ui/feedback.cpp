#include <iostream>
#include "ui/feedback.hpp"

namespace ui {

    void error(std::string msg) {
        std::cerr << "[" <<RED<<BOLD<<"ERROR"<<CLR<< "] "
            << BOLD<<msg<<CLR 
            << std::endl;
    }

    void warn(std::string msg) {
        std::cerr << "[" <<YEL<<BOLD<<"WARN!"<<CLR<< "] "
            << BOLD<<msg<<CLR
            << std::endl;
    }

    void info(std::string msg) {
        std::cout << "[INFO_] " << msg << std::endl;
    }

    void msg(std::string msg) {
        std::cout << msg << std::endl;
    }

}
