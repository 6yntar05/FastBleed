#pragma once

#include <string>
#include <vector>

/// Runtime constants
extern float cps;
extern float relation;
extern unsigned int entropy_variation;
extern unsigned int actions_cooldown;
extern std::string config_path;
extern bool use_gui;

/// Runtime flags
extern bool override_wayland, override_xorg, be_verbose;

/// Available actions
enum e_actions {
    ACT_DELAY,
    ACT_CLICK,
    ACT_RELEASE,
    ACT_MOVE,
    ACT_CLICKER,
    ACT_TEXT_TYPE,
    ACT_SYS_EXEC,
    ACT_NULL
};

/// Shared struct
struct s_event_decl {
    unsigned int count;
    unsigned int *ev_button;
    bool *flag;
    std::vector<e_actions> *action;
    std::vector<unsigned int> *action_param;
};