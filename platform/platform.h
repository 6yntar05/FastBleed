#if (defined (_WIN32) || defined (_WIN64))
    #error Not implemented for now
    #include "winapi/platform.cpp"

#elif __APPLE__
    #error Not implemented for now
    #include "apple/platform.cpp"

#elif __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
    #include "unix/platform.cpp"
#else
    #error WTF what is it instead os?

#endif

static const int GOFRA_SUCCESS = 0;
static const int GOFRA_NOT_IMPL = -405;
static const int GOFRA_LINUX_NO_X11 = -101;
static const int GOFRA_LINUX_NO_WAYLAND = -102;
static const int GOFRA_LINUX_NO_DISPLAYS = -100;