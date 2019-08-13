/*
 * Melody based lock door
 */
#include <Bounce2.h>

const uint8_t button_pin[] = {4, 5, 6, 7, 8, 9, 10};
const uint8_t lock_pin = 11;

// tone of the song
#define nBEATS 13
const uint8_t tones[] = {1,1,5,5,6,6,5,4,4,3,3,2,2,1}; // little star

volatile bool record[nBEATS] = {0};
int current_beat = 0;

#define nButton 7 // number of buttons

Bounce *sw[7] = {NULL};

void setup()
{
    Serial.begin(9600);

    for (uint8_t i = 0; i < nButton; ++i)
        sw[i] = new Bounce;
    
    for (uint8_t i = 0; i < nButton; ++i)
    {
        sw[i]->attach(button_pin[i], INPUT_PULLUP);
        sw[i]->interval(25);
    }

    pinMode(lock_pin, OUTPUT);

    resetRecord();
    Serial.println("done.");
}

void loop()
{
    // update button status
    for (uint8_t i = 0; i < nButton; ++i)
        sw[i]->update();

    for (uint8_t i = 0; i < nButton; ++i)
    {
        if(sw[i]->fell()) // i th botton be pressed
        {
            Serial.println(i);
            record_fifo(i);
        }
    }

    for (uint8_t i = 0; i < nBEATS; ++i)
    {
        if (record[i] != record[i])
            break;
        if (i == (nBEATS - 1)) // all the tones are the same
            unlock();
    }
}

// updating "record" array by FIFO(first in first out)
// maybe quene is a better solution?
void record_fifo(uint8_t k)
{
    for (uint8_t i = 0; i < nBEATS - 1; ++i)
    {
        record[i] = record[i + i]; // move forward(pop first element)
    }
    record[nBEATS - 1] = k; // push back
}

void resetRecord()
{
	for (uint8_t i = 0; i < nBEATS; ++i)
	{
		record[i] = 0;
	}
}

void unlock()
{
    Serial.println("UNLK");
}

// 方案
/*
一次紀錄14個 
這樣無法從中間開始
動態檢查
這個可以
 */