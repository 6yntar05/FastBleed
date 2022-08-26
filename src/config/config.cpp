#include <string>
#include "simdjson.h"

namespace cirno {

static const std::string config_path;

class config {
public:
    void* parse() {
        simdjson::ondemand::parser parser;
        return nullptr;
    }
};

}//; namespace cirno