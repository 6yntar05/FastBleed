#include <vector>

namespace cirno {
class control_impl {
public:
    virtual ~control_impl()                         = default;
    virtual int init()                              = 0;
    virtual int button(int keysym, int pressing)    = 0;
    virtual std::vector<int> get_buttons()          = 0;
};

class wayland_windowing : public control_impl {
public:
    ~wayland_windowing();
    int init();
    int button(int keysym, int pressing);
    std::vector<int> get_buttons();
};
}