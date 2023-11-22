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

const byte ROWS = 3;
const byte COLUMNS = 5;

// Pins for buttons.
byte cols[COLUMNS] = {23, 22, 14, 32, 15};
byte rows[ROWS] = {33, 27, 12};

char keys[ROWS][COLUMNS] = {
	{'=', '1', '4', '7', '+'},
	{'0', '2', '5', '8', '-'},
	{'.', '3', '6', '9', '*'}
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

std::string convert_character(const char character) {
	static const std::map<char, std::string> char_map = {
		{'0', "0"},
		{'1', "1"},
		{'2', "2"},
		{'3', "3"},
		{'4', "4"},
		{'5', "5"},
		{'6', "6"},
		{'7', "7"},
		{'8', "8"},
		{'9', "9"},
		{'+', "plus"},
		{'-', "minus"},
		{'*', "times"},
		{'/', "divide"},
		{'.', "point"},
		{'=', "equals"}
	};
	auto it = char_map.find(character);
	if (it == char_map.end()) {
		return "";
	}
	return it->second;
}

std::string eval(const std::string& expression, const int precision) {
	te_variable vars[] = {};
	int err;
	te_expr* expr = te_compile(expression.c_str(), vars, 0, &err);
	if (!expr) {
		return "Error";
	}
	double result = te_eval(expr);
	te_free(expr);
	std::ostringstream result_stream;
	result_stream << std::fixed << std::setprecision(precision) << result;
	return result_stream.str();
}
AnalogAudioStream analog ;
ESpeak espeak(analog);
std::string current_equation;
std::vector<std::string> history;

void setup() {
	Serial.begin(115200);
	Serial.println("Running = test");
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
		if (key == '=') {
			const char* result = eval(current_equation, 2).c_str();
			espeak.say(result);
		} else {
			const char *to_speak = convert_character(key).c_str();
			espeak.say(to_speak);
			current_equation += key;
		}
	}
}
