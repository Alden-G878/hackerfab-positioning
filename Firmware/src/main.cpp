#include <Arduino.h>
#include "encoder.h"
#include "piezo.h"
#include "software-serial.h"
#include "stepper.h"

// encoder defines
#define ENC_PL_0 33
#define ENC_PR_0 34
#define ENC_PL_1 35
#define ENC_PR_1 36
#define ENC_PL_2 37
#define ENC_PR_2 38
#define ENC_STEPS_PER_REV 4000
#define ENC_UM_PER_REV 500
#define ENC_CCW_POS_0 true
#define ENC_CCW_POS_1 true
#define ENC_CCW_POS_2 true

// software serial defines
#define SOFTWARE_SERIAL_BAUD 115200

// stepper step_dir defines
#define STEPPER_SD_STEPS_PER_REV -1
#define STEPPER_SD_STEP_PIN_X 22
#define STEPPER_SD_DIR_PIN_X 23
#define STEPPER_SD_STEP_PIN_Y 20
#define STEPPER_SD_DIR_PIN_Y 21
#define STEPPER_SD_STEP_PIN_Z 18
#define STEPPER_SD_DIR_PIN_Z 19
#define STEPPER_SD_CCW_POS_X true
#define STEPPER_SD_CCW_POS_Y true
#define STEPPER_SD_CCW_POS_Z true

// stepper positioning defines
#define STEPPER_POS_MICROSTEP_MULT 8
#define STEPPER_POS_STEPS_PER_REV 200 * STEPPER_POS_MICROSTEP_MULT
#define STEPPER_POS_REV_PER_UM -1
#define STEPPER_POS_CCW_POS true

// piezo defines
#define PIEZO_RST_PIN 9
#define PIEZO_SYNC_PIN 8



void setup() {
    // change encoders to be X, Y, Z
    enc::init(ENC_STEPS_PER_REV, ENC_UM_PER_REV, ENC_CCW_POS_0, ENC_CCW_POS_1, ENC_CCW_POS_2, ENC_PL_0, ENC_PL_1, ENC_PL_2, ENC_PR_0, ENC_PR_1, ENC_PR_2);
    enc::reset_x();
    enc::reset_y();
    enc::reset_z();
    
    uint16_t c[PIEZO_GROUP_NUM];
    /*
        Need to assign each piezo to a channel and write down which channel is which piezo
    */
    piezo_system::init(c);

    SoftwareSerial::init(SOFTWARE_SERIAL_BAUD);

    step_dir::init(STEPPER_SD_STEPS_PER_REV, STEPPER_SD_STEP_PIN_X, STEPPER_SD_DIR_PIN_X, STEPPER_SD_STEP_PIN_Y,STEPPER_SD_DIR_PIN_Y,STEPPER_SD_STEP_PIN_Z,STEPPER_SD_DIR_PIN_Z);
    positioning::init(STEPPER_POS_STEPS_PER_REV, STEPPER_POS_REV_PER_UM, STEPPER_POS_CCW_POS);
}

void loop() {
    // poll software serial
    SoftwareSerial::poll();

    // get stepper and piezo position commands
    SoftwareSerial::StepperPositionCommand stepper_command;
    SoftwareSerial::PiezoPositionCommand piezo_command;

    SoftwareSerial::getStepperPositionCommand(&stepper_command);
    SoftwareSerial::getPiezoPositionCommand(&piezo_command);

    // command stepper
    // get number of steps
    double steps_x = enc::get_pos_um(enc::get_pos_steps_x());
    double steps_y = enc::get_pos_um(enc::get_pos_steps_y());
    double steps_z = enc::get_pos_um(enc::get_pos_steps_z());
    // get commanded steps
    double commanded_steps_x = stepper_command.x;
    double commanded_steps_y = stepper_command.y;
    double commanded_steps_z = stepper_command.z;
    // find difference
    double delta_x, delta_y, delta_z;
    delta_x = commanded_steps_x - steps_x;
    delta_y = commanded_steps_y - steps_y;
    delta_z = commanded_steps_z - steps_z;
    // command deltas
    // improvement: bring forward by a little bit, then back
    step_dir::move(step_dir::motor::x,delta_x,STEPPER_SD_CCW_POS_X);
    step_dir::move(step_dir::motor::y,delta_y,STEPPER_SD_CCW_POS_Y);
    step_dir::move(step_dir::motor::z,delta_z,STEPPER_SD_CCW_POS_Z);
    // confirm distance
    double new_steps_x = enc::get_pos_um(enc::get_pos_steps_x());
    double new_steps_y = enc::get_pos_um(enc::get_pos_steps_y());
    double new_steps_z = enc::get_pos_um(enc::get_pos_steps_z());
    // compare to commanded
    double error_x, error_y, error_z;
    error_x = commanded_steps_x - new_steps_x;
    error_y = commanded_steps_y - new_steps_y;
    error_z = commanded_steps_z - new_steps_z;
    Serial.printf("Stepper x error, um: %f\n",error_x);
    Serial.printf("Stepper y error, um: %f\n",error_y);
    Serial.printf("Stepper z error, um: %f\n",error_z);

    // command piezo
    piezo_system::p->piezos[0]->voltage = piezo_system::um_to_voltage(piezo_command.x, piezo_system::p->piezos[0]);
    piezo_system::p->piezos[1]->voltage = piezo_system::um_to_voltage(piezo_command.y, piezo_system::p->piezos[1]);
    piezo_system::p->piezos[2]->voltage = piezo_system::um_to_voltage(piezo_command.z, piezo_system::p->piezos[2]);
    for(size_t i=0;i<PIEZO_GROUP_NUM;++i) {
        piezo_system::command_voltage(i);
    }
}