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
    void init(uint64_t steps_per_rev_, uint32_t um_per_rev, bool ccw_pos_x_, bool ccw_pos_y_, bool ccw_pos_z_, uint8_t pL_x_, uint8_t pL_y_, uint8_t pL_z_, uint8_t pR_x_, uint8_t pR_y_, uint8_t pR_z_); // attach pin interrupts to p0, p1 that inc & dec based on state machine
    void reset_x(); // sets current position to 0
    void reset_y(); // sets current position to 0
    void reset_z(); // sets current position to 0
    int64_t get_pos_steps_x(); // reads value that pin interrupt has calculated
    int64_t get_pos_steps_y(); // reads value that pin interrupt has calculated
    int64_t get_pos_steps_z(); // reads value that pin interrupt has calculated
    double get_pos_rev(int64_t pos_steps);
    double get_pos_um(int64_t pos_steps);
}