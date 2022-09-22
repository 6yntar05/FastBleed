#include "utils/args.hpp"

#ifdef USE_BOOST
    #include <boost/program_options.hpp>
    namespace po = boost::program_options;
#endif

int parse_args(int argc, char* argv[]) {
#ifdef USE_BOOST
    po::options_description desc("Usage: gofra [ options ... ]\n\tWhere options");
    desc.add_options()
        ("help,h", "Help page")
        ("verbose,v", "Be verbose")
        ("gui,g", "Use graphic interface")
        ("config,p", po::value<std::string>(&config_path), "Path to config file")
        ("cps,c", po::value<float>(&cps), "Clicks Per Second: float (0.0:500.0)")
        ("relation,r", po::value<float>(&relation), "'Hold time'/'Release time' relation: float (0.0:500/cps)")
        //("entropy,e", po::value<unsigned int>(&entropy_variation), "Entropy range (value+-delays): uint [0:?)")
        #if defined __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
            ("xorg,x", "Override Xorg")
            ("wayland,w", "Override Wayland")
        #endif
    ;
    po::variables_map args;
    po::store(po::parse_command_line(argc, argv, desc), args);
    po::notify(args);

    #if defined __unix__ || defined (LINUX) || defined(__linux__) || defined(__FreeBSD__)
        if (args.count("xorg")) {
            override_xorg = true;
        }
        if (args.count("wayland")) {
            override_wayland = true;
        }
    #endif

    if (args.count("gui")) {
        use_gui = true;
    }
    if (args.count("verbose")) {
        be_verbose = true;
    }
    if (args.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }
#endif
    return 0;
}
