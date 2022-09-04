#include "./config.hpp"

#include "simdjson.h"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <string>

namespace cirno {

using namespace simdjson;

e_actions to_action(std::string str) {
    if (str == "DELAY") return ACT_DELAY;
    else if (str == "CLICK") return ACT_CLICK;
    else if (str == "RELEASE") return ACT_RELEASE;
    else if (str == "MOVE") return ACT_MOVE;
    else if (str == "CLICKER") return ACT_CLICKER;
    else if (str == "TEXT_TYPE") return ACT_TEXT_TYPE;
    else if (str == "SYS_EXEC") return ACT_SYS_EXEC;

    std::cerr << "Failed to recognize action type\n";
    return ACT_NULL;
}

void c_config::allocate(unsigned int count) {
    this->ret.count = count;

    this->ret.ev_button = new unsigned int[count];
    this->ret.flag = new bool[count];
    std::fill(this->ret.flag, this->ret.flag + count, false);
    this->ret.action = new std::vector<e_actions>[count];
    this->ret.action_param = new std::vector<unsigned int>[count];

    this->allocated = true;
}

void c_config::deallocate() {
    delete[] this->ret.ev_button;
    delete[] this->ret.flag;
    delete[] this->ret.action;
    delete[] this->ret.action_param;
}

c_config::c_config(std::string PATH) { this->PATH = PATH; }

c_config::~c_config() { if (this->allocated) this->deallocate(); }

s_event_decl c_config::parse() {
    std::vector<e_actions> vec_action_script;
    std::vector<unsigned int> vec_action_params;
    unsigned int count, macro_len;

    dom::parser parser;
    dom::element doc = parser.load(this->PATH);

    auto array = doc.at_key("items").get_array();
    count = array.size();
    this->allocate(count);

    for (unsigned int i = 0; i < count; i++) {  // Parsing fields
        // trigger
        this->ret.ev_button[i] = array.at(i).at_key("trigger").get_uint64();

        // actions
        auto _actions = array.at(i).at_key("actions").get_array();
        macro_len = _actions.size();
        for (int i = 0; i < macro_len; i++) {
            vec_action_script.push_back(
                to_action(
                    std::string(_actions.at(i).get_c_str())
                )
            );
        }

        // act_params
        auto _act_params = array.at(i).at_key("act_params").get_array();
        macro_len = _act_params.size();
        for (int i = 0; i < macro_len; i++) {
            vec_action_params.push_back(static_cast<unsigned int>(_act_params.at(i).get_uint64()));
        }

        // write
        this->ret.action[i] = vec_action_script;
        this->ret.action_param[i] = vec_action_params;
        vec_action_script.clear();
        vec_action_params.clear();
    }

    return ret;
}

void c_config::sync() { return; }

} // namespace cirno
