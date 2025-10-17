#include "piezo.h"
#include <cstddef>
#include <cstdlib>

namespace piezo_system {
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

    piezo_stack_t p;

    void init(uint16_t *c) {
        for(size_t i=0;i<PIEZO_STACK_NUM;++i) {
            p.piezos[i] = (piezo_t *)malloc(sizeof(piezo_system::piezo_t));
            p.piezos[i]->channel = c[i];
            p.piezos[i]->voltage = 0.0;
        }
    }
}