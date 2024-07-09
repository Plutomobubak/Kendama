#include <Adc.h>

Adc::Adc(const gpio_num_t pin){
    _pin = pin;

    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    adc_oneshot_chan_cfg_t config;
    config.bitwidth = ADC_BITWIDTH_DEFAULT;
    config.atten = ADC_ATTEN_DB_12;

    adc_oneshot_new_unit(&init_config1, &_adc_handle);

    adc_oneshot_config_channel(_adc_handle, (adc_channel_t)(_pin-1), &config);
}
int Adc::get(){
    int adc_raw;
    adc_oneshot_read(_adc_handle, (adc_channel_t)(_pin-1), &adc_raw);
    return adc_raw;
}