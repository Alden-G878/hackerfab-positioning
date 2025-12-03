#pragma once

#include <cstdint>
#include <Arduino.h>

#define SOFTWARE_SERIAL_PORT Serial1
#define RX_BUF_SIZE 1024 // bytes

namespace SoftwareSerial {
    struct StepperPositionCommand{
        double x;
        double y;
        double z;
    };
    struct PiezoPositionCommand {
        double x;
        double y;
        double z;
    };
    struct StepperPosition{
        double x;
        double y;
        double z;
    };
    struct PiezoCommanded{
        double x;
        double y;
    };

    enum command_type : uint8_t {
        movement,
        zero,
        resolution_set,
        ERROR
    };

    void init(uint64_t b);

    void poll(); //read from message buffer & process

    void getStepperPositionCommand(StepperPositionCommand *s);
    void getPiezoPositionCommand(PiezoPositionCommand *p);
    void sendStepperPosition(StepperPosition *s);
    void sendPiezoCommanded(PiezoCommanded *p);
}