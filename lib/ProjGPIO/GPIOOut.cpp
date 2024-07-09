#include "GPIO.h"


GPIOOut::GPIOOut(const gpio_num_t pin)
{
    _pin = pin;

    gpio_config_t cfg;
    cfg.pin_bit_mask = 1ULL << pin;
    cfg.mode = GPIO_MODE_OUTPUT;

    assert(gpio_config(&cfg));
}
esp_err_t GPIOOut::set(int level){
    return gpio_set_level(_pin,level);
}
esp_err_t GPIOOut::toggle(){
    return gpio_set_level(_pin,!gpio_get_level(_pin));
}
int GPIOOut::get(){
    return gpio_get_level(_pin);
}
