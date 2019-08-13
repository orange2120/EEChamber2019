#include "pitches.h"
#include <Keypad.h>
#include <Servo.h>

#define KEY_ROWS 4  
#define KEY_COLS 3  
#define LED_1 A1 
#define LED_2 A2
#define LED_3 A3
#define LED_4 A4

#define Servo_1_pin 10
#define Servo_2_pin 11
Servo Servo_1;
Servo Servo_2;

#define BOX_3

#ifdef BOX_1
#define Ser_1_open 117
#define Ser_1_lock 10
#define Ser_2_open 167
#define Ser_2_lock 54
#endif
#ifdef BOX_2
#define Ser_1_open 124
#define Ser_1_lock 35
#define Ser_2_open 115
#define Ser_2_lock 27
#endif
#ifdef BOX_3
#define Ser_1_open 126
#define Ser_1_lock 10
#define Ser_2_open 165
#define Ser_2_lock 54
#endif
#ifdef BOX_4
#define Ser_1_open 67
#define Ser_1_lock 10
#define Ser_2_open 150
#define Ser_2_lock 60
#endif
#ifdef BOX_5
#define Ser_1_open 140
#define Ser_1_lock 17
#define Ser_2_open 110
#define Ser_2_lock 17
#endif
#ifdef BOX_5
#define Ser_1_open 105
#define Ser_1_lock 10
#define Ser_2_open 145
#define Ser_2_lock 54
#endif
#ifdef BOX_test
#define Ser_1_open 117
#define Ser_1_lock 10
#define Ser_2_open 165
#define Ser_2_lock 54
#endif

char password[4] = {'2', '2', '3', '2'};

int Ser_1_pos = Ser_1_lock;
int Ser_2_pos = Ser_2_lock;

char keymap[KEY_ROWS][KEY_COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[KEY_ROWS] = {2, 3, 4, 5}; //Rows 0 to 3
byte colPins[KEY_COLS] = {6, 7, 8}; //Columns 0 to 3

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, KEY_ROWS, KEY_COLS);


void setup() {
  Serial.begin(9600);
  Servo_1.attach(Servo_1_pin);
  Servo_2.attach(Servo_2_pin);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  digitalWrite(LED_1, 0);
  digitalWrite(LED_2, 0);
  digitalWrite(LED_3, 0);
  digitalWrite(LED_4, 0);
  pinMode(A0, INPUT);
  digitalWrite(A0, HIGH);
}
int state = 0;
char key[4] = {0};
bool box_state = 0;
bool real_state = 0;
unsigned long long real_start = 0;
void loop() {
  if(analogRead(A0) > 500 && real_state == 0){
    delay(50);
    if(analogRead(A0) > 500 ){
      real_state = 1;
    }
  }
  if(analogRead(A0) < 500 && real_state == 1){
    delay(500);
    if(analogRead(A0) < 500 ){
      digitalWrite(LED_1, 0);
      digitalWrite(LED_2, 0);
      digitalWrite(LED_3, 0);
      digitalWrite(LED_4, 0);
      real_state = 0;
      state = 0;
      box_state = 0;
    }
  }
  
  switch(state){
    case 0://waiting key 0
      if(box_state ==0 ){
        Ser_1_pos = Ser_1_lock;
        Ser_2_pos = Ser_2_lock;
      }else{
        Ser_1_pos = Ser_1_open;
        Ser_2_pos = Ser_2_open;
      }
      key[0] = keypad.getKey();
      if(key[0]){
        state = 1;
        digitalWrite(LED_1, 1);
        tone_map(key[0]);
      }
      delay(20);
      break;
    case 1://waiting key 1
      if(box_state ==0 ){
        Ser_1_pos = Ser_1_lock;
        Ser_2_pos = Ser_2_lock;
      }else{
        Ser_1_pos = Ser_1_open;
        Ser_2_pos = Ser_2_open;
      }
      key[1] = keypad.getKey();
      if(key[1]){ 
        state = 2;
        digitalWrite(LED_2, 1);
        tone_map(key[1]);
      }  
      delay(20);
      break;
    case 2://waiting key 2
      if(box_state ==0 ){
        Ser_1_pos = Ser_1_lock;
        Ser_2_pos = Ser_2_lock;
      }else{
        Ser_1_pos = Ser_1_open;
        Ser_2_pos = Ser_2_open;
      }
      key[2] = keypad.getKey();
      if(key[2]){
        state = 3;
        digitalWrite(LED_3, 1);
        tone_map(key[2]);
      }
      delay(20);
      break;
    case 3://waiting key 0
      if(box_state ==0 ){
        Ser_1_pos = Ser_1_lock;
        Ser_2_pos = Ser_2_lock;
      }else{
        Ser_1_pos = Ser_1_open;
        Ser_2_pos = Ser_2_open;
      }
      key[3] = keypad.getKey();
      if(key[3]){
        state = 4;
        digitalWrite(LED_4, 1);
        tone_map(key[3]);
      }
      delay(20);
      break;
    case 4://all input done
      delay(500);
      if( key[0] == password[0] && key[1] == password[1] && key[2] == password[2] && key[3] == password[3]){
        open_song();
        box_state = 1;
      }else{
        lock_song();
        box_state = 0;
      }
      
      key[0] = 0;
      key[1] = 0;
      key[2] = 0;
      key[3] = 0;
      delay(500);
      state = 0;
      break;
  }
  Servo_1.write(Ser_1_pos);
  Servo_2.write(Ser_2_pos);
}

void lock_song(){
  digitalWrite(LED_1, 0);
  digitalWrite(LED_2, 0);
  digitalWrite(LED_3, 0);
  digitalWrite(LED_4, 0);
  delay(100);
  digitalWrite(LED_1, 1);
  digitalWrite(LED_2, 1);
  digitalWrite(LED_3, 1);
  digitalWrite(LED_4, 1);
  tone(13, NOTE_B2, 300);
  delay(300);
  digitalWrite(LED_1, 0);
  digitalWrite(LED_2, 0);
  digitalWrite(LED_3, 0);
  digitalWrite(LED_4, 0);
  delay(100);
  digitalWrite(LED_1, 1);
  digitalWrite(LED_2, 1);
  digitalWrite(LED_3, 1);
  digitalWrite(LED_4, 1);
  tone(13, NOTE_B2, 300);
  delay(300);
  digitalWrite(LED_1, 0);
  digitalWrite(LED_2, 0);
  digitalWrite(LED_3, 0);
  digitalWrite(LED_4, 0);
}
void open_song(){
  //int music[7] = { NOTE_FS5, NOTE_AS5, NOTE_CS6, NOTE_B5, NOTE_DS6, NOTE_FS6, NOTE_CS7};
  int music[8] = { NOTE_G6, NOTE_FS6, NOTE_DS6, NOTE_A5, NOTE_GS5, NOTE_E6, NOTE_GS6, NOTE_C7};
  for(int i = 0 ; i < 8 ; ++i){
    tone(13, music[i], 130);
    delay(130);
  }
}
void tone_map(char key){
  switch(key){
    case '1':
      tone(13, NOTE_C4, 300);
      delay(300);
      break;
    case '2':
      tone(13, NOTE_D4, 300);
      delay(300);
      break;
    case '3':
      tone(13, NOTE_E4, 300);
      delay(300);
      break;
    case '4':
      tone(13, NOTE_F4, 300);
      delay(300);
      break;
    case '5':
      tone(13, NOTE_G4, 300);
      delay(300);
      break;
    case '6':
      tone(13, NOTE_A4, 300);
      delay(300);
      break;
    case '7':
      tone(13, NOTE_B4, 300);
      delay(300);
      break;
    case '8':
      tone(13, NOTE_C5, 300);
      delay(300);
      break;
    case '9':
      tone(13, NOTE_D5, 300);
      delay(300);
      break;
    case '0':
      tone(13, NOTE_E5, 300);
      delay(300);
      break;
    case '*':
      tone(13, NOTE_A3, 300);
      delay(300);
      break;
    case '#':
      tone(13, NOTE_A5, 300);
      delay(300);
      break;
  }
  
}
