#include <iostream>

#include "utils/args.hpp"
#include "runtime.hpp"

namespace po = utils::program_options;

namespace utils {

void parse_args(const int argc, char *argv[]) {
    po::option_description desc("Usage: FastBleed [ options ... ]\n");
    desc.add_options()
        ("help,h", "Help page")
        ("headless,l", "Headless")
        ("verbose,v", "Be verbose")
        ("config,c", "Path to config file", true)
        #if defined __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
            ("xorg,x", "Override Xorg")
            ("wayland,w", "Override Wayland")
        #endif
    ;

    po::options_map args;
    po::store(po::parse_command_line(argc, argv, desc), args);

    if (args.count("error")) {
        std::cout << "Invalid argument\n" << desc << std::endl;
        exit(0);
    }
    if (args.count("gui")) {
        std::cout << "gui" << std::endl;
    }
    if (args.count("config")) {
        config_path = args.at("config").get_value();
    }
    if (args.count("verbose")) {
        be_verbose = true;
    }
#if defined __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
    if (args.count("xorg")) {
        override_xorg = true;
    }
    if (args.count("wayland")) {
        override_wayland = true;
    }
#endif
    if (args.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }
}

}// end utils

