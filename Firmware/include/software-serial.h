#pragma once
#define SOFTWARE_SERIAL_PORT Serial1;

namespace SoftwareSerial {
    struct StepperPositionCommand{
        double x;
        double y;
        double z;
    };
    struct PiezoPositionCommand {
        double x;
        double y;
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

    void init();

    void poll(); //read from message buffer & process

    void getStepperPositionCommand(StepperPositionCommand *s);
    void getPiezoPositionCommand(PiezoPositionCommand *p);
    void sendStepperPosition(StepperPosition *s);
    void sendPiezoCommanded(PiezoCommanded *p);
}