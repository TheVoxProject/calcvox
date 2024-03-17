#include <Arduino.h>
#include <Keypad.h>
#include "Encoder.h"
//#include "HardwareSerial.h" // probably dont need
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <sstream>
#include <iomanip>
#include "evox.h"
#define CALCVOX_H2
#include "pins.h"
#include <Metro.h>

Metro rotaryTimer = Metro(100);
Metro buttonTimer = Metro(25);

int position = 0;
int prevPosition = 0;
#if defined(UseRotary)
Encoder r = Encoder(RotaryA, RotaryB);
bool buttonState = false;
#endif

const std::map<char, std::string> special_keys = {
	{'S', "sin("},
	{'C', "cos("},
	{'T', "tan("},
	{'O', "on"},
	{'l', "ln("},
	{'o', "log("},
	{'r', "root("},
	{'s', "^2"},
	{'e', "e"},
	{'p', "pi"},
	{'U', "up"},
	{'D', "down"},
	{'R', "right"},
	{'L', "left"},
	{'s', "select"},
	{'a', "all_clear"},
	{'d', "delete"},
	{'A', "repeat"}
};

bool key_pressed(const std::string& key) {
	/* TODO: Figure out how to make this work. 
		#if defined(UseRotary)
	if (buttonTimer.check()) {
		if (digitalRead(RotaryButton) == LOW && buttonState == false) {
			buttonState = true;
			return "all_clear";
		} else if (digitalRead(RotaryButton) == HIGH && buttonState == true) {
			buttonState = false;
		}
	}
	#endif*/
	char pressedKey = keypad.getKey();
	if (pressedKey != NO_KEY) {
		auto it = special_keys.find(pressedKey);
		if (it != special_keys.end()) {
			return true;
		}
	}
	return false;
}

std::string get_key() {
	char key = keypad.getKey();
	if (key != NO_KEY) {
		auto it = special_keys.find(key);
		if (it != special_keys.end()) {
			return it->second;
		}
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
		{"^", "to the power of"},
		{"^2", "squared"},
		{"(", "open parenthesis"},
		{")", "close parenthesis"},
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
	//Serial.println("Loop");
	#if defined(UseRotary)
	if (rotaryTimer.check()) {
		prevPosition = position;
		position = r.read();
		int delta = position - prevPosition;
		if (delta != 0) {
			TalkSerial.print("#!V ");
			TalkSerial.println(delta);
		}
	}
	#endif
	if (key_pressed("=")) {
		std::string result = std::to_string(evox(current_equation));
		Serial.println(result.c_str());
		TalkSerial.println(result.c_str()); // idk why it wouldnt work when saving result a variable but this work. c is magic
		current_equation = "";
	}
	else if (key_pressed("all_clear")) {
		if (!current_equation.empty()) {
			current_equation = "";
			TalkSerial.println("All clear");
		} else {
			TalkSerial.println("Empty");
		}
	}
	else if (key_pressed("delete")) {
		if (!current_equation.empty()) {
			std::string last_char = convert_character(current_equation.substr(current_equation.length() - 1));
			current_equation.pop_back();
			TalkSerial.println(last_char.c_str());
		} else {
			TalkSerial.println("Empty");
		}
	}
	else if (key_pressed("repeat")) {
		TalkSerial.println(current_equation.c_str()); // temp for testing layout likely bad. says "5 to 1" not "5 minus 1"
	}
	else {
		std::string to_speak = convert_character(get_key());
		TalkSerial.println(to_speak.c_str());
		current_equation += get_key();
	}
}
