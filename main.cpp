#include "mbed.h"
#include "BufferedSerial.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "Thread.h"
#include "Mutex.h"

#define SENSOR_BUF_LEN 26

Mutex mut;

uint8_t sensor_buffer[SENSOR_BUF_LEN] = {0};

static BufferedSerial arduino(PC_10, PC_11, 9600);

static BufferedSerial esp(PA_9, PA_10,115200);

void sensor_thread_fn(void);
void esp_thread_fn(void);

char temperature[6] = {0};
char humidity[5] = {0};

char TxBuffer[11] = {0};


int main()
{

    Thread* sensor_thread = new Thread(osPriorityNormal, 2048, nullptr, "Sensor Thread");
    Thread* esp_thread = new Thread(osPriorityNormal, 2048, nullptr, "ESP Thread");

    sensor_thread->start(&sensor_thread_fn);
    sensor_thread->start(&esp_thread_fn);

    while (true)
    {

    }
}


void sensor_thread_fn(void)
{
    while (true)
    {
        mut.lock();
        if (uint32_t num = arduino.read(sensor_buffer, SENSOR_BUF_LEN))
        {
            sensor_buffer[num] = '\0';
            for(int i = 0; i < SENSOR_BUF_LEN; i++)
            {
                if(sensor_buffer[i] == 'h')
                {
                    for(int j = 0; j < 5; j++)
                    {
                        humidity[j] = sensor_buffer[i + j + 1];
                    }
                    

                    for(int j = 0; j < 5; j++)
                    {
                        temperature[j] = sensor_buffer[i + j + 7];
                    }
                    temperature[5] = '\0';
                    break;
                }
            }
        }
        printf("h:%st:%s", humidity, temperature);
        mut.unlock();
        
        ThisThread::sleep_for(10000);
    }
}


void esp_thread_fn(void)
{
    while(true){
        mut.lock();
        int i = 0;
        
        for(i; i < 5; i++){
            TxBuffer[i+5] = temperature[i];
        }
        serial_port.write(TxBuffer, 11);
        mut.unlock();
        ThisThread::sleep_for(5000);
    }

}