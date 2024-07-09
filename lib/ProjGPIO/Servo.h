#pragma once

#include <MCDriver.h>

struct ServoConfig{
    uint32_t min_pulse;
    uint32_t max_pulse;
    int32_t min_deg;
    int32_t max_deg;
};
static const ServoConfig Default = {1000,2000,-90,90};

class Servo{
    public:
        gpio_num_t _pin;
        ServoConfig _cfg;
        MCDriver *_driver;
        Servo(const gpio_num_t pin, const ServoConfig cfg);

        void set(int angle);
    private:
       uint32_t angle_to_pulse(int angle);
};