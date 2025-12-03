#pragma once
#include <cstdint>
#include <Arduino.h>

#define DIR_WAIT 10
#define PUL_WAIT 10
#define CW_DIR HIGH
#define CCW_DIR LOW

namespace step_dir { // may change to interrupt driven system later
    enum motor : uint8_t {
        x,
        y,
        z
    };
    void init(uint64_t step_per_rev_, uint32_t step_pin_x_, uint32_t dir_pin_x_, uint32_t step_pin_y_, uint32_t dir_pin_y_, uint32_t step_pin_z_, uint32_t dir_pin_z_);
    void move(motor m, int64_t num, bool ccw_pos); // moves correct number of CW/CCW steps, num calculated in transform
}

namespace positioning {
    void init(uint64_t steps_per_rev_, uint64_t um_per_rev_, bool ccw_pos_);
    int64_t transform_um_2_step(double um); // m in millimeters, if right_pos then ccw is true when m>0
    double transform_step_2_um(int64_t s);
}