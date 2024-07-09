#pragma once

#include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"
#include "RmtDriver.h"
class MCDriver{
    public:
        gpio_num_t _pin;
        mcpwm_cmpr_handle_t _comparator;
        MCDriver(const gpio_num_t pin, uint32_t time_res, uint32_t time_per);

        void send(uint32_t pulse);
};