#pragma once
#include <cstdint>

namespace step_dir { // may change to interrupt driven system later
    void init(uint64_t step_per_rev, uint32_t step_pin, uint32_t dir_pin);
    void pulseCCW();
    void pulseCW();
    void moveCW(uint64_t num); // call pulseCW in a loop
    void moveCCW(uint64_t num); // call pulseCCW in a loop
}

namespace positioning {
    void init(uint64_t steps_per_rev, uint64_t rev_per_mm, bool ccw_pos);
    uint64_t transform(uint64_t m, bool ccw); // millimeters, if right_pos then ccw is true when m>0
}