#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <functional>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_event.h"

class GPIOIn{
    public:
        gpio_num_t _pin;
        bool _active_low;
        
        GPIOIn(const gpio_num_t pin, const bool active_low);
        GPIOIn(const gpio_num_t pin);

        int read(void);

        esp_err_t enableCallback(gpio_int_type_t interrupt_type, const std::function<void()>& callback);
        static void gpio_task_example(void* arg);
        static void IRAM_ATTR gpio_isr_callback(void* arg);
    private:
        static QueueHandle_t gpio_evt_queue;
        static std::function<void()> event_callback;
};
class GPIOOut{
    public:
        gpio_num_t _pin;
        GPIOOut(const gpio_num_t pin);

        esp_err_t set(int level);
        esp_err_t toggle();
        int get();
};