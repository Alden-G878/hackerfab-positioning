#include "encoder.h"
#include <Arduino.h>

namespace enc
{
    volatile quad_state state;
    volatile quad_state prev_state = RESET;
    volatile bool is_ccw_dir;
    uint64_t steps_per_rev;
    uint32_t pL, pR;
    bool ccw_pos;
    volatile int64_t pos_steps;
    void handle_fsm() {
        bool l = digitalRead(pL);
        bool r = digitalRead(pR);
        uint8_t s = (l && 2) | (r && 1);
        switch(state) {
            case s00:
                prev_state = s00;
                if(s==1) {
                    state = s01;
                    ++pos_steps;
                } else if(s==2) {
                    state = s10;
                    --pos_steps;
                } else {
                    state = ERROR;
                }
            break;
            case s01:
                prev_state = s01;
                if(s==3) {
                    state = s11;
                    ++pos_steps;
                } else if(s==0) {
                    state = s00;
                    --pos_steps;
                } else {
                    state = ERROR;
                }
            break;
            case s11:
                prev_state = s11;
                if(s==2) {
                    state = s10;
                    ++pos_steps;
                } else if(s==1) {
                    state = s01;
                    --pos_steps;
                } else {
                    state = ERROR;
                }
            break;
            case s10:
                prev_state = s10;
                if(s==0) {
                    state = s00;
                    ++pos_steps;
                } else if(s==3) {
                    state = s11;
                    --pos_steps;
                } else {
                    state = ERROR;
                }
            break;
            case RESET:
                if(prev_state!=ERROR) pos_steps = 0;
                if(s==0) state = s00;
                else if(s==1) state = s01;
                else if(s==2) state = s10;
                else if(s==3) state = s11;
                else state = ERROR;
                prev_state = RESET;
            break;
            case ERROR:
                Serial.printf("Encountered error, prev state was %u",prev_state);
                prev_state = ERROR;
                state = RESET;
            break;
        }
    }
    void init(uint32_t pL_, uint32_t pR_, uint64_t steps_per_rev_, bool ccw_pos_) {
        steps_per_rev = steps_per_rev_;
        ccw_pos = ccw_pos_;
        pL = pL_;
        pR = pR;
        pinMode(pL, INPUT);
        pinMode(pR, INPUT);
        attachInterrupt(pL, &handle_fsm, CHANGE);
        attachInterrupt(pR, &handle_fsm, CHANGE);
    }
    void reset() {
        state = RESET;
        pos_steps = 0;
    }
    int64_t get_pos() {
        return pos_steps;
    }
}