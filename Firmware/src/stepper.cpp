#include "stepper.h"
#include "Arduino.h"

namespace step_dir
{
    static uint64_t step_per_rev;
    static uint32_t step_pin_x, step_pin_y, step_pin_z;
    static uint32_t dir_pin_x, dir_pin_y, dir_pin_z;
    void init(uint64_t step_per_rev_, uint32_t step_pin_x_, uint32_t dir_pin_x_, uint32_t step_pin_y_, uint32_t dir_pin_y_, uint32_t step_pin_z_, uint32_t dir_pin_z_) {
        step_per_rev = step_per_rev_;
        step_pin_x = step_pin_x_;
        dir_pin_x = dir_pin_x_;
        step_pin_y = step_pin_y_;
        dir_pin_y = dir_pin_y_;
        step_pin_z = step_pin_z_;
        dir_pin_z = dir_pin_z_;
        pinMode(step_pin_x, OUTPUT);
        pinMode(dir_pin_x, OUTPUT);
        pinMode(step_pin_y, OUTPUT);
        pinMode(dir_pin_y, OUTPUT);
        pinMode(step_pin_z, OUTPUT);
        pinMode(dir_pin_z, OUTPUT);
    }

    void pulseCCW_x() {
        digitalWrite(dir_pin_x, CCW_DIR);
        delayMicroseconds(DIR_WAIT);
        digitalWrite(step_pin_x, HIGH);
        delayMicroseconds(PUL_WAIT);
        digitalWrite(step_pin_x, LOW);
    }
    void pulseCW_x() {
        digitalWrite(dir_pin_x, CW_DIR);
        delayMicroseconds(DIR_WAIT);
        digitalWrite(step_pin_x, HIGH);
        delayMicroseconds(PUL_WAIT);
        digitalWrite(step_pin_x, LOW);
    }
    void moveCW_x(uint64_t num) {
        for(size_t i=0;i<num;i++) {
            pulseCW_x();
        }
    }
    void moveCCW_x(uint64_t num) {
        for(size_t i=0;i<num;i++) {
            pulseCCW_x();
        }
    }
    void pulseCCW_y() {
        digitalWrite(dir_pin_y, CCW_DIR);
        delayMicroseconds(DIR_WAIT);
        digitalWrite(step_pin_y, HIGH);
        delayMicroseconds(PUL_WAIT);
        digitalWrite(step_pin_y, LOW);
    }
    void pulseCW_y() {
        digitalWrite(dir_pin_y, CW_DIR);
        delayMicroseconds(DIR_WAIT);
        digitalWrite(step_pin_y, HIGH);
        delayMicroseconds(PUL_WAIT);
        digitalWrite(step_pin_y, LOW);
    }
    void moveCW_y(uint64_t num) {
        for(size_t i=0;i<num;i++) {
            pulseCW_y();
        }
    }
    void moveCCW_y(uint64_t num) {
        for(size_t i=0;i<num;i++) {
            pulseCCW_y();
        }
    }
    void pulseCCW_z() {
        digitalWrite(dir_pin_z, CCW_DIR);
        delayMicroseconds(DIR_WAIT);
        digitalWrite(step_pin_z, HIGH);
        delayMicroseconds(PUL_WAIT);
        digitalWrite(step_pin_z, LOW);
    }
    void pulseCW_z() {
        digitalWrite(dir_pin_z, CW_DIR);
        delayMicroseconds(DIR_WAIT);
        digitalWrite(step_pin_z, HIGH);
        delayMicroseconds(PUL_WAIT);
        digitalWrite(step_pin_z, LOW);
    }
    void moveCW_z(uint64_t num) {
        for(size_t i=0;i<num;i++) {
            pulseCW_z();
        }
    }
    void moveCCW_z(uint64_t num) {
        for(size_t i=0;i<num;i++) {
            pulseCCW_z();
        }
    }
    void move(motor m, int64_t num, bool ccw_pos) {
        switch(m) {
            case x:
                if(num>0) {
                    if(ccw_pos)moveCCW_x(num);
                    else moveCW_x(num);
                } else if(num<0) {
                    if(ccw_pos)moveCW_x(num);
                    else moveCCW_x(num);
                }
                break;
            case y:
                if(num>0) {
                    if(ccw_pos)moveCCW_y(num);
                    else moveCW_y(num);
                } else if(num<0) {
                    if(ccw_pos)moveCW_y(num);
                    else moveCCW_y(num);
                }
                break;
            case z:
                if(num>0) {
                    if(ccw_pos)moveCCW_z(num);
                    else moveCW_z(num);
                } else if(num<0) {
                    if(ccw_pos)moveCW_z(num);
                    else moveCCW_z(num);
                }
                break;
        }
    }
} // namespace step_dir

namespace positioning
{
    static uint64_t steps_per_rev, um_per_rev;
    static bool ccw_pos;

    void init(uint64_t steps_per_rev_, uint64_t um_per_rev_, bool ccw_pos_) {
        steps_per_rev = steps_per_rev_;
        um_per_rev = um_per_rev_;
        ccw_pos = ccw_pos_;
    }
    int64_t transform_um_2_step(double um){
        bool m_pos = um>0;
        return static_cast<int64_t>(std::floor((um/um_per_rev)*steps_per_rev));
    }
    double transform_step_2_um(int64_t s) {
        return (double)s/(double)(steps_per_rev/um_per_rev);
    }
}
