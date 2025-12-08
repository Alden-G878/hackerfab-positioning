#include "piezo.h"

namespace piezo_system {
    uint8_t rst_b, sclk, sync_b, din;

    double displacment_data[100] = {0.056,0.181,0.3,0.413,0.522,0.627,0.728,0.827,0.925,1.021,1.118,1.215,1.313,1.413,1.517,1.623,1.735,1.851,1.973,2.101,2.234,2.37,2.506,2.641,2.773,2.901,3.026,3.151,3.277,3.409,3.546,3.686,3.823,3.953,4.07,4.174,4.27,4.369,4.477,4.605,4.757,4.913,5.048,5.145,5.217,5.287,5.38,5.511,5.666,5.822,5.961,6.083,6.194,6.299,6.401,6.505,6.611,6.717,6.822,6.926,7.027,7.127,7.228,7.33,7.436,7.545,7.657,7.77,7.881,7.988,8.089,8.186,8.279,8.369,8.458,8.546,8.634,8.722,8.811,8.9,8.989,9.079,9.168,9.255,9.341,9.425,9.506,9.586,9.665,9.744,9.823,9.904,9.992,10.086,10.185,10.282,10.373,10.453,10.517};
    double voltage_data[100] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99};

    double double_abs(double x) {
        return (x>0)?x:-x;
    }

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

    size_t find_nearest_index(double um) {
        double min_delta = double_abs(um - displacment_data[0]);
        size_t min_delta_index = 0;
        for(size_t cur_delta_index = 0;cur_delta_index<100;++cur_delta_index) {
            double cur_delta = double_abs(um - displacment_data[cur_delta_index]);
            if(cur_delta < min_delta) {
                min_delta_index = cur_delta_index;
                min_delta = cur_delta;
            }
        }
    }

    double um_to_voltage(double um, struct piezo *p) {
        // follow voltage curve on spec sheet of piezos
        return voltage_data[find_nearest_index(um)];
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