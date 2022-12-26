#pragma once

#include "mbed.h"
#include "DigitalInOut.h"

class Dht11
{
private:
	DigitalInOut* sensorPin;//defaults to pin 2
public:
    void attach(DigitalInOut* pin);//Attaches dht11 object to a particular sensorPin
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
