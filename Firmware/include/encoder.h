#pragma once
#include <cstdint>

namespace enc {
    enum quad_state : uint8_t {
        s00 = 0,
        s01 = 1,
        s11 = 2,
        s10 = 3,
        RESET = 4,
        ERROR = 5
    };
    
    void init(uint32_t pL, uint32_t pR, uint64_t steps_per_rev, bool ccw_pos); // attach pin interrupts to p0, p1 that inc & dec based on state machine
    void reset(); // sets current position to 0
    int64_t get_pos(); // reads value that pin interrupt has calculated
}