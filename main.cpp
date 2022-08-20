#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <ctype.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#ifdef HAVE_XI2
#include <X11/extensions/XInput2.h>
#endif

#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

extern int xi_opcode; /* xinput extension op code */

XDeviceInfo* find_device_info( Display *display, char *name, Bool only_extended);

#if HAVE_XI2
XIDeviceInfo* xi2_find_device_info(Display *display, char *name);
int xinput_version(Display* display);
#endif


using namespace std;

int xi_opcode;

#define INVALID_EVENT_TYPE -1

static int motion_type = INVALID_EVENT_TYPE;
static int button_press_type = INVALID_EVENT_TYPE;
static int button_release_type = INVALID_EVENT_TYPE;
static int key_press_type = INVALID_EVENT_TYPE;
static int key_release_type = INVALID_EVENT_TYPE;
static int proximity_in_type = INVALID_EVENT_TYPE;
static int proximity_out_type = INVALID_EVENT_TYPE;

static int register_events(Display *dpy,
                           XDeviceInfo *info,
                           char *dev_name,
                           Bool handle_proximity)
{
    int            number = 0;    /* number of events registered */
    XEventClass        event_list[7];
    int            i;
    XDevice        *device;
    Window        root_win;
    unsigned long    screen;
    XInputClassInfo    *ip;

    screen = DefaultScreen(dpy);
    root_win = RootWindow(dpy, screen);

    device = XOpenDevice(dpy, info->id);

    if (!device) {
    printf("unable to open device '%s'\n", dev_name);
    return 0;
    }

    if (device->num_classes > 0) {
    for (ip = device->classes, i=0; i<info->num_classes; ip++, i++) {
        switch (ip->input_class) {
        case KeyClass:
        DeviceKeyPress(device, key_press_type, event_list[number]); number++;
        DeviceKeyRelease(device, key_release_type, event_list[number]); number++;
        break;

        case ButtonClass:
        DeviceButtonPress(device, button_press_type, event_list[number]); number++;
        DeviceButtonRelease(device, button_release_type, event_list[number]); number++;
        break;

        case ValuatorClass:
        DeviceMotionNotify(device, motion_type, event_list[number]); number++;
        if (!handle_proximity) {
            ProximityIn(device, proximity_in_type, event_list[number]); number++;
            ProximityOut(device, proximity_out_type, event_list[number]); number++;
        }
        break;

        default:
        printf("unknown class\n");
        break;
        }
    }

    if (XSelectExtensionEvent(dpy, root_win, event_list, number)) {
        printf("error selecting extended events\n");
        return 0;
    }
    }
    return number;
}


static void print_events(Display *dpy)
{
    XEvent        Event;

    setvbuf(stdout, NULL, _IOLBF, 0);

    while(1) {
    XNextEvent(dpy, &Event);

    if (Event.type == motion_type) {
        int    loop;
        XDeviceMotionEvent *motion = (XDeviceMotionEvent *) &Event;

        printf("motion ");

        for(loop=0; loop<motion->axes_count; loop++) {
        printf("a[%d]=%d ", motion->first_axis + loop, motion->axis_data[loop]);
        }
        printf("\n");
    } else if ((Event.type == button_press_type) ||
           (Event.type == button_release_type)) {
        int    loop;
        XDeviceButtonEvent *button = (XDeviceButtonEvent *) &Event;

        printf("button %s %d ", (Event.type == button_release_type) ? "release" : "press  ",
           button->button);

        for(loop=0; loop<button->axes_count; loop++) {
        printf("a[%d]=%d ", button->first_axis + loop, button->axis_data[loop]);
        }
        printf("\n");
    } else if ((Event.type == key_press_type) ||
           (Event.type == key_release_type)) {
        int    loop;
        XDeviceKeyEvent *key = (XDeviceKeyEvent *) &Event;

        printf("key %s %d ", (Event.type == key_release_type) ? "release" : "press  ",
           key->keycode);

        for(loop=0; loop<key->axes_count; loop++) {
        printf("a[%d]=%d ", key->first_axis + loop, key->axis_data[loop]);
        }
        printf("\n");
    } else if ((Event.type == proximity_out_type) ||
           (Event.type == proximity_in_type)) {
        int    loop;
        XProximityNotifyEvent *prox = (XProximityNotifyEvent *) &Event;

        printf("proximity %s ", (Event.type == proximity_in_type) ? "in " : "out");

        for(loop=0; loop<prox->axes_count; loop++) {
        printf("a[%d]=%d ", prox->first_axis + loop, prox->axis_data[loop]);
        }
        printf("\n");
    }
    else {
        printf("what's that %d\n", Event.type);
    }
    }
}


// Определение версии библиотеки расширений, установленной для X11
int xinput_version(Display    *display)
{
    XExtensionVersion    *version;
    static int vers = -1;

    if (vers != -1)
        return vers;

    version = XGetExtensionVersion(display, INAME);

    if (version && (version != (XExtensionVersion*) NoSuchExtension)) {
    vers = version->major_version;
    XFree(version);
    }

#if HAVE_XI2
    /* Announce our supported version so the server treats us correctly. */
    if (vers >= XI_2_Major)
    {
        const char *forced_version;
        int maj = 2,
            min = 0;

#if HAVE_XI22
        min = 2;
#elif HAVE_XI21
        min = 1;
#endif

        forced_version = getenv("XINPUT_XI2_VERSION");
        if (forced_version) {
            if (sscanf(forced_version, "%d.%d", &maj, &min) != 2) {
                fprintf(stderr, "Invalid format of XINPUT_XI2_VERSION "
                                "environment variable. Need major.minor\n");
                exit(1);
            }
            printf("Overriding XI2 version to: %d.%d\n", maj, min);
        }

        XIQueryVersion(display, &maj, &min);
    }
#endif

    return vers;
}


// Поиск информации об устройстве
XDeviceInfo* find_device_info(Display *display,
                              char *name,
                              Bool only_extended)
{
    XDeviceInfo *devices;
    XDeviceInfo *found = NULL;
    int        loop;
    int        num_devices;
    int        len = strlen(name);
    Bool    is_id = True;
    XID        id = (XID)-1;

    for(loop=0; loop<len; loop++) {
    if (!isdigit(name[loop])) {
        is_id = False;
        break;
    }
    }

    if (is_id) {
    id = atoi(name);
    }

    devices = XListInputDevices(display, &num_devices);

    for(loop=0; loop<num_devices; loop++) {
    if ((!only_extended || (devices[loop].use >= IsXExtensionDevice)) &&
        ((!is_id && strcmp(devices[loop].name, name) == 0) ||
         (is_id && devices[loop].id == id))) {
        if (found) {
            fprintf(stderr,
                    "Warning: There are multiple devices named '%s'.\n"
                    "To ensure the correct one is selected, please use "
                    "the device ID instead.\n\n", name);
        return NULL;
        } else {
        found = &devices[loop];
        }
    }
    }
    return found;
}


int test(Display *display, char *deviceId)
{
    XDeviceInfo *info;

    Bool handle_proximity = True;

    info = find_device_info(display, deviceId, True);

    if(!info)
    {
      printf("unable to find device '%s'\n", deviceId);
      exit(1);
    }
    else
    {
      if(register_events(display, info, deviceId, handle_proximity))
         print_events(display);
      else
      {
        fprintf(stderr, "no event registered...\n");
        exit(1);
      }
    }

    return 0;
}


int main()
{
  Display *display;
  int event, error;

  // Инициируется указатель на текущий дисплей
  display = XOpenDisplay(NULL);
  if (display == NULL)
  {
    printf("Unable to connect to X server\n");
    exit(1);
  }

  // Проверяется наличие расширений
  if(!XQueryExtension(display, "XInputExtension", &xi_opcode, &event, &error))
  {
    printf("X Input extension not available.\n");
    exit(1);
  }

  // Проверяется версия расширения, она не должна быть нулевой
  if(!xinput_version(display))
  {
    printf("%s extension not available\n", INAME);
    exit(1);
  }

  char deviceId[10];
  sprintf(deviceId, "13");

  test(display, deviceId);

  XSync(display, False);
  XCloseDisplay(display);

  return 0;
}
