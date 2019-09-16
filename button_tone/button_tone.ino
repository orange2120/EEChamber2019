/* 
 * Melody based door lock
 * if you press right melody, the door will unlock.
 */
#include <Bounce2.h>

const uint8_t button_pin[] = {6, 7, 8, 9, 10, 4, 5};
const uint8_t lock_pin = 11;

#define nButton 7 // number of buttons
Bounce *sw[7] = {NULL};

#define DOOR_OPEN_DELAY 10000 //ms

// tone of the song
#define nBEATS 14
const uint8_t tones[] = {1,1,5,5,6,6,5,4,4,3,3,2,2,1}; // little star
volatile uint8_t record[nBEATS] = {0};

void setup()
{
    Serial.begin(115200);

    for (uint8_t i = 0; i < nButton; ++i)
        sw[i] = new Bounce;
    
    for (uint8_t i = 0; i < nButton; ++i)
    {
        sw[i]->attach(button_pin[i], INPUT_PULLUP);
        sw[i]->interval(25);
    }

    pinMode(lock_pin, OUTPUT);
    digitalWrite(lock_pin, LOW);

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
            record_fifo(i + 1);
            
            #ifdef DEBUG
            for (uint8_t j = 0; j < nBEATS; ++j)
                Serial.print(record[j]);

                Serial.println("");
            #endif
        }
    }

    // check the tones is correspond with answer
    for (uint8_t i = 0; i < nBEATS; ++i)
    {
        if (record[i] != tones[i])
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
        record[i] = record[i + 1]; // move forward(pop first element)
    }
    record[nBEATS - 1] = k; // push back
}

void resetRecord()
{
	for (uint8_t i = 0; i < nBEATS; ++i)
		record[i] = 0;
}

// unlock the door
void unlock()
{
    #ifdef DEBUG
        Serial.println("UNLK");
    #endif
    digitalWrite(lock_pin, HIGH);
    delay(DOOR_OPEN_DELAY);
    digitalWrite(lock_pin, LOW);
    resetRecord();
}