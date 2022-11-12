#include "utils/args.hpp"
#include <iostream>
#include <typeinfo>

namespace utils {

namespace program_options {

// template<typename T>
// value<T>::value() {}

// template<typename T>
// value<T>::value(const T &value) : _value(value) {}

option::option(const char name_short, const std::string description) :
    name_short(name_short), description(description) {}

option::option(const char name_short, untyped_value *value, const std::string description) :
    name_short(name_short), value(value), description(description) {}

char option::get_name_short() const {
    return this->name_short;
}

std::string option::get_description() const {
    return this->description;
}

untyped_value *option::get_value() {
    return this->value;
}

void option::set_value(untyped_value *value) {
    this->value = value;
}

options_map::options_map() {}

option_init::option_init(option_description *owner) : owner(owner) {}

option_init &option_init::operator()(const std::string name, const std::string description) {
    return this->operator()(name, nullptr, description);
}

option_init &option_init::operator()(const std::string name, untyped_value *value, const std::string description) {
    owner->add(name, value, description);

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
    this->add(name, nullptr, description);
}

void option_description::add(const std::string name, untyped_value *value, const std::string description) {
    this->options.insert({
        name.substr(0, name.length() - 2),
        option(name[name.length() - 1], value, description)
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
    options_map out;
    bool is_found = false;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int j = 1;
            while (argv[i][j++] != 0) {
                is_found = false;
                for (auto option: od.get_options()) {
                    if (option.second.get_name_short() == argv[i][j - 1]) {
                        if (option.second.get_value() != nullptr) {
                            *option.second.get_value()->value->u_value = argv[++i]; // не работает как надо
                            std::cout << (option.second.get_value()->value->u_value) << std::endl; // не работает как надо
                        }
                        out.insert({option.first, option.second});
                        is_found = true;
                        break;
                    }
                }
                if (!is_found) {
                    out.insert({"error", option('\0', "error")});
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
