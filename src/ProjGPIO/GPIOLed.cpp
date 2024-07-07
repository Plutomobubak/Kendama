#include "ProjGPIO.h"

namespace ProjGPIO
{
    //constructor
    esp_err_t GPIOLed::_init(const gpio_num_t pin/*, const int size/*, const strip_t type*/){
        
        esp_err_t status{ESP_OK};

        _pin = pin;
        //_size = size;
        //_type = type;

        tx_chan = NULL;
        rmt_tx_channel_config_t cfg;
        cfg.clk_src = RMT_CLK_SRC_DEFAULT;
        cfg.gpio_num = pin;
        cfg.mem_block_symbols = (gpio_num_t)/*size*/1*(3/4)+0.5;
        cfg.resolution_hz = 800000;
        cfg.trans_queue_depth = 4;          
        cfg.flags.invert_out = false;        
        cfg.flags.with_dma = false;

        status |= rmt_new_tx_channel(&cfg, &tx_chan);

        rmt_carrier_config_t car_cfg;
        car_cfg.duty_cycle = 0.33;
        car_cfg.frequency_hz = 800000;

        status |= rmt_apply_carrier(tx_chan,&car_cfg);

        ws_encoder = NULL;
        status |= rmt_new_ws_encoder(&ws_encoder,/*size*/1);
        status |= rmt_enable(tx_chan);

        tr_cfg.loop_count = 0;

        rgb leds[/*size*/1];
        leds[0].b = 255;
        leds[0].g = 255;
        leds[0].r = 255;

        status |= rmt_transmit(tx_chan,ws_encoder,leds,sizeof(leds),&tr_cfg);
        return status;
    }

    esp_err_t GPIOLed::show(){
        rgb leds[_size];
        leds[0].b = 255;
        leds[0].g = 255;
        leds[0].r = 255;

        return rmt_transmit(tx_chan,ws_encoder,leds,sizeof(leds),&tr_cfg);
    }
}