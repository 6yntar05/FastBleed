#include "utils/config.hpp"
#include "ui/feedback.hpp"

#include "simdjson.h"
#include "excepts.hpp"
#include <cstddef>
#include <unordered_map>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>

namespace utils {

using namespace simdjson;

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
    dom::element doc;

    try {
        doc = parser.load(this->PATH);
    } catch (const simdjson_error& e) {
        // Create new config
        throw excepts::error("Unable to open config", "config.cpp");
    }

    auto array = doc.at_key("items").get_array();
    count = static_cast<unsigned int>(array.size());
    this->allocate(count);

    for (size_t i = 0; i < count; i++) {  // Parsing fields
        // trigger
        this->ret.ev_button[i] = array.at(i).at_key("trigger").get_uint64();

        // actions
        auto _actions = array.at(i).at_key("actions").get_array();
        macro_len = static_cast<unsigned int>(_actions.size());
        for (unsigned int i = 0; i < macro_len; i++) {
            vec_action_script.push_back(
                to_action(
                    std::string(_actions.at(i).get_c_str())
                )
            );
        }

        // act_params
        auto _act_params = array.at(i).at_key("act_params").get_array();
        macro_len = static_cast<unsigned int>(_act_params.size());
        for (unsigned int i = 0; i < macro_len; i++) {
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

void c_config::sync() {}

} // namespace utils
