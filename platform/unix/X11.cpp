#include <iostream>
#include <vector>
#include <string.h>    //depr.
#include <chrono>
#include <thread>
#include <X11/Xlib.h>
#include <X11/Xutil.h> // XGetPixel and XDestroyImage
#include <X11/extensions/XTest.h>

namespace cirno {
class x11_windowing : public control_impl {
private:
    Display *display;

public:
    ~x11_windowing() {
        XCloseDisplay(display);
    }

    int status() {
        display = XOpenDisplay(NULL); //Take out!
        if (display == NULL){
            return -1;
        };
        return 0;
    }

    int button(int keysym, int pressing) override {
        XTestFakeButtonEvent(display, keysym, pressing, CurrentTime);
        XFlush(display);
        return 0;
    }

    std::vector<int> get_buttons() override {
        std::vector<int> res;
        res.push_back(2);
        return res;
    }
};
}//cirno
