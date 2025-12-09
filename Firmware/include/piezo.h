#pragma once
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <Arduino.h>
#include <SPI.h>
#define PIEZO_SERIAL_PORT Serial2
#define PIEZO_GROUP_NUM 3
#define PIEZO_CLOCKSPEED 1000000
#define PIEZO_MAX_VOLTAGE 100.0

namespace piezo_system {
    enum piezo_id : uint8_t{
        x,
        y,
        z
    };
    struct piezo {
        double voltage;
        int32_t displacment; // nm
        uint16_t channel;
        piezo_id id;
    };
    struct piezo_group
    {
        struct piezo *piezos[PIEZO_GROUP_NUM];
    };

    //struct piezo_group *p;

    void init(uint16_t *c, uint8_t rst_b_, uint8_t sync_b_); // may take as input configured voltage range

    void command_voltage(uint16_t c); // read docs of DAC to figure out 

    void reset_dac(); // sets voltages of all channels to zero

    double um_to_voltage(double um, piezo *p); // translate int32_t of nm displacement into voltage level

    piezo_group *get_p();
}