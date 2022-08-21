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

extern bool glob_a_enabled;
extern bool glob_b_enabled;

namespace cirno {

#ifdef USE_X11

typedef struct{
	int Status1, Status2, x, y, mmoved, doit;
	Display *lclDisplay, *recDisplay;
	XRecordContext rc;
} Priv;

inline void eventCallback(XPointer priv, XRecordInterceptData *d) {
    Priv *p=(Priv *) priv;
    unsigned int *ud4, tstamp, wroot, wevent, wchild, type, detail;
    unsigned char *ud1, type1, detail1, samescreen;
    unsigned short *ud2, seq;
    short *d2, rootx, rooty, eventx, eventy, kstate;

    if (d->category != XRecordFromServer || p->doit == 0) {
        XRecordFreeData(d);
        return;
    }

    ud1=(unsigned char *)d->data;
    ud2=(unsigned short *)d->data;
    d2=(short *)d->data;
    ud4=(unsigned int *)d->data;

    type1=ud1[0]&0x7F; type=type1;
    detail1=ud1[1]; detail=detail1;
    seq=ud2[1];
    tstamp=ud4[1];
    wroot=ud4[2];
    wevent=ud4[3];
    wchild=ud4[4];
    rootx=d2[10];
    rooty=d2[11];
    eventx=d2[12];
    eventy=d2[13];
    kstate=d2[14];
    samescreen=ud1[30];

    if (p->Status1) {
        p->Status1--;
        if (type==KeyRelease) {
            std::cerr << "- Skipping stale KeyRelease event. " << p->Status1 << std::endl;
            XRecordFreeData(d);
            return;
        } else {
            p->Status1=0;
        }
    }
    if (p->x==-1 && p->y==-1 && p->mmoved==0 && type!=MotionNotify) {
        std::cerr << "- Please move the mouse before any other event to synchronize pointer" << std::endl;
        std::cerr << "  coordinates! This event is now ignored!" << std::endl;
        XRecordFreeData(d);
        return;
    }

    switch(type) {
        case ButtonPress:
            if (detail == 9) {
                if (p->mmoved) {
                    std::cerr << "+MotionNotify: " << p->x << ":" << p->y << std::endl;
                    p->mmoved=0;
                }
                if (p->Status2<0) p->Status2=0;
                p->Status2++;
                std::cerr << "ButtonPress: " << detail << std::endl;
                glob_a_enabled = true;
            } else if (detail == 8) {
                if (p->mmoved) {
                    std::cerr << "+MotionNotify: " << p->x << ":" << p->y << std::endl;
                    p->mmoved=0;
                }
                if (p->Status2<0) p->Status2=0;
                p->Status2++;
                std::cerr << "ButtonPress: " << detail << std::endl;
                glob_b_enabled = true;
            }
            break;

        case ButtonRelease:
            if (detail == 9) {
                if (p->mmoved) {
                    std::cerr << "+MotionNotify: " << p->x << ":" << p->y << std::endl;
                    p->mmoved=0;
                }
                if (p->Status2<0) p->Status2=0;
                p->Status2--;
                std::cerr << "ButtonRelease: " << detail << std::endl;
                glob_a_enabled = false;
            } else if (detail == 8) {
                if (p->mmoved) {
                    std::cerr << "+MotionNotify: " << p->x << ":" << p->y << std::endl;
                    p->mmoved=0;
                }
                if (p->Status2<0) p->Status2=0;
                p->Status2--;
                std::cerr << "ButtonRelease: " << detail << std::endl;
                glob_b_enabled = false;
            }
            break;
    }
}

//class x11_windowing : control_impl {
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

int x11_windowing::handle_events(int keysym, bool intercept) {
    // Taken from alvatar/xmacro xmacrorec2
    Window       Root, rRoot, rChild;
    XRecordContext rc;
    XRecordRange *rr;
    XRecordClientSpec rcs;
    Priv priv;
    int rootx=0, rooty=0, winx=0, winy=0;
    unsigned int mmask;
    int sret;
    //if (!XQueryPointer(x11_windowing::lclDisplay, x11_windowing::rootWindow,
    //                &rRoot, &rChild, &rootx, &rooty, &winx, &winy, &mmask)) {
    //    return -1;
    //}
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

    priv.x=rootx;
    priv.y=rooty;
    priv.mmoved=1;
    priv.Status2=0;
    priv.Status1=2;
    priv.doit=1;
    priv.lclDisplay=x11_windowing::lclDisplay;
    priv.recDisplay=x11_windowing::recDisplay;
    priv.rc=rc;
    
    if (!XRecordEnableContextAsync(recDisplay, rc, eventCallback, (XPointer)&priv)) {
        return -3;
    }
    // Start thread

    while (priv.doit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        XRecordProcessReplies(recDisplay);
    }

    sret=XRecordDisableContext(lclDisplay, rc);
    if (!sret) return -4;
    sret=XRecordFreeContext(lclDisplay, rc);
    if (!sret) return -5;
    XFree(rr);

    return 0;
}
//} class x11_windowing : control_impl

#else /* If this build completed whithout X11 support */

    //class x11_windowing : control_impl {
    x11_windowing::~x11_windowing()                                 {}
    int x11_windowing::init()                                       {return -102;}
    int x11_windowing::action_button(int keysym, bool pressing)     {return -1;}
    int x11_windowing::handle_events(int keysym, bool intercept)    {return -1;}
    //} class x11_windowing : control_impl

#endif

}//cirno
