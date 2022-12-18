#include "dht11_mbedos.h"

#define HIGH 1
#define LOW 0

void dht11::attach(DigitalInOut* pin)
{
    sensorPin = pin;
}

int dht11::read()
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	sensorPin->output();
	sensorPin->write(LOW);
	//delay(18);
	sensorPin->write(HIGH);
	//delayMicroseconds(40);
	sensorPin->input();

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(sensorPin->read() == LOW)
		if (loopCnt-- == 0) return -2;

	loopCnt = 10000;
	while(sensorPin->read() == HIGH)
		if (loopCnt-- == 0) return -2;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(sensorPin->read() == LOW)
			if (loopCnt-- == 0) return -2;

		unsigned long t = micros();

		loopCnt = 10000;
		while(sensorPin->read() == HIGH)
			if (loopCnt-- == 0) return -2;

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}

	// WRITE TO RIGHT VARS
    // as bits[1] and bits[3] are allways zero they are omitted in formulas.
	humidity    = bits[0]; 
	temperature = bits[2]; 

	uint8_t sum = bits[0] + bits[2];  

	if (bits[4] != sum) return -1;
	return 0;
}