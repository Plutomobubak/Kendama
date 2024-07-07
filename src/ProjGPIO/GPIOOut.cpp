#include "ProjGPIO.h"

namespace ProjGPIO
{
    //Constructor for input pin
    esp_err_t GPIOOut::_init(const gpio_num_t pin)
    {
        esp_err_t status{ESP_OK};

        _pin = pin;

        gpio_config_t cfg;
        cfg.pin_bit_mask = 1ULL << pin;
        cfg.mode = GPIO_MODE_OUTPUT;

        status |= gpio_config(&cfg);

        return status;
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
}