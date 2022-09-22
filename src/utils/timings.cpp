#include <cmath>
#include <chrono>
#include <random>
#include <iostream>

#include "utils/timings.hpp"
#include "properties.hpp"
#include "runtime.hpp"

namespace utils {

/// Calculate clicker timings by args
t_timings calculate_timings(float cps, float relation, unsigned int entropy_variation) {
    t_timings ret;
    ret.entropy_variation = entropy_variation;

    if (relation<=0) {
        relation = c_relation;
        ui::warn("CPS<=0! Built-in cps("+std::to_string(cps)+") value are used.");
    }

    if (cps<=0) {
        cps = c_cps;
        ui::warn("CPS<=0! Built-in cps("+std::to_string(cps)+") value are used.");
    }
    float total_click_time = 1000.0f / cps;

    // Fallback values:
    ret.hold_time    = static_cast<unsigned int>(total_click_time) / 2;    
    ret.release_time = static_cast<unsigned int>(total_click_time) / 2;

    // An auxiliary number that helps determine the most approximate value; = biggest possible value :
    float best       = total_click_time;

    float x, y; // System of equations:
    // /x + t = total_click_time
    // \x / y = relation

    for (x = 1; x < total_click_time - 1; x++) {
        y = ceil(total_click_time - x);
        if (fabs((y+x) - total_click_time) < 1) {
            if (fabs((x/y) - relation) < fabs(best - relation)) {
                best = x/y;
                ret.hold_time    = static_cast<unsigned int>(x);
                ret.release_time = static_cast<unsigned int>(y);
            }
        }
    }

    if (best == total_click_time) {
        ui::warn("Failed to calculate timings! Fallback values are used.");
        return calculate_timings(c_cps, c_relation, c_entropy_variation);    // Incorrect build-in values can lead to looping recursion
    }

    if (be_verbose)
        std::cerr << "[INFO ] Calculated timings => "
            << ret.hold_time << "ms /" << ret.release_time << "ms" << std::endl;

    return ret;
}

} // namespace utils