#include <Arduino.h>
#include <Keypad.h>
#include "speech_handler.h"

const byte ROW_NUM = 3; 
const byte COLUMN_NUM = 5; 

byte cols[COLUMN_NUM] = { 23,22,14,32,15 };
byte rows[ROW_NUM] = { 33,27,22 };

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','4','5'},
  {'6','7','8','9','0'},
  {'*','#','A','B','C'}
};

void setupKeypad() {
  for (byte r = 0; r < ROW_NUM; r++) {
    pinMode(rows[r], OUTPUT);
    digitalWrite(rows[r], HIGH);
  }

  for (byte c = 0; c < COLUMN_NUM; c++) {
    pinMode(cols[c], INPUT_PULLUP);
  }
}

char getKey() {
  for (byte r = 0; r < ROW_NUM; r++) {
    digitalWrite(rows[r], LOW);

    for (byte c = 0; c < COLUMN_NUM; c++) {
      if (digitalRead(cols[c]) == LOW) {
        delay(10);

        if (digitalRead(cols[c]) == LOW) {
          digitalWrite(rows[r], HIGH);
          return keys[r][c];
        }
      }
      //delay(5);
    }

    digitalWrite(rows[r], HIGH);
  }

  return NO_KEY;
}

AnalogAudioStream analog ; // or replace with AudioKitStream for AudioKit
speech_handler speech(analog);

void setup() {
  Serial.begin(115200);
  setupKeypad();
}

void loop() {
  char key = getKey();

  if (key != NO_KEY) {
    Serial.println(key);
  }
  if (key == '1') {
    speech.speak("Hello World");
  }
}