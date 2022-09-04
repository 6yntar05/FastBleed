#include "../properties.hpp"
#include <iostream>
#include <string>

#ifdef USE_COLORS
    #define CLR "\033[0m"
    #define RED "\033[31m"
    #define YEL "\033[33m"
    #define BOLD "\033[1m"
#else
    #define CLR ""
    #define RED ""
    #define YEL ""
    #define BOLD ""
#endif

namespace cirno {
    void error(std::string msg) {
        std::cerr << "["<<RED<<BOLD<<"ERROR"<<CLR<<"] "
            << BOLD<<msg<<CLR 
            << std::endl;
    }
    void warn(std::string msg) {
        std::cerr << "["<<YEL<<BOLD<<"WARN!"<<CLR<<"] "
            << BOLD<<msg<<CLR
            << std::endl;
    }
    void general_state(bool state) {
        std::cout << "[INFO_] Starting action handler.\n";
    }
}