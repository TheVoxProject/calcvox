#include <Arduino.h>
#include <Keypad.h>
#include <string>
#include <vector>
#include <map>
//#include "tinyexpr.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#include "Talkie.h"
#include "Vocab_US_Large.h"
#define CALCVOX_PROTOTYPE
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
					Serial.println(keys[r][c].c_str());
					return keys[r][c];
				}
			}
			//delay(5);
		}
		digitalWrite(cols[r], HIGH);
	}
	return "";
}

std::string convert_character(const std::string character) {
	static const std::map<std::string, std::string> char_map = {
		{"0", "0"},
		{"1", "1"},
		{"2", "2"},
		{"3", "3"},
		{"4", "4"},
		{"5", "5"},
		{"6", "6"},
		{"7", "7"},
		{"8", "8"},
		{"9", "9"},
		{"+", "plus"},
		{"-", "minus"},
		{"*", "times"},
		{"/", "divide"},
		{".", "point"},
		{"=", "equals"}
	};
	auto it = char_map.find(character);
	if (it == char_map.end()) {
		return "";
	}
	return it->second;
}

std::string eval(const std::string& expression, const int precision) {
	/* te_variable vars[] = {};
	int err;
	te_expr* expr = te_compile(expression.c_str(), vars, 0, &err);
	if (!expr) {
		return "Error";
	}
	double result = te_eval(expr);
	te_free(expr);
	std::ostringstream result_stream;
	result_stream << std::fixed << std::setprecision(precision) << result;
	return result_stream.str(); */
	return "12";
}

std::string current_equation;
std::vector<std::string> history;
Talkie voice;

void setup() {
	Serial.begin(115200);
	#if defined(TEENSYDUINO)
		pinMode(5, OUTPUT);
		digitalWrite(5, HIGH); //Enable Amplified PROP shield
	#endif
	// setup_keypad();
}

void loop() {
	voice.say(sp2_DANGER);
	voice.say(sp2_DANGER);
	voice.say(sp2_RED);
	voice.say(sp2_ALERT);
	voice.say(sp2_MOTOR);
	voice.say(sp2_IS);
	voice.say(sp2_ON);
	voice.say(sp2_FIRE);
	/* std::string key = get_key();
	if (key != "") {
		Serial.println(key.c_str());
		if (key == "=") {
			const char* result = eval(current_equation, 2).c_str();
			flite.say(result);
		} else if (key == "all_clear") {
			if (!current_equation.empty()) {
				current_equation = "";
				flite.say("All clear");
			} else {
				flite.say("Empty");
			}
		} else if (key == "delete") {
			if (!current_equation.empty()) {
				std::string last_char = convert_character(current_equation.substr(current_equation.length() - 1));
				current_equation.pop_back();
				flite.say(last_char.c_str());
			} else {
				flite.say("Empty");
			}
		} else {
			const char *to_speak = convert_character(key).c_str();
			flite.say(to_speak);
			current_equation += key;
		}
	} */
}
