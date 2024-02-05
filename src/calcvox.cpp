#include <Arduino.h>
#include <Keypad.h>
#include "Encoder.h"
//#include "HardwareSerial.h" // probably dont need
#include <string>
#include <vector>
#include <map>
extern "C" {
	// define _times to return 0
	int _times(struct tms* buf) {
		return 0;
	}
}
#include "tinyexpr.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#define CALCVOX_H2
#include "pins.h"

void setup_keypad() {
	// Not used on H2
	return;
}
int position = 0;
int prevPosition = 0;
#if defined(UseRotary)
Encoder r = Encoder(RotaryA, RotaryB);
bool buttonState = false;
#endif

std::string get_key() {
	#if defined(UseRotary)
	//need to make sure it is only on keydown
	if (digitalRead(RotaryButton) == LOW && buttonState == false) {
		buttonState = true;
		return "all_clear";
	} else if (digitalRead(RotaryButton) == HIGH && buttonState == true) {
		buttonState = false;
	}
	#endif
	// TODO: Will eventually need to map to full strings EG sin ans etc
	char key = keypad.getKey();
	//Serial.println(key);
	if (key != NO_KEY) {
		return std::string(1, key);
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
		{"=", "equals"},
		{"clockwise", "clockwise"},
		{"counterclockwise", "counterclockwise"},
		{"all_clear", "all clear"},
		{"delete", "delete"}
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

void setup() {
	#if defined(UseRotary)
	pinMode(RotaryButton, INPUT);
	prevPosition = r.read();
	#endif
	Serial.begin(115200);
	TalkSerial.begin(115200);

}

void loop() {
	//
	// Code will be able to be updated to use TalkSerial for the serial connection to the ESP.
	// For now it is just set to usb serial as I work on the prototype more and test it
	//
	//Serial.println("Loop");
	#if defined(UseRotary)
	prevPosition = position;
	position = r.read();
	int delta = position - prevPosition;
	// is pretty broken rn, TODO: fix
	if (delta != 0) {
		TalkSerial.print("#!Volume ");
		TalkSerial.println(delta);
	}
	//need to make sure it is only on keydown
	#endif
	std::string key = get_key();
	if (key != "") {
		Serial.println(key.c_str());
		if (key == "=") {
			std::string result = eval(current_equation, 2);
			TalkSerial.println(result.c_str()); // idk why it wouldnt work when saving result a variable but this work. c is magic
		} else if (key == "all_clear") {
			if (!current_equation.empty()) {
				current_equation = "";
				TalkSerial.println("All clear");
			} else {
				TalkSerial.println("Empty");
			}
		} else if (key == "delete") {
			if (!current_equation.empty()) {
				std::string last_char = convert_character(current_equation.substr(current_equation.length() - 1));
				current_equation.pop_back();
				TalkSerial.println(last_char.c_str());
			} else {
				TalkSerial.println("Empty");
			}
		} else {
			const char *to_speak = convert_character(key).c_str();
			TalkSerial.println(to_speak);
			current_equation += key;
		}
	} 
}
