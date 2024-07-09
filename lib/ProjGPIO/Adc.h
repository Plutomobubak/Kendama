#pragma once

#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc_cal.h"

class Adc{
    public:
        gpio_num_t _pin;
        adc_oneshot_unit_handle_t _adc_handle;

        Adc(const gpio_num_t pin);
        //returns 0 to 4095
        int get();
};