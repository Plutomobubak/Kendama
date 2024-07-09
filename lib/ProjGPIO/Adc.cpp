#include <Adc.h>

Adc::Adc(const gpio_num_t pin){
    _pin = pin;
    _efuse = esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_DEFAULT_VREF)==ESP_OK;
    if(_efuse){
        esp_adc_cal_characterize(_pin<11?ADC_UNIT_1:ADC_UNIT_2, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, &_efuse_chars);
    }
    if(_pin<11){
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten((adc1_channel_t)(_pin-1),ADC_ATTEN_DB_12);
    }
    else if(_pin<21){
        adc2_config_channel_atten((adc2_channel_t)(_pin-1),ADC_ATTEN_DB_12);
    } 
}
int Adc::get(){
    if(_efuse)
        return esp_adc_cal_raw_to_voltage(adc1_get_raw((adc1_channel_t)(_pin-1)), &_efuse_chars);
    else if(_pin<11)
        return adc1_get_raw((adc1_channel_t)(_pin-1))/4095*2500;
    else if(_pin<21){
        int raw_out;
        adc2_get_raw((adc2_channel_t)(_pin-1),ADC_WIDTH_BIT_12, &raw_out);
        return raw_out;
    }
}