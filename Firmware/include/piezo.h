#pragma once
#include <cstdint>
#define PIEZO_SERIAL_PORT Serial2
#define PIEZO_STACK_NUM 8

namespace pizeo_system {
    struct piezo {
        double voltage;
        uint16_t channel;
    };
    typedef struct piezo piezo_t;
    struct piezo_stack
    {
        piezo_t *piezos[PIEZO_STACK_NUM];
    };
    typedef struct piezo_stack piezo_stack_t;

    void init(uint16_t *c); // may take as input configured voltage range

    void command_voltage(double v); // read docs of DAC to figure out 

    // not sure what else we need, need to read DAC docs first
}