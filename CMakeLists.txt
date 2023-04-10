cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(rx_test)

pico_sdk_init()

add_executable(rx_test
        rx_test.cpp
)

target_link_libraries(rx_test pico_stdlib)

pico_enable_stdio_usb(rx_test 1)
pico_enable_stdio_uart(rx_test 0)

pico_add_extra_outputs(rx_test)