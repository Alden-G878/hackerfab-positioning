#pragma once
#include <cstdint>

namespace enc {
    enum quad_state : uint8_t {
        PREV_LEFT = 0,
        PREV_RIGHT = 1
    };
    
    void init(uint32_t p0, uint32_t p1, bool ccw_pos); // attach pin interrupts to p0, p1 that inc & dec based on state machine
    void reset(); // sets current position to 0
    void get_pos(); // reads value that pin interrupt has calculated

}