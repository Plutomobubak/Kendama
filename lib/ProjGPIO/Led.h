#pragma once

#include <cassert>
#include <cstring>
#include <memory>

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_intr_alloc.h>
#include <esp_ipc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <RmtDriver.h>

enum BufferType { SingleBuffer = 0, DoubleBuffer };

enum IsrCore { CoreFirst = 0, CoreSecond = 1, CoreCurrent = 2 };

using LedType = TimingParams;

static const LedType LED_WS2812 = { 350, 700, 800, 600, 50000 };
// longer reset time because https://blog.adafruit.com/2017/05/03/psa-the-ws2812b-rgb-led-has-been-revised-will-require-code-tweak/
static const LedType LED_WS2812B = { 400, 800, 850, 450, 300000 }; // universal
static const LedType LED_WS2812B_NEWVARIANT = { 200, 750, 750, 200, 300000 };
static const LedType LED_WS2812B_OLDVARIANT = { 400, 800, 850, 450, 50000 };
// This is timing from datasheet, but does not seem to actually work - try LED_WS2812B
static const LedType LED_WS2812C = { 250, 550, 550, 250, 280000 };
static const LedType LED_SK6812 = { 300, 600, 900, 600, 80000 };
static const LedType LED_WS2813 = { 350, 800, 350, 350, 300000 };

class Led{
        public:
            friend class RmtDriver;

            gpio_num_t _pin;
            LedType _type;

            Led(const gpio_num_t pin, const int size,const int channel, const LedType type, BufferType doubleBuffer = DoubleBuffer,
                IsrCore isrCore = CoreCurrent)
                : _finishedFlag(xSemaphoreCreateBinary())
                , _driver(type, size, pin, channel, _finishedFlag)
                , _channel(channel)
                , _size(size)
                , _firstBuffer(new Rgb[size])
                , _secondBuffer(doubleBuffer ? new Rgb[size] : nullptr) {
                assert(channel >= 0 && channel < CHANNEL_COUNT);
                assert(ledForChannel(channel) == nullptr);

                xSemaphoreGive(_finishedFlag);

                _driver.init();

            #if !defined(SOC_CPU_CORES_NUM) || SOC_CPU_CORES_NUM > 1
                    if (!anyAlive() && isrCore != CoreCurrent) {
                        _interruptCore = isrCore;
                        ESP_ERROR_CHECK(esp_ipc_call_blocking(isrCore, registerInterrupt, (void*)this));
                    } else
            #endif
                    {
                        registerInterrupt((void*)this);
                    }

                    ledForChannel(channel) = this;
                }
                ~Led(){
                    ledForChannel(_channel) = nullptr;
            #if !defined(SOC_CPU_CORES_NUM) || SOC_CPU_CORES_NUM > 1
                    if (!anyAlive() && _interruptCore != CoreCurrent) {
                        ESP_ERROR_CHECK(esp_ipc_call_blocking(_interruptCore, unregisterInterrupt, (void*)this));
                    } else
            #endif
                    {
                        unregisterInterrupt((void*)this);
                    }
                    vSemaphoreDelete(_finishedFlag);
                }

            int size() const { return _size; }
            int channel() const { return _channel; }

            Rgb& operator[](int idx) { return _firstBuffer[idx]; }

            const Rgb& operator[](int idx) const { return _firstBuffer[idx]; }
            esp_err_t show();
            
            gpio_num_t get(int index);

            static IsrCore _interruptCore;//was private but idk how ownership works
        private:
            
            static void registerInterrupt(void* selfVoid) {
                auto* self = (Led*)selfVoid;
                ESP_ERROR_CHECK(self->_driver.registerIsr(!anyAlive()));
            }

            static void unregisterInterrupt(void* selfVoid) {
                auto* self = (Led*)selfVoid;
                ESP_ERROR_CHECK(self->_driver.unregisterIsr());
            }

            static Led*& IRAM_ATTR ledForChannel(int channel);

            static bool anyAlive() {
                for (int i = 0; i != CHANNEL_COUNT; i++)
                    if (ledForChannel(i) != nullptr)
                        return true;
                return false;
            }

            void swapBuffers() {
                if (_secondBuffer)
                    _firstBuffer.swap(_secondBuffer);
            }

            esp_err_t startTransmission() {
                // Invalid use of the library, you must wait() fir previous frame to get processed first
                if (xSemaphoreTake(_finishedFlag, 0) != pdTRUE)
                    abort();

                auto err = _driver.transmit(_firstBuffer.get());
                if (err != ESP_OK) {
                    return err;
                }

                return ESP_OK;
            }

            SemaphoreHandle_t _finishedFlag;
            RmtDriver _driver;
            int _channel;
            int _size;
            std::unique_ptr<Rgb[]> _firstBuffer;
            std::unique_ptr<Rgb[]> _secondBuffer;
};