#include <vector>
#include <ostream>
#include <string>
#include <map>

namespace utils {

void parse_args(const int argc, char *argv[]);

namespace program_options {

class option {
private:
    char name_short;
    // std::string name_long;
    std::string description;

public:
    option(const char name_short, const std::string description);

    char get_name_short() const;
    // std::string get_name_long() const;
    std::string get_description() const;
};

class options_map : public std::map<std::string, option> {
private:

public:
    // std::map<std::string, option> args;
    options_map();
    // options_map(std::map<std::string, option> &args);
};

class option_description;

class option_init {
private:
    option_description *owner;

public:
    option_init(option_description *owner);

    option_init &operator()(const std::string name, const std::string description);
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
    
    options_map get_options() const;

    friend std::ostream& operator<<(std::ostream &os, const option_description &od);
};

void parse(int argc, char *argv[]);

options_map
parse_command_line(int argc, char *argv[], option_description &od);

void store(const options_map &in, options_map &out);

}// end program_options

}// end utils