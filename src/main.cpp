#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include <GPIO.h>
#include <Led.h>
#include <Adc.h>
#include <iostream>

using namespace std;

Led led(GPIO_NUM_21,8,1,LED_WS2812,DoubleBuffer);
GPIOIn button(GPIO_NUM_18, true);
Adc met(GPIO_NUM_2);

void run(int c,int delayMs)
{
    vTaskDelay(pdMS_TO_TICKS(delayMs));
    std::cout << met.get() << endl;
    for(int i=0;i<8;i++)
        led[i]=Rgb{static_cast<uint8_t>(rand()%256),static_cast<uint8_t>(rand()%256),static_cast<uint8_t>(rand()%256)};
    led.show();
}

void setup(){
    button.enableCallback(GPIO_INTR_NEGEDGE,[](){});
}
extern "C" void app_main(void)
{
    esp_event_loop_create_default();

    int c=0;

    setup();
    while (true)
    {
        run(c, 1000);
        c++;
    }    
}