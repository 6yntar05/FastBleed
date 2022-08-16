#include <cmath>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>
#include <boost/program_options.hpp>
#include "properties.hpp"                               // Default values
#include "platform/platform_picker.hpp"                 // cirno::init() returns platform-non-specifically abstraction

const float cps               = c_cps;                  // Click Per Second
const float porosity          = c_porosity;             // Hold time / Release time
const int   entropy_variation = c_entropy_variation;    // Introduces randomness in the timings of pressing
// = Default values from "properties.hpp"

namespace po = boost::program_options;
int parse_args(int argc, char* argv[]);

typedef struct s_timings {
    int hold_time;
    int release_time;
    int entropy_variation;
} t_timings;
t_timings calculate_timings(float cps, float porosity);

int main(int argc, char* argv[]){
    parse_args(argc, argv);
    std::shared_ptr<cirno::control_impl> control = cirno::init();                   // Pick platform-non-specifically abstraction

    t_timings timings = calculate_timings(cps, porosity);                           // Press delays around CPS value

    std::random_device rd; std::mt19937 gen_seed(rd());
    std::uniform_int_distribution<> entropy(-entropy_variation, entropy_variation); // Introduces randomness in the timings of pressing

    int status = control->status();
    switch (status) {
        case -405:
            std::cerr << "This subsystem is now implemented.\n";
            break;
        case -100:
            std::cerr << "No displays found.\n";
            break;
        case -102:
            std::cerr << "This build completed without X11 support.\n";
            break;
        case -101:
            std::cerr << "This build completed without Wayland support.\n";
            break;
    }; if (status < 0) exit(1);

    auto start_time = std::chrono::steady_clock::now();
    control->button(1,true);
    auto stop_time = std::chrono::steady_clock::now();
    control->button(1,false);
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time);
    std::cout << "Benchmark click time: " << elapsed_ns.count() << " ns\n";

    while(true){
        control->button(1, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(timings.hold_time + entropy(gen_seed)));
        control->button(1, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(timings.release_time + entropy(gen_seed)));
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

t_timings calculate_timings(float cps, float porosity) {
    t_timings ret;
    float total_click_time = 1000.0f / cps;

    // Fallback values:
    ret.hold_time    = total_click_time / 2;    
    ret.release_time = total_click_time / 2;

    // An auxiliary number that helps determine the most approximate value; = biggest possible value :
    float best       = total_click_time;

    float x, y; // System of equations:
    // /x + t = total_click_time
    // \x / y = porosity

    for (x = 1; x < total_click_time - 1; x++){
        y = ceil(total_click_time - x);
        if ( fabs((y+x) - total_click_time) < 1 ) {
            if ( fabs((x/y) - porosity) < fabs(best - porosity) ) {
                best = x/y;
                ret.hold_time    = x;
                ret.release_time = y;
            }// else {break;}
        }
    }
    
    #ifdef DEBUG
        std::cerr << "[DEBUG] calculate_timings() => " << ret.hold_time << "/" << ret.release_time << std::endl;
    #endif
    return ret;
}