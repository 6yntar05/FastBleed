#if (defined (_WIN32) || defined (_WIN64))
    #include "platform/winapi/platform.hpp"

#elif __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
    #include "platform/unix/platform.hpp"

#elif __APPLE__
    #error Not implemented for now
    #include "platform/apple/platform.hpp"

#else
    #error WTF what is it instead os? Unable to pick any implementation.

#endif