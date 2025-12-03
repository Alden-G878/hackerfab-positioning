#include "software-serial.h"

namespace SoftwareSerial {
    static int8_t rx_buf[RX_BUF_SIZE];
    StepperPositionCommand step_pos_c;
    PiezoPositionCommand piez_pos_c;
    int8_t min_stepper_res;
    int8_t maximum_voltage;

    void process_gcode(int8_t *s) {
        // structure should be T X_top X_high X_low X_bot Y_top Y_high Y_low Y_bot Z_top Z_high Z_low Z_bot
        //      if T is 'P', X, Y, Z will be interpreted as a stepper pos command, units nm, M = {M_top,M_high,M_low,M_bot}
        //      if T is 'Z', X, Y, Z will be ignored and position will be zeroed
        //      If T is 'S', X will indicate minimum stepper resolution in nm, Y will indicate maximum voltage of piezo, and Z will be reserved
        command_type cmd_type;
        int8_t x_top, x_high, x_low, x_bot;
        int8_t y_top, y_high, y_low, y_bot;
        int8_t z_top, z_high, z_low, z_bot;
        int32_t x, y, z;
        switch(*s) {
            case 'P':
                x_top = *(s+2);
                x_high = *(s+4);
                x_low = *(s+6);
                x_bot = *(s+8);
                y_top = *(s+10);
                y_high = *(s+12);
                y_low = *(s+14);
                y_bot = *(s+16);
                z_top = *(s+18);
                z_high = *(s+20);
                z_low = *(s+22);
                z_bot = *(s+24);
                cmd_type = movement;
                break;
            case 'Z':
                cmd_type = zero;
                break;
            case 'S':
                x_top = *(s+2);
                x_high = *(s+4);
                x_low = *(s+6);
                x_bot = *(s+8);
                y_top = *(s+10);
                y_high = *(s+12);
                y_low = *(s+14);
                y_bot = *(s+16);
                z_top = *(s+18);
                z_high = *(s+20);
                z_low = *(s+22);
                z_bot = *(s+24);
                cmd_type = resolution_set;
                break;
            default:
                cmd_type = ERROR;
                break;
        }

        x = (x_top << 24) | (x_high << 16) | (x_low << 8) | (x_bot << 0);
        y = (y_top << 24) | (y_high << 16) | (y_low << 8) | (y_bot << 0);
        z = (z_top << 24) | (z_high << 16) | (z_low << 8) | (z_bot << 0);

        switch(cmd_type) {
            case movement:
                int32_t x_stepper, y_stepper, z_stepper;
                int32_t x_piezo, y_piezo, z_piezo;
                x_stepper = x / min_stepper_res;
                x_piezo = x % min_stepper_res;
                y_stepper = y / min_stepper_res;
                y_piezo = y % min_stepper_res;
                z_stepper = z / min_stepper_res;
                z_piezo = z % min_stepper_res;
                step_pos_c.x = (double)x_stepper / 1000.0;
                step_pos_c.y = (double)y_stepper / 1000.0;
                step_pos_c.z = (double)z_stepper / 1000.0;
                piez_pos_c.x = (double)x_piezo / 1000.0;
                piez_pos_c.y = (double)y_piezo / 1000.0;
                piez_pos_c.z = (double)z_piezo / 1000.0;
            break;
            case zero:
                step_pos_c.x = 0.0;
                step_pos_c.y = 0.0;
                step_pos_c.z = 0.0;
                piez_pos_c.x = 0.0;
                piez_pos_c.y = 0.0;
                piez_pos_c.z = 0.0;
            break;
            case resolution_set:
                min_stepper_res = x;
                maximum_voltage = y;
            break;
            default:
            break;
        }
    }

    void process_stepper( char *s) {
        double temp;
        step_pos_c.x = temp;
        step_pos_c.y = temp;
        step_pos_c.z = temp;
    }

    void process_piezo(char *s) {
        double temp;
        piez_pos_c.x;
        piez_pos_c.y;
    }

    void init(uint64_t b) {
        SOFTWARE_SERIAL_PORT.begin(b);
        SOFTWARE_SERIAL_PORT.addMemoryForRead(rx_buf, sizeof(rx_buf));
        min_stepper_res = 10000;
    }

    void poll() {
        int8_t temp_buf[RX_BUF_SIZE];
        int8_t c;
        c = SOFTWARE_SERIAL_PORT.read();
        size_t i=0;
        while(c!=-1) {
            temp_buf[i] = c;
            if(c=='\n') {
                temp_buf[i] = '\0';
                process_gcode(&(temp_buf[0]));
                i=0;
                break;
            }
            c = SOFTWARE_SERIAL_PORT.read();
            ++i;
        }
    }

    void getStepperPositionCommand(StepperPositionCommand *s) {
        memcpy(s, &step_pos_c, sizeof(step_pos_c));
    }

    void getPiezoPositionCommand(PiezoPositionCommand *p) {
        memcpy(p, &piez_pos_c, sizeof(piez_pos_c));
    }
    void sendStepperPosition(StepperPosition *s) {
        // unimplemented as of now
    }
    void sendPiezoCommanded(PiezoCommanded *p) {
        // unimplemented as of now
    }
}