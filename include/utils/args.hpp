#include <vector>
#include <ostream>
#include <string>
#include <map>

namespace utils {

void parse_args(const int argc, char *argv[]);

namespace program_options {

// class untyped_value;
// template<typename T>
// class typed_value;

// class untyped_value {
// public:
// protected:
// };

// template<typename T>
// class typed_value : public untyped_value {
// public:
//     T *value;

//     typed_value(T *value) {
//         this->value = value;
//     }
//     ~typed_value() {
//         delete value;
//     }
// };

// template<typename T>
// typed_value<T> *value() {
//     return program_options::value<T>(0);
// }

// template<typename T>
// typed_value<T> *value(T *v) {
//     typed_value<T> *r = new typed_value<T>(v);
//     return r;
// }

class option {
private:
    char name_short;
    std::string name_long;
    std::string description;
    std::string value = "0";

public:
    option(const char name_short, const std::string name_long, const std::string description);
    // option(const char name_short, const std::string name_long, untyped_value *value, const std::string description);

    char get_name_short() const;
    std::string get_name_long() const;
    std::string get_description() const;

    std::string get_value() const;
    void set_value(const std::string value);
};

class options_map : public std::map<std::string, option> {
private:

public:
    options_map();
};

class option_description;

class option_init {
private:
    option_description *owner;

public:
    option_init(option_description *owner);

    option_init &operator()(const std::string name, const std::string description);
    // option_init &operator()(const std::string name, untyped_value *value, const std::string description);
};

class option_description {
private:
    std::string description;
    std::vector<option> options;

public:
    int option_name_lenght;
    
public:
    option_description(const std::string &description);

    option_init add_options();

    void add(const std::string name, const std::string description);
    // void add(const std::string name, untyped_value *value, const std::string description);
    
    std::vector<option> get_options() const;

    friend std::ostream& operator<<(std::ostream &os, const option_description &od);
};

void parse(int argc, char *argv[]);

options_map
parse_command_line(int argc, char *argv[], option_description &od);

void store(const options_map &in, options_map &out);

}// end program_options

}// end utils
