#include "dht11_mbedos.h"
#include "Kernel.h"
#include "PinNamesTypes.h"
#include "ThisThread.h"
#include "Timer.h"
#include "mbed_wait_api.h"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <ratio>

#define HIGH 1
#define LOW 0

Dht11::Dht11(PinName pin)
{
    sensorPin = new DigitalInOut(pin);
    temperature = 0;
    humidity = 0;
    sensorPin->output();
    sensorPin->write(HIGH);
}

int Dht11::read()
{
    uint8_t data[5] = {0};
    Timer timer_object;
    timer_object.reset();
    uint16_t data_time = 0;
    uint8_t data_bit = 0;

    sensorPin->input();
    sensorPin->mode(OpenDrain);
    wait_us(1e6);

    //start signal
    sensorPin->output();
    //sensorPin->mode(OpenDrain);
    sensorPin->write(LOW);
    wait_us(20e3); //wait for 18ms
    //sensorPin->write(HIGH);
    //wait_us(30); //wait for 20-40us
    //sensorPin->write(LOW);

    //core_util_critical_section_enter();

    sensorPin->input();
    sensorPin->mode(OpenDrain);

    wait_us(80);
    while(sensorPin->read() == LOW);
    //if(sensorPin->read() == LOW) wait_us(80); //wait for 80us
    //wait_us(80); //wait for 80us
    wait_us(20);
    while(sensorPin->read() == HIGH);
    //if(sensorPin->read() == HIGH) wait_us(80); //wait for 80us


    
    
    for(int i = 0; i < 5; i++)
    {
        for(int j = 7; j >= 0; j--)
        {
            wait_us(50); //wait for 50us
            //while(sensorPin->read() == LOW); //wait while low

            timer_object.start();
            while(sensorPin->read() == HIGH) wait_us(10);
            //wait_us(40);
            //if(sensorPin) data_bit = 0;
            //else data_bit = 1;
            timer_object.stop();
            data_time = (uint16_t)chrono::duration_cast<chrono::microseconds>(timer_object.elapsed_time()).count();
            if(data_time < 50) data_bit = 0;
            else data_bit = 1;
            data[i] += data_bit << j;
            timer_object.reset();
        }
    }
    //core_util_critical_section_exit();

    temperature = data[3];
    humidity = data[1];

    sensorPin->output();
    sensorPin->write(HIGH);
    
    return 0;
    

}
//*/