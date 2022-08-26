#include <string>

namespace cirno {

static const std::string config_path;

class config {
public:
    void* parse();
};

}//; namespace cirno