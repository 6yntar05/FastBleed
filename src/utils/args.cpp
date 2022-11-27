#include "utils/args.hpp"
#include <iostream>
#include <typeinfo>

namespace utils {

namespace program_options {

// template<typename T>
// value<T>::value() {}

// template<typename T>
// value<T>::value(const T &value) : _value(value) {}

option::option(const char name_short, const std::string name_long, const std::string description) :
    name_short(name_short), description(description) {}

// option::option(const char name_short, const std::string name_long, untyped_value *value, const std::string description) :
//     name_short(name_short), value(value), description(description) {}

char option::get_name_short() const {
    return this->name_short;
}

std::string option::get_name_long() const {
    return this->name_long;
}

std::string option::get_description() const {
    return this->description;
}

std::string option::get_value() const {
    return this->value;
}

void option::set_value(const std::string value) {
    this->value = value;
}

options_map::options_map() {}

option_init::option_init(option_description *owner) : owner(owner) {}

// option_init &option_init::operator()(const std::string name, const std::string description) {
//     return this->operator()(name, nullptr, description);
// }

option_init &option_init::operator()(const std::string name, const std::string description) {
    owner->add(name, description);

    int end = name.size() - 2;

    if (end > owner->option_name_lenght) {
        owner->option_name_lenght = end;
    }

    return *this;
}

option_description::option_description(const std::string &description) :
    description(description) {}

option_init option_description::add_options() {
    return option_init(this);
}

void option_description::add(const std::string name, const std::string description) {
    this->options.push_back(option(name[name.length() - 1], name.substr(0, name.length() - 2), description));
}

// void option_description::add(const std::string name, untyped_value *value, const std::string description) {
//     this->options.push_back(option(name[name.length() - 1], name.substr(0, name.length() - 2), value, description));
// }

std::vector<option> option_description::get_options() const {
    return this->options;
}

std::ostream &operator<<(std::ostream &os, const option_description &od) {
    std::string s_options;

    for (auto option : od.options) {
        s_options += "  -";
        s_options += option.get_name_short();
        s_options += " [ --" + option.get_name_long() + " ]";
        
        for (int j = option.get_name_long().length(); j < od.option_name_lenght; j++) {
            s_options += ' ';
        }
        
        s_options += '\t' + option.get_description() + '\n';
    }
    s_options.pop_back();

    return os << od.description << std::endl << s_options;
}

options_map
parse_command_line(const int argc, char *argv[], option_description &od) {
    options_map out;
    bool is_found = false;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int j = 1;
            while (argv[i][j++] != 0) {
                is_found = false;
                for (auto option : od.get_options()) {
                    if (option.get_name_short() == argv[i][j - 1]) {
                        if (option.get_value() != "0") {
                            // option.set_value(argv[++i]);
                            // std::cout << option.get_value()->v->value << std::endl;
                            // *option.second.get_value()->v->value = argv[++i];
                            // option.second.get_value()->value = argv[++i];
                            // std::cout << (option.second.get_value()->value->u_value) << std::endl;
                        }
                        out.insert({option.get_name_long(), option});
                        is_found = true;
                        break;
                    }
                }
                if (!is_found) {
                    out.insert({"error", option('\0', "error", "error")});
                    break;
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
