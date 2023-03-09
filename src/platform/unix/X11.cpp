#include "properties.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include "platform/unix/platform.hpp"
#include "ui/feedback.hpp"
#include "runtime.hpp"
#include "excepts.hpp"

#ifdef USE_X11
    #include <X11/X.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/cursorfont.h>
    #include <X11/keysymdef.h>
    #include <X11/keysym.h>
    #include <X11/extensions/record.h>
#endif

namespace platform {

#ifdef USE_X11

    void x11_windowing::eventCallback(XPointer xheap, XRecordInterceptData *xdata) {
        s_XHeap *heap = reinterpret_cast<s_XHeap*>(xheap);

        if (xdata->category != XRecordFromServer) {
            XRecordFreeData(xdata);
            return;
        }

        unsigned int type   = static_cast<unsigned char *>(xdata->data)[0];
        unsigned int button = static_cast<unsigned char *>(xdata->data)[1];

        if (type != MotionNotify) { // discard at once (temporary)
            
            for (unsigned int i = 0; i < heap -> events_decl->size(); i++) {
                if (
                    (heap -> events_decl->at(i)->was_mouse && ((type == ButtonPress) || (type == ButtonRelease))) ||
                    (!heap -> events_decl->at(i)->was_mouse && ((type == KeyPress) || (type == KeyRelease)))
                ) {
                    if (button == heap -> events_decl->at(i)->ev_button) {
                        heap -> events_decl->at(i)->set_active(
                            (type == ButtonPress) || (type == KeyPress) ? true : false
                        );
                    }
                }
            }
        }

        XRecordFreeData(xdata);
    }

/*********************[  class x11_windowing : control_impl {  ]**********************/
    x11_windowing::~x11_windowing() {
        if (this->lclDisplay) {
            XCloseDisplay(this->lclDisplay);
            XCloseDisplay(this->recDisplay);
        }
    }

    void x11_windowing::init() {
        this->lclDisplay = XOpenDisplay(0); //Take out!
        this->recDisplay = XOpenDisplay(0);

        if ((this->lclDisplay == NULL) || (this->recDisplay == NULL))
            throw excepts::error("Display is null", "Xorg.cpp");

        if (int ver; !XRecordQueryVersion(recDisplay, &ver, &ver))
            throw excepts::error("XRecord extension is not found!", "Xorg.cpp");

        this->lclScreen = DefaultScreen(this->lclDisplay);
        this->rootWindow = RootWindow(this->lclDisplay, this->lclScreen);
    }

    void x11_windowing::handle_events(s_event_decl *events_decl) {
        XRecordContext context;
        XRecordRange *allocRange;
        XRecordClientSpec clientSpec;
        s_XHeap xheap;

        allocRange = XRecordAllocRange();
        if (!allocRange)
            throw excepts::error("Failed to call XRecordAllocRange()", "Xorg.cpp");

        allocRange->device_events.first = KeyPress;
        allocRange->device_events.last = MotionNotify;
        clientSpec = XRecordAllClients;
        context = XRecordCreateContext(this->recDisplay, 0, &clientSpec, 1, &allocRange, 1);
        if (!context)
            throw excepts::error("Failed to get XRecord context", "Xorg.cpp");

        xheap.lclDisplay     = this->lclDisplay;
        xheap.recDisplay     = this->recDisplay;
        xheap.context        = context;
        xheap.events_decl    = events_decl;
        
        if (!XRecordEnableContext(recDisplay, context, this->eventCallback, reinterpret_cast<XPointer>(&xheap)))
            throw excepts::error("Failed to start async eventCallback()", "Xorg.cpp");

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(c_actions_cooldown));
            XRecordProcessReplies(recDisplay);
        }

        XRecordDisableContext(lclDisplay, context);
        XRecordFreeContext(lclDisplay, context);
        XFree(allocRange);
    }

    void x11_windowing::action_button(int keysym, bool pressing) {
        XTestFakeButtonEvent(this->lclDisplay, keysym, pressing, CurrentTime);
        XFlush(this->lclDisplay);
    }

    void x11_windowing::exec(std::string command) {
        system(command.c_str());
    }

/*********************[ }; //class x11_windowing : control_impl ]*********************/

#else /* If this build completed whithout X11 support */

/*********************[  class x11_windowing : control_impl {  ]**********************/
    x11_windowing::~x11_windowing() {}
    
    void x11_windowing::init() {
        throw excepts::error("This build completed without X11 support");
    }

    void x11_windowing::action_button(int keysym, bool pressing) {
        throw excepts::error("This build completed without X11 support");
    }

    void x11_windowing::handle_events(struct s_event_decl *events_decl) {
        throw excepts::error("This build completed without X11 support");
    }

    void x11_windowing::exec(std::string command) {
        system(command.c_str());
    }
/*********************[ }; //class x11_windowing : control_impl ]*********************/

#endif

} // namespace platform
