#include "GPIO.h"

GPIOIn::GPIOIn(const gpio_num_t pin, const bool active_low)
{
    _pin = pin;
    _active_low = active_low;

    gpio_config_t cfg;
    cfg.pin_bit_mask = 1ULL << pin;
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&cfg);
}
GPIOIn::GPIOIn(const gpio_num_t pin){
    GPIOIn(pin, true);
}

int GPIOIn::read(){
    return _active_low? !gpio_get_level(_pin):gpio_get_level(_pin);
}

//variables for callback inits
QueueHandle_t GPIOIn::gpio_evt_queue = NULL;
std::function<void()> GPIOIn::event_callback = nullptr;
//template for callback
void GPIOIn::gpio_task_example(void* arg)
{
    uint32_t io_num;
    TickType_t last_interrupt_time = 0; // Variable to track the last interrupt time

    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            // Calculate time since the last interrupt
            TickType_t current_time = xTaskGetTickCount();
            TickType_t time_since_last_interrupt = current_time - last_interrupt_time;

            // Check if the time since the last interrupt is greater than the debounce delay
            if (time_since_last_interrupt >= pdMS_TO_TICKS(50)) {
                if (event_callback) {
                    event_callback();
                }
                // Update the last interrupt time
                last_interrupt_time = current_time;
            }
        }
    }
}
/**
 * @brief Enables callback for GPIOIn
 * 
 * @param interrupt_type Action to activate callback
 *              0: GPIO_INTR_DISABLE
 *              1: GPIO_INTR_POSEDGE - CHANGE TO POS (PRESS)
 *              2: GPIO_INTR_NEGEDGE - CHANGE TO NEG (RELEASE)
 *              3: GPIO_INTR_ANYEDGE - ANY CHANGE (PRESS or RELEASE)
 *              4: GPIO_INTR_LOW_LEVEL -  NEG (STAY RELEASED)
 *              5: GPIO_INTR_HIGH_LEVEL - POS (HOLD)
 *              6: GPIO_INTR_MAX (idk)
 * @param callback void() function to call on action, lambda [](){
 *  *code*
 * }
 */
esp_err_t GPIOIn::enableCallback(gpio_int_type_t interrupt_type,const  std::function<void()>& callback) {
    gpio_set_intr_type(_pin, interrupt_type);
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    event_callback = callback;
    // Start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(_pin, gpio_isr_callback, (void*) _pin);

    return ESP_OK; // Assuming success
}
//isr callback
void IRAM_ATTR GPIOIn::gpio_isr_callback(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}