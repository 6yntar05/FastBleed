#include <iostream>

#include "utils/args.hpp"
#include "runtime.hpp"

namespace po = utils::program_options;

namespace utils {

void parse_args(const int argc, char *argv[]) {
    po::option_description desc("Usage: FastBleed [ options ... ]");
    desc.add_options()
        ("help,h", "Help page")
        ("gui,g", "Use graphic interface")
        ("verbose,v", "Be verbose")
        ("config,p", po::value<std::string>(&config_path), "Path to config file")
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
        // exit(0);
    }
    if (args.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }
}

}// end utils

