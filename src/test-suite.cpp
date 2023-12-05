#include <Arduino.h>
#include <Keypad.h>
#include "AudioTools.h"
#include "filesystems.h"
#include "espeak.h"
#include "SPI.h"
#include <string>
#include <vector>
#include <map>
#include "tinyexpr.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#define CALCVOX_H1
#include "pins.h"

void setup_keypad() {
	for (byte r = 0; r < COLUMNS; r++) {
		pinMode(cols[r], OUTPUT);
		digitalWrite(cols[r], HIGH);
	}
	for (byte c = 0; c < ROWS; c++) {
		pinMode(rows[c], INPUT_PULLUP);
	}
}

std::string get_key() {
	for (byte r = 0; r < COLUMNS; r++) {
		digitalWrite(cols[r], LOW);
		for (byte c = 0; c < ROWS; c++) {
			if (digitalRead(rows[c]) == LOW) {
				delay(10);
				if (digitalRead(rows[c]) == LOW) {
					digitalWrite(cols[r], HIGH);
					return keys[r][c];
				}
			}
			//delay(5);
		}
		digitalWrite(cols[r], HIGH);
	}
	return "";
}


AudioInfo info(44100, 2, 16);


#if defined(I2SOUT)
I2SStream audio_out;
auto i2s_config = audio_out.defaultConfig(TX_MODE);
config.copyFrom(info); 
config.i2s_format = I2S_LSB_FORMAT;
config.pin_ws = i2s_ws;
config.pin_bck = i2s_bclk;
config.pin_data = i2s_din;

#elif defined(ANALOGOUT)
AnalogAudioStream audio_out;
#endif
std::string current_equation;
std::vector<std::string> history;

void setup() {
	Serial.begin(115200);
	Serial.println("Running = test");
	/* need to deal with
    espeak.begin();
	auto espeak_info = espeak.audioInfo();
	auto cfg = analog.defaultConfig();
	cfg.channels = espeak_info.channels; // 1
	cfg.sample_rate = espeak_info.sample_rate; // 22050
	cfg.bits_per_sample = espeak_info.bits_per_sample; // 16
	analog.begin(cfg);*/
	setup_keypad();
}

void loop() {
	Serial.println("CalcVox Hardware Test: Select option to test");
    Serial.println("1. Button Matrix Pins");
    Serial.println("2. Button Matrix Assignments");
    Serial.println("3. Audio Output");
    Serial.println("4. Speech Output");
    Serial.println("5. Rotary Encoder");

 
    while (Serial.available() == 0) {}
    int option = Serial.parseInt();
    switch (option)
    {
    case 1:
        Serial.println("Testing Button Matrix Pins");
        Serial.println("Showing pins when pressed: reset device to exit");
        for (byte r = 0; r < COLUMNS; r++) {
            digitalWrite(cols[r], LOW);
            for (byte c = 0; c < ROWS; c++) {
                if (digitalRead(rows[c]) == LOW) {
                    delay(10);
                    if (digitalRead(rows[c]) == LOW) {
                        digitalWrite(cols[r], HIGH);
                        Serial.println("Row: " + String(r) + " Column: " + String(c));
                    }
                }
                //delay(5);
            }
		    digitalWrite(cols[r], HIGH);
	    }
        break;
    
    case 2:
        Serial.println("Testing Button Matrix Assignments");
        Serial.println("Showing keys when pressed: reset device to exit");
        while (true) {
            std::string key = get_key();
            if (key != "") {
                Serial.println(key.c_str());
            }
        }
        break;
    
    default:
        Serial.println("Invalid option");
        break;
    }

}
