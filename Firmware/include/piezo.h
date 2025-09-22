#pragma once
#define PIEZO_SERIAL_PORT Serial2

namespace pizeo_system {
    void init(); // may take as input configured voltage range

    void command_voltage(double v); // read docs of DAC to figure out 

    // not sure what else we need, need to read DAC docs first
}