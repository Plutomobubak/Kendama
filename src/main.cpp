#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include "main.h"
#include <iostream>

using namespace std;

Test App;

void Test::run(int c,int delayMs)
{
    std::cout << button.read() << endl;
    vTaskDelay(pdMS_TO_TICKS(delayMs));
    led.show();
}

void Test::setup(){
    button.enableCallback(GPIO_INTR_NEGEDGE,[](){});
}
extern "C" void app_main(void)
{
    esp_event_loop_create_default();

    int c=0;

    App.setup();
    while (true)
    {
        App.run(c, 1000);
        c++;
    }    
}