#pragma once

#include <string>
#include <unordered_map>

#include "runtime.hpp"
#include "spdlog/spdlog.h"

namespace utils {

class c_config {
private:
    std::string PATH;
    s_event_decl ret;
    bool allocated = false;
    
    /// Interpret string to enum e_actions
    e_actions to_action(std::string str) {
        std::unordered_map<std::string, e_actions> umap;
        umap["DELAY"] = ACT_DELAY;
        umap["CLICK"] = ACT_CLICK;
        umap["RELEASE"] = ACT_RELEASE;
        umap["MOVE"] = ACT_MOVE;
        umap["CLICKER"] = ACT_CLICKER;
        umap["TEXT_TYPE"] = ACT_TEXT_TYPE; // TODO: binder??

        if (umap.find(str) == umap.end())
            spdlog::error("Config syntax error : Invalid action \"" + str + "\"");

        return umap.at(str);
    }

public:
    s_event_decl parse();
    c_config() : c_config("./config.cfg") {};
    c_config(std::string path);
};

} // namespace utils