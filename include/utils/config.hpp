#pragma once

#include <string>
#include "runtime.hpp"

namespace utils {

class c_config {
private:
    std::string PATH;
    s_event_decl ret;
    bool allocated = false;
    e_actions to_action(std::string str);

public:
    c_config(std::string PATH);
    ~c_config();

    void allocate(unsigned int count);
    void deallocate();

    s_event_decl parse();
    void sync();
};

} // namespace utils