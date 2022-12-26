#include "dht11_mbedos.h"
#include "Kernel.h"
#include "ThisThread.h"
#include "us_ticker_api.h"
#include <cstdint>
#include <ratio>

#define HIGH 1
#define LOW 0

void Dht11::attach(DigitalInOut* pin)
{
    sensorPin = pin;
}

int Dht11::read()
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
	//wait 18ms
    ThisThread::sleep_for(18);
	sensorPin->write(HIGH);
	//wait 40us
    wait_us(40);
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

        //auto t = Kernel::Clock::now();

        //auto t0_t = std::chrono::time_point_cast<std::chrono::microseconds>(Kernel::Clock::now());
        //volatile long t0 = t0_t.time_since_epoch().count();

        volatile auto t0 = us_ticker_read();

		//unsigned long t = micros();

		loopCnt = 10000;
		while(sensorPin->read() == HIGH)
			if (loopCnt-- == 0) return -2;

        //auto t1_t = std::chrono::time_point_cast<std::chrono::microseconds>(Kernel::Clock::now());
        //volatile long t1 = t1_t.time_since_epoch().count();

        volatile auto t1 = us_ticker_read();

		if ((t1 - t0) > 40) bits[idx] |= (1 << cnt);
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