#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include <GPIO.h>
#include <Led.h>
#include <Adc.h>
#include <Servo.h>
#include <dcmotorl.h>
#include <simple_radio.h>
#include <iostream>
//#include <NVSL.h>

using namespace std;

Servo ser(GPIO_NUM_21,Default);
//NVSL nv(nullptr);


void run(int c,int delayMs)
{
    vTaskDelay(pdMS_TO_TICKS(delayMs));
    SimpleRadio.sendNumber(69);
}

void setup(){
    robutekL.startTicker();
    robutekR.startTicker();
    SimpleRadio.begin(0);
    SimpleRadio.setOnNumberCallback([](double val, PacketInfo info){
        cout << val << endl;
    });
   
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