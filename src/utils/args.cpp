#include "utils/args.hpp"
// #include <iostream>

namespace utils {

namespace program_options {

// class option
option::option(const char name_short, const std::string description) :
    name_short(name_short), description(description) {
    // this->name_short = name;
    // this->description = description;
}

char option::get_name_short() const {
    return this->name_short;
}

// std::string option::get_name_long() const {
//     return this->name_long;
// }

std::string option::get_description() const {
    return this->description;
}

// class option_map
options_map::options_map() {}

// options_map::options_map(std::map<std::string, option> &args) {

// }

// class option_init
option_init::option_init(option_description *owner) : owner(owner) {}

option_init &option_init::operator()(const std::string name, const std::string description) {
    owner->add(name, description);

    int end = name.find(',', 0);

    if (end > owner->option_name_lenght) {
        owner->option_name_lenght = end;
    }

    return *this;
}

// class option_description
option_description::option_description(const std::string &description) :
    description(description) {}

option_init option_description::add_options() {
    return option_init(this);
}

void option_description::add(const std::string name, const std::string description) {
    // option option(name, description);
    
    this->options.insert({
        name.substr(0, name.length() - 2),
        option(name[name.length() - 1], description)
    });
}

options_map option_description::get_options() const {
    return this->options;
}

std::ostream &operator<<(std::ostream &os, const option_description &od) {
    std::string s_options;

    for (auto option : od.options) {
        s_options += "  -";
        s_options += option.second.get_name_short();
        s_options += " [ --" + option.first + " ]";
        
        for (int j = option.first.length(); j < od.option_name_lenght; j++) {
            s_options += ' ';
        }
        
        s_options += '\t' + option.second.get_description() + '\n';
    }
    s_options.pop_back();

    return os << od.description << std::endl << s_options;
}

options_map
parse_command_line(const int argc, char *argv[], option_description &od) {
    // std::vector<option> options = od.get_options();
    // std::map<char, std::string> args_map;
    options_map out;
    bool is_found = false;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int j = 1;
            while (argv[i][j++] != 0) {
                is_found = false;
                for (auto option: od.get_options()) {
                    if (option.second.get_name_short() == argv[i][j - 1]) {
                        out.insert({option.first, option.second});
                        
                        is_found = true;
                        break;
                    }
                }
                if (!is_found) {
                    out.insert({"error", option('\0', "error")});
                }
            }
        }
    }

    return out;
}

void store(const options_map &in, options_map &out) {
    out = in;
}

}// end program_options

}// end utils