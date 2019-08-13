/* 
 * International clock position detector 
 */

const uint8_t pin[] = {4, 5, 6, 7};

uint32_t now = 0;
uint32_t last = 0;

void setup()
{
    Serial.begin(9600);
    for (uint8_t i = 0; i < 4; ++i)
        pinMode(pin[i], INPUT);
}

void loop()
{
    now = millis();
    if (now - last >= 200)
    {
        if (digitalRead(pin[0]) && digitalRead(pin[1]) && digitalRead(pin[2]) && digitalRead(pin[3]))
        {
            Serial.print("O"); // box unlock command
        }
        else
            Serial.print("C"); // box lock command

        last = now;
    }
}