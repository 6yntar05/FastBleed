#include <cmath>
#include <chrono>
#include <random>
#include <string>
#include <iostream>

#include "ui/feedback.hpp"

extern std::string config_path;
extern float cps;
extern float relation;
extern bool override_xorg;
extern bool override_wayland;
extern bool use_gui;
extern bool be_verbose;

typedef struct s_timings {
    unsigned int hold_time;
    unsigned int release_time;
    unsigned int entropy_variation;
} t_timings;

t_timings calculate_timings(float cps, float relation, unsigned int entropy_variation);