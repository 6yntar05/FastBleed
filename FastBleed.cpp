#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <boost/program_options.hpp>
#include "platform/platform_picker.hpp"

namespace po = boost::program_options;
int parse_args(int argc, char* argv[]);

int main(int argc, char* argv[]){
    parse_args(argc, argv);
    std::shared_ptr<cirno::control_impl> control = cirno::init();

    int status = control->status();
    switch (status) {
        case -405:
            std::cerr << "This subsystem is now implemented.\n";
            exit(1);
            break;
        case -100:
            std::cerr << "No displays found.\n";
            exit(1);
            break;
        case -102:
            std::cerr << "This build completed without X11 support.\n";
            exit(1);
            break;
        case -101:
            std::cerr << "This build completed without Wayland support.\n";
            exit(1);
            break;
    };

    auto start_time = std::chrono::steady_clock::now();
    control->button(1,true);
    auto stop_time = std::chrono::steady_clock::now();
    control->button(1,false);
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time);
    std::cout << elapsed_ns.count() << " ns\n";

    while(true){
        control->button(1, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        control->button(1, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    };

    control->~control_impl();

    return 0;
}

int parse_args(int argc, char* argv[]){
    po::options_description desc("Usage: gofra [ options ... ]\n\tWhere options");
    desc.add_options()
        ("help,h", "Help page")
        //("test", po::value<int>(), "test")
    ;

    po::variables_map args;
    po::store(po::parse_command_line(argc, argv, desc), args);
    po::notify(args);

    if (args.count("help")) {
        std::cout << desc << "\n";
        exit(0);
    }

    return 0;
}