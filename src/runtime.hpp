#pragma once

#include <string>
#include <vector>

/// Runtime constants
extern float cps;
extern float relation;
extern unsigned int entropy_variation;
extern unsigned int actions_cooldown;
extern std::string config_path;

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

struct s_action {
    e_actions action = {};
    unsigned int param = 0;
};

using vec_actions = std::vector<s_action>;

struct s_macro {
  private:
    bool active_flag    = false;

  public:
    // trigger:
    bool was_mouse      = false;
    unsigned int ev_button;

    // actions
    vec_actions actions;

    // service
    bool is_active() { return this->active_flag; }
    void set_active(bool state) { this->active_flag = state; }
    s_macro() {}
    explicit s_macro(unsigned int ev_button, vec_actions actions) : ev_button(ev_button) {}
    explicit s_macro(bool was_mouse, unsigned int ev_button, vec_actions actions) :  was_mouse(was_mouse), ev_button(ev_button) {}
};

// Shared vector of structs
using s_event_decl = std::vector<s_macro*>;