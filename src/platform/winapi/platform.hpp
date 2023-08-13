#include <vector>
#include <memory>
#include "runtime.hpp"

namespace platform {

class control_impl {
public:
    ~control_impl();
    void init();
    void action_button(int keysym, bool pressing);
    void handle_events(s_event_decl *events_decl);
};

class user_control {
public:
    std::shared_ptr<control_impl> make_api();
};

std::shared_ptr<control_impl> get_platform();

} // namespace platform