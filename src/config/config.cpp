#include "./config.hpp"

//#include "simdjson.h"
#include <cassert>
#include <iostream>
#include <string>

namespace cirno {

//using namespace simdjson;

c_config::c_config(std::string PATH) { this->PATH = PATH; }

void c_config::parse() {
    /*
    ondemand::parser parser;
    auto json = padded_string::load(this->PATH);
    ondemand::document doc = parser.iterate(json);
    ondemand::object object = doc.get_object();
    */
    //std::cerr << doc.find_field("count") << std::endl;
}

void c_config::sync() { return; }

} // namespace cirno