#include <vector>

struct s_event_decl {
    unsigned int count;
    unsigned int *button;
    bool *flag;
};


namespace cirno {
class control_impl {
public:
    virtual ~control_impl()                                 = default;
    virtual int init()                                      = 0;
    virtual int button(int keysym, int pressing)            = 0;
    virtual int handle_events(struct s_event_decl *events_decl)= 0;
};

class wayland_windowing : public control_impl {
public:
    ~wayland_windowing();
    int init();
    int action_button(int keysym, bool pressing);
    int handle_events(struct s_event_decl *events_decl);
};
}