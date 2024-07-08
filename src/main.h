#pragma once

#include "ProjGPIO.h"

class Test final
    {
        public: void run(int c,int delayMs);
        void setup(void);

        ProjGPIO::GPIOLed led;
        ProjGPIO::GPIOIn button;
    };