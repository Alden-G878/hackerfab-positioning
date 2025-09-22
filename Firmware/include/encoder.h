#pragma once
#include <cstdint>

namespace enc {
    enum quad_state : uint8_t {
        PREV_LEFT_HIGH = 0,
        PREV_LEFT_LOW = 1,
        PREV_RIGHT_HIGH = 2,
        PREV_RIGHT_LOW = 3
    };
    
    void init(uint32_t p0, uint32_t p1, uint64_t steps_per_rev, bool ccw_pos); // attach pin interrupts to p0, p1 that inc & dec based on state machine
    void reset(); // sets current position to 0
    int64_t get_pos(); // reads value that pin interrupt has calculated

}