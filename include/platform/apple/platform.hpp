#include <iostream>
#include <vector>
#include <memory>

struct s_event_decl;

namespace cirno {

class control_impl {
public:
    ~control_impl();
    int init();
    int action_button(int keysym, bool pressing);
    int handle_events(struct s_event_decl *events_decl);
};

class user_windowing {
public:
    std::shared_ptr<control_impl> make_api();
};

std::shared_ptr<control_impl> get_platform();

}//namespace cirno