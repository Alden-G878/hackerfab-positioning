#pragma once
#include <cstdint>
#include <Arduino.h>

#define DIR_WAIT 10
#define PUL_WAIT 10
#define CW_DIR HIGH
#define CCW_DIR LOW

namespace step_dir { // may change to interrupt driven system later
    void init(uint64_t step_per_rev, uint32_t step_pin, uint32_t dir_pin);
    void pulseCCW();
    void pulseCW();
    void moveCW(uint64_t num); // call pulseCW in a loop, does not reutrn until complete
    void moveCCW(uint64_t num); // call pulseCCW in a loop, does not return until complete
    void move(int64_t num, bool ccw_pos); // moves correct number of CW/CCW steps, num calculated in transform
}

namespace positioning {
    void init(uint64_t steps_per_rev, uint64_t rev_per_mm, bool ccw_pos);
    int64_t transform_mm_2_step(int64_t m); // m in millimeters, if right_pos then ccw is true when m>0
    double transform_step_2_mm(uint64_t s);
}