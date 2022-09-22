#include <iostream>

extern std::string config_path;
extern float cps;
extern float relation;
extern bool override_xorg;
extern bool override_wayland;
extern bool use_gui;
extern bool be_verbose;

int parse_args(int argc, char* argv[]);
