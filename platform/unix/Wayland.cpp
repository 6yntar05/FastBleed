#include <iostream>
#include <vector>
#include <libinput.h>

namespace cirno {
class wayland_windowing : public control_impl {
public:
    int status() {return 0;}

    int button(int keysym, int pressing) override {
        // Not implemented for now
        return -1;
    }

    std::vector<int> get_buttons() override {
        // Not implemented for now
        return {};
    }
};
}