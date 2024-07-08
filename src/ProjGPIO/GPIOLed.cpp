#include "ProjGPIO.h"

namespace ProjGPIO
{
    //constructor
    void GPIOLed::_init(const gpio_num_t pin, const int size, const strip_t type){

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

        rmt_new_tx_channel(&cfg, &tx_chan);

        rmt_carrier_config_t car_cfg;
        car_cfg.duty_cycle = 0.33;
        car_cfg.frequency_hz = 800000;

        rmt_apply_carrier(tx_chan,&car_cfg);

        ws_encoder = NULL;
        rmt_new_ws_encoder(&ws_encoder,/*size*/1);
        rmt_enable(tx_chan);

        tr_cfg.loop_count = 0;

        rgb leds[/*size*/1];
        leds[0].b = 255;
        leds[0].g = 255;
        leds[0].r = 255;

        rmt_transmit(tx_chan,ws_encoder,leds,sizeof(leds),&tr_cfg);
    }

    esp_err_t GPIOLed::show(){
        rgb leds[_size];
        leds[0].b = 255;
        leds[0].g = 255;
        leds[0].r = 255;

        return rmt_transmit(tx_chan,ws_encoder,leds,sizeof(leds),&tr_cfg);
    }
}