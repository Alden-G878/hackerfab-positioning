#include "stepper.h"
#include "Arduino.h"

namespace step_dir
{
    static uint64_t step_per_rev;
    static uint32_t step_pin;
    static uint32_t dir_pin;
    void init(uint64_t step_per_rev_, uint32_t step_pin_, uint32_t dir_pin_) {
        step_per_rev = step_per_rev_;
        step_pin = step_pin_;
        dir_pin = dir_pin_;
        pinMode(step_pin, OUTPUT);
        pinMode(dir_pin, OUTPUT);

    }

    void pulseCCW() {
        digitalWrite(dir_pin, CCW_DIR);
        delayMicroseconds(DIR_WAIT);
        digitalWrite(step_pin, HIGH);
        delayMicroseconds(PUL_WAIT);
        digitalWrite(step_pin, LOW);
    }
    void pulseCW() {
        digitalWrite(dir_pin, CW_DIR);
        delayMicroseconds(DIR_WAIT);
        digitalWrite(step_pin, HIGH);
        delayMicroseconds(PUL_WAIT);
        digitalWrite(step_pin, LOW);
    }
    void moveCW(uint64_t num) {
        for(size_t i=0;i<num;i++) {
            pulseCW();
        }
    }
    void moveCCW(uint64_t num) {
        for(size_t i=0;i<num;i++) {
            pulseCCW();
        }
    }
    void move(int64_t num, bool ccw_pos) {
        if(num>0) {
            if(ccw_pos)moveCCW(num);
            else moveCW(num);
        } else if(num<0) {
            if(ccw_pos)moveCW(num);
            else moveCCW(num);
        }
    }
} // namespace step_dir

namespace positioning
{
    static uint64_t steps_per_rev, rev_per_mm;
    static bool ccw_pos;

    void init(uint64_t steps_per_rev_, uint64_t rev_per_mm_, bool ccw_pos_) {
        steps_per_rev = steps_per_rev_;
        rev_per_mm = rev_per_mm_;
        ccw_pos = ccw_pos_;
    }
    int64_t transform_mm_2_step(int64_t m){
        bool m_pos = m>0;
        return m*rev_per_mm*steps_per_rev;
    }
    double transform_step_2_mm(uint64_t s) {
        return (double)s/(double)(steps_per_rev*rev_per_mm);
    }
}
