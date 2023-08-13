#include <string>

namespace utils {

typedef struct s_timings {
    unsigned int hold_time;
    unsigned int release_time;
    unsigned int entropy_variation;
} t_timings;

t_timings calculate_timings(float cps, float relation, unsigned int entropy_variation);

}