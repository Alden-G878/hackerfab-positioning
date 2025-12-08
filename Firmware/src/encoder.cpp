#include "encoder.h"
#include <Arduino.h>

namespace enc
{
    volatile quad_state state_x;
    volatile quad_state prev_state_x = RESET;
    volatile quad_state state_y;
    volatile quad_state prev_state_y = RESET;
    volatile quad_state state_z;
    volatile quad_state prev_state_z = RESET;
    volatile bool is_ccw_dir;
    uint64_t steps_per_rev;
    double um_per_rev;
    uint8_t pL_x, pR_x;
    uint8_t pL_y, pR_y;
    uint8_t pL_z, pR_z;
    bool ccw_pos_x;
    bool ccw_pos_y;
    bool ccw_pos_z;
    volatile int64_t pos_steps_x;
    volatile int64_t pos_steps_y;
    volatile int64_t pos_steps_z;
    void handle_fsm(volatile quad_state *state, volatile quad_state *prev_state, uint8_t *pL, uint8_t *pR, volatile int64_t *pos_steps) {
        bool l = digitalRead(*pL);
        bool r = digitalRead(*pR);
        uint8_t s = (l && 2) | (r && 1);
        switch(*state) {
            case s00:
                *prev_state = s00;
                if(s==1) {
                    *state = s01;
                    ++(*pos_steps);
                } else if(s==2) {
                    *state = s10;
                    --(*pos_steps);
                } else {
                    *state = ERROR;
                }
            break;
            case s01:
                *prev_state = s01;
                if(s==3) {
                    *state = s11;
                    ++(*pos_steps);
                } else if(s==0) {
                    *state = s00;
                    --(*pos_steps);
                } else {
                    *state = ERROR;
                }
            break;
            case s11:
                *prev_state = s11;
                if(s==2) {
                    *state = s10;
                    ++(*pos_steps);
                } else if(s==1) {
                    *state = s01;
                    --(*pos_steps);
                } else {
                    *state = ERROR;
                }
            break;
            case s10:
                *prev_state = s10;
                if(s==0) {
                    *state = s00;
                    ++(*pos_steps);
                } else if(s==3) {
                    *state = s11;
                    --(*pos_steps);
                } else {
                    *state = ERROR;
                }
            break;
            case RESET:
                if(*prev_state!=ERROR) (*pos_steps) = 0;
                if(s==0) *state = s00;
                else if(s==1) *state = s01;
                else if(s==2) *state = s10;
                else if(s==3) *state = s11;
                else *state = ERROR;
                *prev_state = RESET;
            break;
            case ERROR:
                Serial.printf("Encountered error, prev state was %u",prev_state);
                *prev_state = ERROR;
                *state = RESET;
            break;
        }
    }
    void handle_fsm_x() {
        handle_fsm(&state_x, &prev_state_x, &pL_x, &pR_x, &pos_steps_x);
    }
    void handle_fsm_y() {
        handle_fsm(&state_y, &prev_state_y, &pL_y, &pR_y, &pos_steps_y);
    }
    void handle_fsm_z() {
        handle_fsm(&state_z, &prev_state_z, &pL_z, &pR_z, &pos_steps_z);
    }
    void init(uint64_t steps_per_rev_, uint32_t um_per_rev_, bool ccw_pos_x_, bool ccw_pos_y_, bool ccw_pos_z_, uint8_t pL_x_, uint8_t pL_y_, uint8_t pL_z_, uint8_t pR_x_, uint8_t pR_y_, uint8_t pR_z_) {
        steps_per_rev = steps_per_rev_;
        um_per_rev = um_per_rev_;
        ccw_pos_x = ccw_pos_x_;
        ccw_pos_y = ccw_pos_y_;
        ccw_pos_z = ccw_pos_z_;
        pL_x = pL_x_;
        pR_x = pR_x_;
        pL_y = pL_y_;
        pR_y = pR_y_;
        pL_z = pL_z_;
        pR_z = pR_z_;
        pinMode(pL_x, INPUT);
        pinMode(pL_y, INPUT);
        pinMode(pL_z, INPUT);
        pinMode(pR_x, INPUT);
        pinMode(pR_y, INPUT);
        pinMode(pR_z, INPUT);
        attachInterrupt(pL_x, &handle_fsm_x, CHANGE);
        attachInterrupt(pR_x, &handle_fsm_x, CHANGE);
        attachInterrupt(pL_y, &handle_fsm_y, CHANGE);
        attachInterrupt(pR_y, &handle_fsm_y, CHANGE);
        attachInterrupt(pL_z, &handle_fsm_z, CHANGE);
        attachInterrupt(pR_z, &handle_fsm_z, CHANGE);
    }
    void reset_x() {
        state_x = RESET;
        pos_steps_x = 0;
    }
    void reset_y() {
        state_y = RESET;
        pos_steps_y = 0;
    }
    void reset_z() {
        state_z = RESET;
        pos_steps_z = 0;
    }
    int64_t get_pos_steps_x() {
        return pos_steps_x;
    }
    int64_t get_pos_steps_y() {
        return pos_steps_y;
    }
    int64_t get_pos_steps_z() {
        return pos_steps_z;
    }
    double get_pos_rev(int64_t pos_steps) {
        return (double)pos_steps / (double)steps_per_rev;
    }

    double get_pos_um(int64_t pos_steps) {
        return ((double)pos_steps / (double)steps_per_rev) * um_per_rev;
    }
}