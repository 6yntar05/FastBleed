#include <iostream>

#include "utils/args.hpp"
#include "runtime.hpp"

namespace po = utils::program_options;
namespace utils {

void parse_args(const int argc, char *argv[]) {
    po::option_description desc("Usage: FastBleed [ options ... ]\n");
    desc.add_options()
        ("help,h", "Help page")
        ("verbose,v", "Be verbose")
        ("config,c", "Path to config file", true)
        #if defined __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
            ("uinput,u", "Override uinput (default)")
            ("xorg,x", "Override Xorg")
        #endif
    ;

    po::options_map args;
    po::store(po::parse_command_line(argc, argv, desc), args);

    if (args.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    } else if (args.count("error")) {
        std::cout << "Invalid argument\n" << desc << std::endl;
        exit(0);
    }

    if (args.count("config"))
        config_path = args.at("config").get_value();

    if (args.count("verbose"))
        be_verbose = true;

#if defined __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
    if (args.count("uinput"))
        override_uinput = true;

    if (args.count("xorg"))
        override_xorg = true;
#endif
}

} // namespace utils

