#pragma once

#include <iostream>
#include <unordered_map>

#include "driver/ledc.h"

class Ledc {
        std::unordered_map<int, int> _usedTimers;
        std::unordered_map<int, int> _usedChannels;
    public:
        void configureTimer(int timerNum, int frequency, int resolution) {
            if (resolution < 1 || resolution >= LEDC_TIMER_BIT_MAX) {
                std::cerr << "Resolution must be between 1 and " + std::to_string(LEDC_TIMER_BIT_MAX - 1) <<std::endl;
            }
            if (frequency < 1) {
                std::cerr << "Frequency must be greater than 0" << std::endl;
            }

            ledc_timer_config_t ledc_timer = {
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .duty_resolution = static_cast<ledc_timer_bit_t>(resolution),
                .timer_num = static_cast<ledc_timer_t>(timerNum),
                .freq_hz = static_cast<uint32_t>(frequency),
                .clk_cfg = LEDC_AUTO_CLK,
                .deconfigure = false
            };
            esp_err_t err = ledc_timer_config(&ledc_timer);
            if (err != ESP_OK) {
                std::cerr << esp_err_to_name(err) << std::endl;
            }

            _usedTimers.insert({ timerNum, resolution });
        }

        void configureChannel(int channelNum, int gpioNum, int timerNum, int duty) {
            auto timer = _usedTimers.find(timerNum);
            if (timer == _usedTimers.end()) {
                std::cerr << "Timer not configured" << std::endl;
            }
            if (duty < 0 || duty > 1023) {
                std::cerr << "Duty must be between 0 and 1023" << std::endl;
            }

            duty = (1 << timer->second) * duty / 1023;

            ledc_channel_config_t ledc_channel = {
                .gpio_num = (gpioNum),
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .channel = static_cast<ledc_channel_t>(channelNum),
                .intr_type = LEDC_INTR_DISABLE,
                .timer_sel = static_cast<ledc_timer_t>(timerNum),
                .duty = static_cast<uint32_t>(duty),
                .hpoint = 0,
                .flags = { 0 }
            };
            esp_err_t err = ledc_channel_config(&ledc_channel);
            if (err != ESP_OK) {
                std::cerr << esp_err_to_name(err) << std::endl;
            }

            _usedChannels.insert({ channelNum, timerNum });
        }

        void setFrequency(int timerNum, int frequency) {
            if (_usedTimers.find(timerNum) == _usedTimers.end()) {
                std::cerr << "Timer not in use" << std::endl;
            }
            if (frequency < 1) {
                std::cerr << "Frequency must be greater than 0" << std::endl;
            }

            esp_err_t err = ledc_set_freq(LEDC_LOW_SPEED_MODE, static_cast<ledc_timer_t>(timerNum), frequency);
            if (err != ESP_OK) {
                std::cerr << esp_err_to_name(err) << std::endl;
            }
        }

        void setDuty(int channelNum, int duty) {
            auto channel = _usedChannels.find(channelNum);
            if (channel == _usedChannels.end()) {
                std::cerr << "Channel not in use" << std::endl;
            }
            auto timer = _usedTimers.find(channel->second);
            if (duty < 0 || duty > 1023) {
                std::cerr << "Duty must be between 0 and 1023" << std::endl;
            }

            duty = (1 << timer->second) * duty / 1023;

            esp_err_t err = ledc_set_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(channelNum), duty);
            if (err != ESP_OK) {
                std::cerr << esp_err_to_name(err) << std::endl;
            }

            err = ledc_update_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(channelNum));
            if (err != ESP_OK) {
                std::cerr << esp_err_to_name(err) << std::endl;
            }
        }

        void stopTimer(int timerNum) {
            bool channelInUse = false;
            for (auto channel : _usedChannels) {
                if (channel.second == timerNum) {
                    channelInUse = true;
                    break;
                }
            }
            if (channelInUse) {
                std::cerr << "Timer still in use by channel" << std::endl;
            }

            esp_err_t err = ledc_timer_rst(LEDC_LOW_SPEED_MODE, static_cast<ledc_timer_t>(timerNum));
            if (err != ESP_OK) {
                std::cerr << esp_err_to_name(err) << std::endl;
            }

            _usedTimers.erase(timerNum);
        }

        void stopChannel(int channelNum) {
            esp_err_t err = ledc_stop(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(channelNum), 0);
            if (err != ESP_OK) {
                std::cerr << esp_err_to_name(err) << std::endl;
            }

            _usedChannels.erase(channelNum);
        }

        ~Ledc() {
            for (auto channel : _usedChannels) {
                ledc_stop(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(channel.first), 0);
            }
            for (auto timer : _usedTimers) {
                ledc_timer_rst(LEDC_LOW_SPEED_MODE, static_cast<ledc_timer_t>(timer.first));
            }
        }
    };