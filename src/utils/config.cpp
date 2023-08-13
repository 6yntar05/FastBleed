#include "utils/config.hpp"

#include "simdjson.h"
#include "excepts.hpp"
#include <cstddef>
#include <string.h>
#include <unordered_map>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>

namespace utils {

using namespace simdjson;

s_event_decl c_config::parse() {
    s_event_decl ret;
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

    for (size_t i = 0; i < count; i++) {  // Parsing fields for every macro
        s_macro* macro = new s_macro;

        macro->ev_button = static_cast<unsigned int>(array.at(i).at_key("trigger").get_uint64());
        macro->was_mouse = true; // Temporary

        // actions
        vec_actions actions;
        auto _actions = array.at(i).at_key("actions").get_array();
        macro_len = static_cast<unsigned int>(_actions.size());
        for (unsigned int i = 0; i < macro_len; i++) {
            s_action action;
            action.action = 
                to_action(
                    std::string(_actions.at(i).get_c_str())
                );
            actions.push_back(action);
        }

        // action params
        std::vector<unsigned int> vec_action_params;
        auto _act_params = array.at(i).at_key("act_params").get_array();
        macro_len = static_cast<unsigned int>(_act_params.size());
        for (unsigned int i = 0; i < macro_len; i++) {
            actions.at(i).param = static_cast<unsigned int>(_act_params.at(i).get_uint64());
        }

        // write
        macro->actions = actions;
        ret.push_back(macro);
    }

    return ret;
}

c_config::c_config(std::string PATH) { this->PATH = PATH; }

} // namespace utils
