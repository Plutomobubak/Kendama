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
#include <driver/rmt_tx.h>
#include "esp_event.h"

namespace ProjGPIO
{
    class GPIOIn{
        public:
            gpio_num_t _pin;
            bool _active_low;
            
            void _init(const gpio_num_t pin, const bool active_low);
            void _init(const gpio_num_t pin);

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
            esp_err_t _init(const gpio_num_t pin);

            esp_err_t set(int level);
            esp_err_t toggle();
            int get();
    };
    class GPIOLed{
        public:
            enum strip_t { WS2812};
            typedef struct {
                uint8_t g;
                uint8_t b;
                uint8_t r;
            } rgb;
            gpio_num_t _pin;
            strip_t _type;
            int _size;

            void _init(const gpio_num_t pin, const int size, const strip_t type);

            esp_err_t set(rgb colour,int index);
            esp_err_t show();
            gpio_num_t get(int index);

            esp_err_t rmt_new_ws_encoder(rmt_encoder_handle_t *ret_encoder, int size);
        private:
            rmt_channel_handle_t tx_chan;
            rmt_encoder_handle_t ws_encoder;
            rmt_transmit_config_t tr_cfg;
            static size_t rmt_encode_ws(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state);
    };
}