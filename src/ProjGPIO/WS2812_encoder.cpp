#include "ProjGPIO.h"

namespace ProjGPIO{

    typedef struct{
        rmt_encoder_t base;
        rmt_encoder_t *copy_encoder;
        rmt_encoder_t *bytes_encoder;
        rmt_symbol_word_t ws_reset_symbol;
        int size;
        int state;
    } rmt_ws_encoder_t;
    
    esp_err_t GPIOLed::rmt_new_ws_encoder(rmt_encoder_handle_t *ret_encoder, int size){
        esp_err_t status = ESP_OK;
        rmt_ws_encoder_t *ws_encoder = NULL;
        ws_encoder = (rmt_ws_encoder_t *)rmt_alloc_encoder_mem(sizeof(rmt_ws_encoder_t));
        ws_encoder->base.encode = ProjGPIO::GPIOLed::rmt_encode_ws;
        ws_encoder->size = size;
        rmt_copy_encoder_config_t cenc_cfg;
        status |= rmt_new_copy_encoder(&cenc_cfg,&ws_encoder->copy_encoder);
        //RESET_CODE
        ws_encoder->ws_reset_symbol.level0 = 1;
        ws_encoder->ws_reset_symbol.duration0 = 0;
        ws_encoder->ws_reset_symbol.level1 = 0;
        ws_encoder->ws_reset_symbol.duration1 = 800000 * 50 / 1000000;//freq 800kHz * 50 μs
        //bytes
        rmt_bytes_encoder_config_t bytes_enc_cfg;
        //bit 0
        bytes_enc_cfg.bit0.level0 = 1;
        bytes_enc_cfg.bit0.duration0 = 800000 * 0.35 / 1000000;//0.35 μs
        bytes_enc_cfg.bit0.level1 = 0;
        bytes_enc_cfg.bit0.duration1 = 800000 * 0.8 / 1000000;// 0.8 μs
        //bit 1
        bytes_enc_cfg.bit1.level0 = 1;
        bytes_enc_cfg.bit1.duration0 = 800000 * 0.7 / 1000000;// 0.7 μs
        bytes_enc_cfg.bit1.level1 = 0;
        bytes_enc_cfg.bit1.duration1 = 800000 * 0.6 / 1000000;// 0.6 μs

        status |= rmt_new_bytes_encoder(&bytes_enc_cfg,&ws_encoder->bytes_encoder);

        *ret_encoder = &ws_encoder->base;
        return status;
    }

    size_t GPIOLed::rmt_encode_ws(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state){
        rmt_ws_encoder_t *ws_encoder = __containerof(encoder, rmt_ws_encoder_t, base);
        rmt_encode_state_t session_state = RMT_ENCODING_RESET;
        rmt_encode_state_t state = RMT_ENCODING_RESET;
        size_t encoded_symbols = 0;
        rmt_encoder_handle_t copy_encoder = ws_encoder->copy_encoder;
        rmt_encoder_handle_t bytes_encoder = ws_encoder->bytes_encoder;
        int size = ws_encoder->size;
        ProjGPIO::GPIOLed::rgb *leds= (ProjGPIO::GPIOLed::rgb *)primary_data;
        //send poo
        if(state >= size){//Send RESET_CODE on end of led array
            encoded_symbols += copy_encoder->encode(copy_encoder, channel, &ws_encoder->ws_reset_symbol,sizeof(rmt_symbol_word_t), &session_state);
            if (session_state & RMT_ENCODING_COMPLETE) {
            ws_encoder->state = RMT_ENCODING_RESET; // back to the initial encoding session
            //state |= RMT_ENCODING_COMPLETE;
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            //state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space to put other encoding artifacts
        }
        }
        else{
            encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, &leds[state], sizeof(ProjGPIO::GPIOLed::rgb),&session_state);
            if (session_state & RMT_ENCODING_COMPLETE) {
                ws_encoder->state++; // we can only switch to next state when current encoder finished
            }
            if (session_state & RMT_ENCODING_MEM_FULL) {
                //state |= RMT_ENCODING_MEM_FULL;
                goto out; // yield if there's no free space to put other encoding artifacts
            }
        }
    out:
        *ret_state = state;
        return encoded_symbols;
    }
}