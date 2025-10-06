#include "software-serial.h"

namespace SoftwareSerial {
    static int8_t rx_buf[RX_BUF_SIZE];
    StepperPositionCommand step_pos_c;
    PiezoPositionCommand piez_pos_c;

    void process_gcode(int8_t *s) {
        // structure should be T X Y Z
        //      if T is 'P', X, Y, Z will be interpreted as a stepper pos command
        //      if T is 'S', X, Y will be interpreted as a piezo pos command, Z disregarded

        // how does string parsing work?
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