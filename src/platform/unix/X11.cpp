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

    typedef struct {
        int work_flag;
        Display *lclDisplay, *recDisplay;
        XRecordContext context;
        s_event_decl events_decl;
    } s_XHeap;

    void eventCallback(XPointer xheap, XRecordInterceptData *data) {
        s_XHeap *heap=(s_XHeap *) xheap;
        unsigned int type, button;
        s_event_decl *events = &heap->events_decl;

        if (data->category != XRecordFromServer || heap->work_flag == 0) {
            XRecordFreeData(data);
            return;
        }
        type=(static_cast<unsigned char *>(data->data))[0];
        button=(static_cast<unsigned char *>(data->data))[1];

        switch (type) {
            case ButtonPress:
                for (unsigned int i=0; i<events->count; i++) {
                    if (button == events->ev_button[i]) {
                        events->flag[i] = true;
                    }
                }
                break;

            case ButtonRelease:
                for (unsigned int i=0; i<events->count; i++) {
                    if (button == events->ev_button[i]) {
                        events->flag[i] = false;
                    }
                }
                break;
            
            default:
                break;
        }
        XRecordFreeData(data);
    }

/*********************[  class x11_windowing : control_impl {  ]**********************/
    x11_windowing::~x11_windowing() {
        if (x11_windowing::lclDisplay) {
            // XCloseDisplay(x11_windowing::lclDisplay); // Double free (?)
            // XCloseDisplay(x11_windowing::recDisplay);
        }
    }

    void x11_windowing::init() {
        x11_windowing::lclDisplay = XOpenDisplay(0); //Take out!
        x11_windowing::recDisplay = XOpenDisplay(0);
        if ((x11_windowing::lclDisplay == NULL)||(x11_windowing::recDisplay == NULL)) {
            throw excepts::error("Display is null", "Xorg.cpp");
        };
        int unnecessary;
        if (!XRecordQueryVersion(recDisplay, &unnecessary, &unnecessary)) {
            throw excepts::error("XRecord extension is not found!", "Xorg.cpp");
        }
        x11_windowing::lclScreen = DefaultScreen(x11_windowing::lclDisplay);
        x11_windowing::rootWindow = RootWindow(x11_windowing::lclDisplay, x11_windowing::lclScreen);
    }

    void x11_windowing::action_button(int keysym, bool pressing) {
        XTestFakeButtonEvent(x11_windowing::lclDisplay, keysym, pressing, CurrentTime);
        XFlush(x11_windowing::lclDisplay);
    }

    void x11_windowing::handle_events(struct s_event_decl *events_decl) {
        XRecordContext context;
        XRecordRange *allocRange;
        XRecordClientSpec clientSpec;
        s_XHeap xheap;

        allocRange = XRecordAllocRange();
        if (!allocRange) {
            throw excepts::error("Failed to call XRecordAllocRange()", "Xorg.cpp");
        }

        allocRange->device_events.first=KeyPress;
        allocRange->device_events.last=MotionNotify;
        clientSpec=XRecordAllClients;
        context=XRecordCreateContext(x11_windowing::recDisplay, 0, &clientSpec, 1, &allocRange, 1);
        if (!context) {
            throw excepts::error("Failed to get XRecord context", "Xorg.cpp");
        }

        xheap.work_flag      = true;
        xheap.lclDisplay     = x11_windowing::lclDisplay;
        xheap.recDisplay     = x11_windowing::recDisplay;
        xheap.context        = context;
        xheap.events_decl    = *events_decl;
        
        if (!XRecordEnableContextAsync(recDisplay, context, eventCallback, (XPointer)&xheap)) {
            throw excepts::error("Failed to start async eventCallback()", "Xorg.cpp");
        }

        while (xheap.work_flag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(c_actions_cooldown));
            XRecordProcessReplies(recDisplay);
        }

        XRecordDisableContext(lclDisplay, context);
        XRecordFreeContext(lclDisplay, context);
        XFree(allocRange);
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
/*********************[ }; //class x11_windowing : control_impl ]*********************/

#endif

} // namespace platform
