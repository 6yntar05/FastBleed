#include <iostream>
#include <string>

namespace cirno {
    void raise(std::string msg){
        // if cli:
        std::cerr << msg << std::endl;
        // if gui:
        //      empty
    }

}