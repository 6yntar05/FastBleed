#include <cstdlib>
#include <memory>
#include <functional>
#include "../../ui/event.h"     // cirno::raise()
#include "../../properties.h"   // defined USE_X11, USE_WAYLAND

namespace cirno {
class control_impl {
public:
    int status() {
        std::cerr << "Windows\n";
        return 0;
    }
    int button(int keysym, int pressing) {return -411;}
    std::vector<int> get_buttons() {return {};}
};

class user_windowing {
public:
    static std::shared_ptr<control_impl> make_api(void) {
        return std::make_shared<control_impl>();
    }
};


std::shared_ptr<control_impl> init() {
    user_windowing bebra;
    return bebra.make_api();
};

}//namespace cirno