#include <memory>
#include <string>
#include <vector>
#include "excepts.hpp"
#include "ui/feedback.hpp"

using std::string;

namespace excepts {

error::error(const string msg, const string file, const string func, const string info) 
    : msg(msg), file(file), func(func), info(info) {
    // Show error
    ui::error(msg);

    // Construct what()
    this->what_string = "excepts::error";

    if (!this->file.empty()) this->what_string.append("\n\t| In file: "+file+" |\n");
    if (!this->func.empty()) this->what_string.append("\n\t| In function: "+func+" |\n");
    if (!this->info.empty()) this->what_string.append("\n\t| "+info+" |\n");

    this->what_msg = this->what_string.data();
}

const char * error::what() const throw() {
    return this->what_msg;
}

}