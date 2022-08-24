/* DEFAULTS */
    const float c_cps = 14.0f;                  // Clicks Per Second: float (0.0:500.0)
    const float c_relation = 0.70f;             // 'Hold time'/'Release time' relation: float (0.0:500/cps)
    const unsigned int c_entropy_variation = 5; // Entropy range (value+-delays): uint [0:+inf)

/* GENERIC BUILD PROPERTIES */
    #define COLORS

/* LINUX BUILD PROPERTIES */
    #define USE_WAYLAND
    #define USE_X11
