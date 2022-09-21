#pragma once

#include <string>
#include <string_view>
#include <vector>

enum e_actions {
    ACT_DELAY,
    ACT_CLICK,
    ACT_RELEASE,
    ACT_MOVE,
    ACT_CLICKER,
    ACT_TEXT_TYPE,
    ACT_SYS_EXEC,
    ACT_NULL
};

struct s_event_decl {
    unsigned int count;
    unsigned int *ev_button;
    bool *flag;
    std::vector<e_actions> *action;
    std::vector<unsigned int> *action_param;
};

namespace cirno {

e_actions to_action(std::string str);

class c_config {
private:
    std::string PATH;
    s_event_decl ret;
    bool allocated = false;

public:
    c_config(std::string PATH);
    ~c_config();

    void allocate(unsigned int count);
    void deallocate();

    s_event_decl parse();
    void sync();
};

} // namespace cirno