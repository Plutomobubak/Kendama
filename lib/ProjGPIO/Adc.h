#pragma once

#include "esp_adc/adc_oneshot.h"
#include "driver/gpio.h"

class Adc{
    public:
        gpio_num_t _pin;
        adc_oneshot_unit_handle_t _adc_handle;

        Adc(const gpio_num_t pin);
        //returns 0 to 4095
        int get();
};