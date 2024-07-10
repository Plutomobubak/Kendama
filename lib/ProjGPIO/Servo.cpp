#include "Servo.h"

uint32_t Servo::angle_to_pulse(int angle)
{
    return (angle - _cfg.min_deg) * (_cfg.max_pulse - _cfg.min_pulse) / (_cfg.max_deg - _cfg.min_deg) + _cfg.min_pulse;
}
uint32_t Servo::angle_to_duty(int angle){
    return angle_to_pulse(angle)/20 *1023;
}
Servo::Servo(const gpio_num_t pin, const ServoConfig cfg){
    _pin = pin;
    _cfg = cfg;

    _driver.configureTimer(0, _cfg.freq, _cfg.res);
    _driver.configureChannel(0, _pin, 0, 1023);
}
void Servo::set(int angle){
    _driver.setDuty(0, angle_to_duty(angle));;
}