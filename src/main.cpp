#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include <GPIO.h>
#include <Led.h>
#include <Adc.h>
#include <Servo.h>
#include <DCMotor.h>
#include <iostream>

using namespace std;

Led led(GPIO_NUM_21,8,1,LED_WS2812,DoubleBuffer);
GPIOIn button(GPIO_NUM_18, true);
Adc met(GPIO_NUM_2);
Servo ser(GPIO_NUM_21,Default); 

void run(int c,int delayMs)
{
    vTaskDelay(pdMS_TO_TICKS(delayMs));
    for(int i=0;i<8;i++)
        led[i]=Rgb{static_cast<uint8_t>(met.get()%256),static_cast<uint8_t>(0%256),static_cast<uint8_t>(0%256)};
    led.show();
    robutekL.setSpeed(200);
    robutekL.moveDistance(100);
}

void setup(){
    button.enableCallback(GPIO_INTR_NEGEDGE,[](){});
    ser.set(0);
}
extern "C" void app_main(void)
{
    esp_event_loop_create_default();

    int c=0;

    setup();
    while (true)
    {
        run(c, 50);
        c++;
    }    
}