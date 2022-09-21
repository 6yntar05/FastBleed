/* DEFAULTS */
    static const float c_cps = 14.0f;                   // Clicks Per Second: float (0.0:500.0)
    static const float c_relation = 0.70f;              // 'Hold time'/'Release time' relation: float (0.0:500/cps)
    static const unsigned int c_entropy_variation = 5;  // Entropy range (value+-delays): uint [0:+inf) (ms)
    static const unsigned int c_actions_cooldown = 20;  // Sleep time of inactivity (ms)
