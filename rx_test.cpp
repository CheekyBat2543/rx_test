#include <stdio.h>
#include "pico/stdlib.h"
#include <vector>

#define HIGH 1
#define LOW 0
#define LED_GREEN 25
#define FREQUENCY 25
#define HIGH_PIN 23
#define RX_PIN 14
#define RX_CLOCK_PIN 15
#define CHAR_BUFFER 128

class Rx { // class for handling data transfer
public:
    Rx(const uint rxpin, const uint rxclock){ // class constructor
        bit_position = 0; 
        byte_received = 0;
        data.reserve(15);
        rx_pin = rxpin;
        rx_clock = rxclock;
        gpio_init(rx_pin);
        gpio_init(rx_clock);
        gpio_init(LED_GREEN);
        gpio_set_dir(rx_pin, GPIO_IN);
        gpio_set_dir(rx_clock, GPIO_IN);
        gpio_set_dir(LED_GREEN, GPIO_OUT);
        gpio_pull_down(rx_pin);
        gpio_pull_down(rx_clock);
    }
    void getBit(void){
        bool rx_bit = gpio_get(rx_pin);
        if(bit_position == 8){
            bit_position = 0;
            data.push_back(byte_received);
            printf("<==> %c\n", byte_received);
            byte_received = 0;
        }
        if(rx_bit){
            byte_received |=  (0x80 >> bit_position);
            gpio_put(LED_GREEN, HIGH);
            printf("%u", rx_bit);
        }
        bit_position++;
        return;
    }
   ~Rx(void){
        printf("Class Destroyed.\n");
    }
    std::vector<char> data;
    uint rx_pin;
    uint rx_clock;
private:
    uint8_t bit_position;
    char byte_received; 
    bool clearBuffer(void){
       /*  delete data; */
        return true;
    }
};

Rx rx(RX_PIN, RX_CLOCK_PIN);

void rx_callback(uint rx_clock, uint32_t event){
    rx.getBit();
}

int main(){

    stdio_init_all();
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_irq_enabled_with_callback(rx.rx_clock, 0x08, 1, rx_callback);
    
/*-------------------------------------------*/
    
    while(1){

        sleep_ms(10000);
        printf("%s", rx.data);
    }
}
