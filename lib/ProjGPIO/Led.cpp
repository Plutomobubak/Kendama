#include "Led.h"

IsrCore Led::_interruptCore = CoreCurrent;

Led*& IRAM_ATTR Led::ledForChannel(int channel) {
    static Led* table[CHANNEL_COUNT] = {};
    assert(channel < CHANNEL_COUNT);
    return table[channel];
}

esp_err_t Led::show(){
    esp_err_t err = startTransmission();
    swapBuffers();
    return err;
}
