#include <string>

namespace cirno {

static const std::string config_path;

class c_config {
private:
    std::string PATH;

public:
    c_config(std::string PATH);
    void parse();
    void sync();
};

} // namespace cirno