#include "DigitalInOut.h"
#include "InterfaceDigitalInOut.h"
#include "PinNameAliases.h"
#include "ThisThread.h"
#include "mbed.h"
#include "dht11_mbedos.h"
#include <cstdio>

DigitalInOut* dht11_pin = new DigitalInOut(D2);

Dht11* dht11_sensor = new Dht11;



// main() runs in its own thread in the OS
int main()
{
    dht11_sensor->attach(dht11_pin);

    while (true) {
        dht11_sensor->read();
        printf("%d, %d\n", dht11_sensor->humidity, dht11_sensor->temperature);
        ThisThread::sleep_for(500);
    }
}

