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

//Servo ser(GPIO_NUM_21,Default); 

void run(int c,int delayMs)
{
    vTaskDelay(pdMS_TO_TICKS(delayMs));
}

void setup(){
    robutekL.startTicker();
    robutekR.startTicker();
    robutekL.setSpeed(INT_MAX);
    robutekR.setSpeed(INT_MAX);
    robutekL.moveDistance(10);
    robutekR.moveDistance(10);
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