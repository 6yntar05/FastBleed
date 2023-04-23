#include <string_view>

#ifdef USE_COLORS
    #define CLR "\033[0m"
    #define RED "\033[31m"
    #define YEL "\033[33m"
    #define BOLD "\033[1m"
#else
    #define CLR ""
    #define RED ""
    #define YEL ""
    #define BOLD ""
#endif

namespace ui {
// todo: ostream
void error(std::string_view msg);
void warn(std::string_view msg);
void info(std::string_view msg);
void msg(std::string_view msg);

}