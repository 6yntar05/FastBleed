#include "../../properties.hpp"
#include <vector>
#include <cstdint>
#include <cmath>
#include <csignal>
#include <chrono>
#include <random>
#include <thread>
#include <iostream>
#include <functional>
#include "X11.hpp"
#include "../../ui/event.hpp"

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
        int Status1, Status2, x, y, mmoved, doit;
        Display *lclDisplay, *recDisplay;
        XRecordContext rc;
        struct s_event_decl events_decl;
    } Priv;

    void eventCallback(XPointer priv, XRecordInterceptData *data) {
        Priv *p=(Priv *) priv;

        unsigned int *ud4, tstamp, wroot, wevent, wchild, type, button;
        unsigned char *ud1;
        unsigned short *ud2, seq;
        short *d2, rootx, rooty, eventx, eventy, kstate;

        struct s_event_decl events = p->events_decl;

        if (data->category != XRecordFromServer || p->doit == 0) {
            XRecordFreeData(data);
            return;
        }

        ud1=(unsigned char *)data->data;
        ud2=(unsigned short *)data->data;
        d2=(short *)data->data;
        ud4=(unsigned int *)data->data;

        type=ud1[0]&0x7F;
        button=ud1[1];

        switch (type) {
            case ButtonPress:
                for (unsigned int i=0; i<events.count; i++) {
                    if (button == events.ev_button[i]) {
                        if (p->mmoved) p->mmoved=0;
                        if (p->Status2<0) p->Status2=0;
                        p->Status2++;
                        events.flag[i] = true;
                    }
                }
                break;

            case ButtonRelease:
                for (unsigned int i=0; i<events.count; i++) {
                    if (button == events.ev_button[i]) {
                        if (p->mmoved) p->mmoved=0;
                        if (p->Status2<0) p->Status2=0;
                        p->Status2++;
                        events.flag[i] = false;
                    }
                }
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
        // Partly taken from alvatar/xmacro
        Window Root, rRoot, rChild;
        XRecordContext rc;
        XRecordRange *rr;
        XRecordClientSpec rcs;
        Priv priv;

        rr = XRecordAllocRange();
        if (!rr) {
            error("Failed to call XRecordAllocRange()");
            return -1;
        }

        rr->device_events.first=KeyPress;
        rr->device_events.last=MotionNotify;
        rcs=XRecordAllClients;
        rc=XRecordCreateContext(x11_windowing::recDisplay, 0, &rcs, 1, &rr, 1);
        if (!rc) {return -2;}

        priv.x              = 0;
        priv.y              = 0;
        priv.mmoved         = 1;
        priv.Status2        = 0;
        priv.Status1        = 2;
        priv.doit           = 1;
        priv.lclDisplay     = x11_windowing::lclDisplay;
        priv.recDisplay     = x11_windowing::recDisplay;
        priv.rc             = rc;
        priv.events_decl    = *events_decl;
        
        if (!XRecordEnableContextAsync(recDisplay, rc, eventCallback, (XPointer)&priv)) {
            return -3;
        }

        while (priv.doit) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            XRecordProcessReplies(recDisplay);
        }

        XRecordDisableContext(lclDisplay, rc);
        XRecordFreeContext(lclDisplay, rc);
        XFree(rr);

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
