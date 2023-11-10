#include <Arduino.h>
#include <Keypad.h>
#include "AudioTools.h"
#include "filesystems.h"
#include "espeak.h"
#include "SPI.h"
#include <string>

const byte ROWS = 3;
const byte COLUMNS = 5;

// Pins for buttons.
byte cols[COLUMNS] = {23, 22, 14, 32, 15};
byte rows[ROWS] = {33, 27, 22};

char keys[ROWS][COLUMNS] = {
	{'.', '1', '4', '7', '+'},
	{'0', '2', '5', '8', '-'},
	{'=', '3', '6', '9', '*'}
};

void setup_keypad() {
	for (byte r = 0; r < ROWS; r++) {
		pinMode(rows[r], OUTPUT);
		digitalWrite(rows[r], HIGH);
	}
	for (byte c = 0; c < COLUMNS; c++) {
		pinMode(cols[c], INPUT_PULLUP);
	}
}

char get_key() {
	for (byte r = 0; r < ROWS; r++) {
		digitalWrite(rows[r], LOW);
		for (byte c = 0; c < COLUMNS; c++) {
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

// Todo: clean this up.
std::string convert_character(const char character) {
	std::string final;
	switch (character) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		final = character;
		break;
	case '+':
		final = "plus";
		break;
	case '-':
		final = "minus";
		break;
	case '*':
		final = "times";
		break;
	case '/':
		final = "divide";
		break;
	case '.':
		final = "point";
		break;
	case '=':
		final = "equals";
		break;
	default:
		final = "";
		break;
	}
	return final;
}

AnalogAudioStream analog ;
ESpeak espeak(analog);

void setup() {
	Serial.begin(115200);
	espeak.begin();
	auto espeak_info = espeak.audioInfo();
	auto cfg = analog.defaultConfig();
	cfg.channels = espeak_info.channels; // 1
	cfg.sample_rate = espeak_info.sample_rate; // 22050
	cfg.bits_per_sample = espeak_info.bits_per_sample; // 16
	analog.begin(cfg);
	setup_keypad();
}

void loop() {
	char key = get_key();
	if (key != NO_KEY) {
		// Serial.println(key);
		const char* to_speak = convert_character(key).c_str();
		espeak.say(to_speak);
	}
}
