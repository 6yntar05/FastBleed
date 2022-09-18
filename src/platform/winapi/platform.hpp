#include <iostream>
#include <vector>
#include <memory>

struct s_event_decl;

namespace cirno {

class windows_windowing {
public:
    ~windows_windowing();
    int init();
    int action_button(int keysym, bool pressing);
    int handle_events(struct s_event_decl *events_decl);
};

class user_windowing {
public:
    std::shared_ptr<windows_windowing> make_api();
};

std::shared_ptr<windows_windowing> get_platform();

}//namespace cirno