#pragma once

#include "ProjGPIO.h"

class Test final
    {
        public: void run(int c,int delayMs);
        void setup(void);

        ProjGPIO::GPIOLed led{GPIO_NUM_21};
        ProjGPIO::GPIOIn button{GPIO_NUM_18, true};
    };