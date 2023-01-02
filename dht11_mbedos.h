#pragma once

#include "mbed.h"
#include "DigitalInOut.h"
#include <cstdio>

class Dht11
{
private:
	DigitalInOut* sensorPin;
public:
    Dht11(PinName pin);//Attaches dht11 object to a particular Pin
	int read();
	int humidity;
	int temperature;
	int celcius();
	double fahrenheit();
	double fahrenheit(double dCelcius);
	double kelvin();
	double kelvin(double dCelcius);
	double dewPoint();
	double dewPointFast();
};
