set(SOURCES main.cpp ProjGPIO/GPIOIn.cpp ProjGPIO/GPIOOut.cpp ProjGPIO/GPIOLed.cpp ProjGPIO/WS2812_encoder.cpp)
FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*) 
idf_component_register(SRCS ${SOURCES}
                    REQUIRES driver esp_event
                    INCLUDE_DIRS . ../include/ProjGPIO)
