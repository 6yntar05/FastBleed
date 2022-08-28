#include "../../properties.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include "X11.hpp"
#include "../../ui/feedback.hpp"

#ifdef USE_X11
    #include <X11/X.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/cursorfont.h>
    #include <X11/keysymdef.h>
    #include <X11/keysym.h>
    #include <X11/extensions/record.h>
#endif

namespace cirno {

#ifdef USE_X11

    typedef struct{
        int work_flag;
        Display *lclDisplay, *recDisplay;
        XRecordContext context;
        struct s_event_decl events_decl;
    } s_XHeap;

    void eventCallback(XPointer xheap, XRecordInterceptData *data) {
        s_XHeap *heap=(s_XHeap *) xheap;
        unsigned int type, button;
        struct s_event_decl events = heap->events_decl;

        if (data->category != XRecordFromServer || heap->work_flag == 0) {
            XRecordFreeData(data);
            return;
        }
        type=(static_cast<unsigned char *>(data->data))[0];
        button=(static_cast<unsigned char *>(data->data))[1];

        switch (type) {
            case ButtonPress:
                for (unsigned int i=0; i<events.count; i++) {
                    if (button == events.ev_button[i]) {
                        events.flag[i] = true;
                    }
                }
                break;

            case ButtonRelease:
                for (unsigned int i=0; i<events.count; i++) {
                    if (button == events.ev_button[i]) {
                        events.flag[i] = false;
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

    int x11_windowing::init() {
        x11_windowing::lclDisplay = XOpenDisplay(0); //Take out!
        x11_windowing::recDisplay = XOpenDisplay(0);
        if ((x11_windowing::lclDisplay == NULL)||(x11_windowing::recDisplay == NULL)) {
            error("Unnable to open X Display!");
            return -1;
        };
        int unnecessary;
        if (!XRecordQueryVersion(recDisplay, &unnecessary, &unnecessary)) {
            error("XRecord extension is not found!");
            return -1;
        }
        x11_windowing::lclScreen = DefaultScreen(x11_windowing::lclDisplay);
        x11_windowing::rootWindow = RootWindow(x11_windowing::lclDisplay, x11_windowing::lclScreen);
        return 0;
    }

    int x11_windowing::action_button(int keysym, bool pressing) {
        XTestFakeButtonEvent(x11_windowing::lclDisplay, keysym, pressing, CurrentTime);
        XFlush(x11_windowing::lclDisplay);
        return 0;
    }

    int x11_windowing::handle_events(struct s_event_decl *events_decl) {
        XRecordContext context;
        XRecordRange *allocRange;
        XRecordClientSpec clientSpec;
        s_XHeap xheap;

        allocRange = XRecordAllocRange();
        if (!allocRange) {
            error("Failed to call XRecordAllocRange()");
            return -1;
        }

        allocRange->device_events.first=KeyPress;
        allocRange->device_events.last=MotionNotify;
        clientSpec=XRecordAllClients;
        context=XRecordCreateContext(x11_windowing::recDisplay, 0, &clientSpec, 1, &allocRange, 1);
        if (!context) {
            error("Failed to get XRecord context");
            return -2;
        }

        xheap.work_flag      = true;
        xheap.lclDisplay     = x11_windowing::lclDisplay;
        xheap.recDisplay     = x11_windowing::recDisplay;
        xheap.context        = context;
        xheap.events_decl    = *events_decl;
        
        if (!XRecordEnableContextAsync(recDisplay, context, eventCallback, (XPointer)&xheap)) {
            error("Failed to start async eventCallback()");
            return -3;
        }

        while (xheap.work_flag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(c_actions_cooldown));
            XRecordProcessReplies(recDisplay);
        }

        XRecordDisableContext(lclDisplay, context);
        XRecordFreeContext(lclDisplay, context);
        XFree(allocRange);

        return 0;
    }
/*********************[ }; //class x11_windowing : control_impl ]*********************/

#else /* If this build completed whithout X11 support */

/*********************[  class x11_windowing : control_impl {  ]**********************/
    x11_windowing::~x11_windowing()                                     {}
    int x11_windowing::init()                                           {return -202;}
    int x11_windowing::action_button(int keysym, bool pressing)         {return -1;}
    int x11_windowing::handle_events(struct s_event_decl *events_decl)  {return -1;}
/*********************[ }; //class x11_windowing : control_impl ]*********************/

#endif

}//cirno
