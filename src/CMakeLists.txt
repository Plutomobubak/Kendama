
FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*) 
idf_component_register(SRCS ${app_sources}
                    REQUIRES driver esp_event bt nvs_flash esp_adc hal soc
                    INCLUDE_DIRS . ../include/ProjGPIO)
