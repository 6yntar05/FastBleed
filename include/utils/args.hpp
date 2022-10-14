#include <vector>
#include <ostream>
#include <string>
#include <unordered_map>

namespace utils {

void parse_args(const int argc, char *argv[]);

namespace program_options {

template<typename T>
class typed_value {
private:
    T *value;

public:
    typed_value() : value(0) {};
    typed_value(T *value) : value(value) {};
};

template<typename T>
typed_value<T> *value() {
    return program_options::value<T>(0);
}

template<typename T>
typed_value<T> *value(T *v)
{
    typed_value<T> *r = new typed_value<T>(v);

    return r;
}

class option {
private:
    char name_short;
    std::string description;

public:
    option(const char name_short, const std::string description);
    template<typename T>
    option(const char name_short, typed_value<T> *value, const std::string description);

    char get_name_short() const;
    std::string get_description() const;
};

class options_map : public std::unordered_map<std::string, option> {
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
    
    template<typename T>
    option_init &operator()(const std::string name, typed_value<T> *value, const std::string description);
};

class option_description {
private:
    std::string description;
    options_map options;

public:
    int option_name_lenght;
    
public:
    option_description(const std::string &description);

    option_init add_options();

    void add(const std::string name, const std::string description);
    template<typename T>
    void add(const std::string name, typed_value<T> *value, const std::string description);
    
    options_map get_options() const;

    friend std::ostream& operator<<(std::ostream &os, const option_description &od);
};

void parse(int argc, char *argv[]);

options_map
parse_command_line(int argc, char *argv[], option_description &od);

void store(const options_map &in, options_map &out);

}// end program_options

}// end utils