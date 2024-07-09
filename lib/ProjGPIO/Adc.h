#pragma once

#include "driver/adc.h"
#include "esp_adc_cal.h"

class Adc{
    public:
        gpio_num_t _pin;
        bool _efuse;
        esp_adc_cal_characteristics_t _efuse_chars;
        Adc(const gpio_num_t pin);

        int get();
};