#include <iostream>
#include <string>

namespace cirno {
    void error(std::string msg){
        // if cli:
            std::cerr << "[EROOR] " << msg << std::endl;
        // if gui:
            // Not implemented
    }

}