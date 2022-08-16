#if (defined (_WIN32) || defined (_WIN64))
    #error Not implemented for now
    #include "winapi/platform.hpp"

#elif __APPLE__
    #error Not implemented for now
    #include "apple/platform.hpp"

#elif __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
    #include "unix/platform.hpp"

#else
    #error WTF what is it instead os?

#endif