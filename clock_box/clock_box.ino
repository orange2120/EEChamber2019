/*
 * For clock controlled box
 * based on UART protocol
 */
#include <Servo.h>

const uint8_t servo_pin[] = {10, 11};
const uint8_t buzzer_pin = 13;
const uint8_t endstop_pin = A0; // detect box open/close

// position(angle) of servos
#define Ser_1_unlk 105
#define Ser_1_lock 10
#define Ser_2_unlk 165
#define Ser_2_lock 54

Servo sv1;
Servo sv2;

#define OPEN_CMD 'O'
#define LOCK_CMD 'C'

char last_cmd = LOCK_CMD;

void setup()
{
    Serial.begin(9600);

    pinMode(endstop_pin, INPUT_PULLUP);

    sv1.attach(servo_pin[0]);
    sv2.attach(servo_pin[1]);

    if (!digitalRead(endstop_pin))
        box_lock();
    else
        box_unlk();
}

void loop()
{
    if(Serial.available())
    {
        char c = Serial.read();
        if (!digitalRead(endstop_pin))
        {
            if (c == OPEN_CMD && last_cmd != c)
            {
                box_unlk();
                last_cmd = c;
            }
            else if (c == LOCK_CMD && last_cmd != c)
            {
            
                delay(500);
                box_lock();
                last_cmd = c;
            }  
        }
        else // to avoid lock accidentally
            box_unlk_fast(); 
    }
}

// Unlock the box quickly
void box_unlk_fast()
{
    sv1.write(Ser_1_unlk);
    sv2.write(Ser_2_unlk);
}

// Unlock the box
void box_unlk()
{
    tone(buzzer_pin, 440, 500);
    for (int i = Ser_1_lock; i < Ser_1_unlk; ++i)
    {
        sv1.write(i);
        delay(10);
    }
    tone(buzzer_pin, 440, 500);
    for (int i = Ser_2_lock; i < Ser_2_unlk; ++i)
    {
        sv2.write(i);
        delay(10);
    }
    tone(buzzer_pin, 440, 1000);

}

// Lock the box
void box_lock()
{
    sv1.write(Ser_1_lock);
    sv2.write(Ser_2_lock);
}