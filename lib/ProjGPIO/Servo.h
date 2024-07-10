#pragma once

#include <Ledc.h>

struct ServoConfig{
    //pulse in milliseconds
    uint32_t min_pulse;
    uint32_t max_pulse;
    int32_t min_deg;
    int32_t max_deg;
    uint32_t freq;
    uint32_t res;
};
static const ServoConfig Default = {1,2,-90,90,50,12};

class Servo{
    public:
        gpio_num_t _pin;
        ServoConfig _cfg;
        Ledc _driver;
        Servo(const gpio_num_t pin, const ServoConfig cfg);

        void set(int angle);
    private:
       uint32_t angle_to_pulse(int angle);
       uint32_t angle_to_duty(int angle);
};