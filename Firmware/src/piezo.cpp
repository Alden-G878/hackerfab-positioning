#include "piezo.h"

namespace piezo_system {
    uint8_t rst_b, sclk, sync_b, din;

    void reset_dac() {
        digitalWrite(rst_b, HIGH);
        delayMicroseconds(1);
        digitalWrite(rst_b, LOW);
        delayMicroseconds(1);
        digitalWrite(rst_b, HIGH);
    }

    void init(uint16_t *c, uint8_t rst_b_, uint8_t sync_b_) {
        rst_b = rst_b_;
        sync_b = sync_b_;
        pinMode(rst_b, OUTPUT);
        pinMode(sync_b, OUTPUT);
        reset_dac();
        p = (piezo_system::piezo_group *)malloc(sizeof(p));
        for(size_t i=0;i<PIEZO_GROUP_NUM;++i) {
            p->piezos[i] = (struct piezo *)malloc(sizeof(p->piezos[i]));
            p->piezos[i]->channel = c[i];
            p->piezos[i]->voltage = 0.0;
            p->piezos[i]->displacment = 0;
        }
        SPI.begin();
    }

    double um_to_voltage(double um, struct piezo *p) {
        // follow voltage curve on spec sheet of piezos
    }

    uint16_t remap(double v) {
        // remap 0 <= v <= 250 to 0 <= res[int] <= {14{1'b1}}
        double v_frac = v / PIEZO_MAX_VOLTAGE;
        uint16_t int_max = 0x3FFF;
        return (uint16_t)(v_frac * int_max);
    }

    void command_voltage(uint16_t select) { // need to verify clock only runs when transferring data
        struct piezo *piezo = p->piezos[select];

        uint8_t channel = piezo->channel;
        uint16_t command = remap(piezo->voltage);

        uint32_t data = ((uint32_t)(channel << 3) << 16) | (((uint32_t)command) << 13);

        // bring sync high
        digitalWrite(sync_b, HIGH);
        // delay 1 microsecond
        delayMicroseconds(1);
        // bring sync low
        digitalWrite(sync_b, LOW);
        // delay 1 microsecond
        delayMicroseconds(1);
        // start SPI transaction
        SPI.beginTransaction(SPISettings(PIEZO_CLOCKSPEED, MSBFIRST, SPI_MODE1)); // from example waveform, default state of clock is low, data sampled on falling edge
        // bring sync high & delay a microsecond
        digitalWrite(sync_b, HIGH);
        delayMicroseconds(1);
        // transfer 19 bits of data
        SPI.transfer32(data);
        // end SPI transaction
        SPI.endTransaction();
        // delay a microsecond
        delayMicroseconds(1);
    }
}