#include "Servo.h"

uint32_t Servo::angle_to_pulse(int angle)
{
    return (angle - _cfg.min_deg) * (_cfg.max_pulse - _cfg.min_pulse) / (_cfg.max_deg - _cfg.min_deg) + _cfg.min_pulse;
}

Servo::Servo(const gpio_num_t pin, const ServoConfig cfg){
    _pin = pin;
    _cfg = cfg;

    _driver = new MCDriver(_pin,1000000,20000);
}
void Servo::set(int angle){
    (*_driver).send(angle_to_pulse(angle));
}