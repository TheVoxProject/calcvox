#define CALCVOX_H2
#include "pins.h"
#include "calcvox.h"
#include "menu.h"

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
    };
    auto it = char_map.find(character);
    if (it == char_map.end()) return "";
    return it->second;
}

std::string current_equation;
history hist;

void setup() {
#if defined(UseRotary)
    pinMode(RotaryButton, INPUT);
    prevPosition = r.read();
#endif
    Serial.begin(115200);
    TalkSerial.begin(115200);

    // startup tone
    tone(Buzzer, 1000);
    delay(100);
    noTone(Buzzer);
    delay(50);
    tone(Buzzer, 2000);
    delay(100);
    noTone(Buzzer);
}

void loop() {
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
    if (keypad.isPressed('=')) {
        std::string result = std::to_string(evox(current_equation));
        Serial.println(result.c_str());
        TalkSerial.println(result.c_str()); // idk why it wouldnt work when saving result a variable but this work. c is magic
        current_equation = "";
    }
    else if (keypad.isPressed('C')) {
        if (!current_equation.empty()) {
            current_equation = "";
            TalkSerial.println("All clear");
        } else {
            TalkSerial.println("Empty");
        }
    }
    else if (keypad.isPressed('d')) {
        if (!current_equation.empty()) {
            std::string last_char = convert_character(current_equation.substr(current_equation.length() - 1));
            current_equation.pop_back();
            TalkSerial.println(last_char.c_str());
        } else {
            TalkSerial.println("Empty");
        }
    }
    else if (keypad.isPressed('r')) {
        TalkSerial.println(current_equation.c_str()); // temp for testing layout likely bad. says "5 to 1" not "5 minus 1"
    }
    else if (keypad.isPressed('U'))
        hist.scroll_up();
    else if (keypad.isPressed('D'))
        hist.scroll_down();
    /* else {
        std::string to_speak = convert_character(get_key());
        TalkSerial.println(to_speak.c_str());
        current_equation += get_key();
    } */
}
