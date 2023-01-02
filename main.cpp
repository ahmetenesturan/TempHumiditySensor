
#include "DigitalInOut.h"
#include "DigitalOut.h"
#include "InterfaceDigitalInOut.h"
#include "PinNameAliases.h"
#include "PinNamesTypes.h"
#include "ThisThread.h"
#include "mbed.h"
#include "dht11_mbedos.h"
#include <cstdio>
//#include "DHT/DHT.h"



Dht11* dht11_sensor = new Dht11(PC_3);

//DHT11 dht11_sensor(PD_9);


//DHT sensor(PD_9, DHT11);


// main() runs in its own thread in the OS
int main()
{
    //dht11_sensor->attach(dht11_pin);
    
    ThisThread::sleep_for(2000);


    while (true) {
        //sensor.readData();
        dht11_sensor->read();
        printf("Nem:%d, Sicaklik:%d\n", dht11_sensor->humidity, dht11_sensor->temperature);
        //float a = sensor.ReadTemperature(CELCIUS);
        //printf("%d, %d\n", (int)sensor.ReadTemperature(CELCIUS), (int)sensor.ReadHumidity());
        ThisThread::sleep_for(5000);
    }
}


/*
#include "PinNames.h"
#include "mbed.h"
#include "DHT.h"
 
DHT sensor(PC_5, DHT11);
 
int main()
{
    int error = 0;
    float h = 0.0f, c = 0.0f, f = 0.0f, k = 0.0f, dp = 0.0f, dpf = 0.0f;
 
    while(1) {
        wait_us(2e3);
        error = sensor.readData();
        if (0 == error) {
            c   = sensor.ReadTemperature(CELCIUS);
            f   = sensor.ReadTemperature(FARENHEIT);
            k   = sensor.ReadTemperature(KELVIN);
            h   = sensor.ReadHumidity();
            dp  = sensor.CalcdewPoint(c, h);
            dpf = sensor.CalcdewPointFast(c, h);
            printf("Temperature in Kelvin: %4.2f, Celcius: %4.2f, Farenheit %4.2f\n", k, c, f);
            printf("Humidity is %4.2f, Dewpoint: %4.2f, Dewpoint fast: %4.2f\n", h, dp, dpf);
        } else {
            printf("Error: %d\n", error);
        }
    }
}
*/