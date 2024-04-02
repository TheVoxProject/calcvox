#include "calcvox.h"
#include "evox.hpp"
#include "history.h"
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

bool key_pressed(const std::string &key) {
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
    if (it == char_map.end())
        return "";
    return it->second;
}

std::string last_spoken; // for repeat

// Speaks an std::string using the TTS engine over serial
void speak(const std::string &text) {
    TalkSerial.println(text.c_str());
    last_spoken     = text;
}

std::string current_equation;
history hist;

void setup() {
#if defined(UseRotary)
    pinMode(RotaryButton, INPUT);
    prevPosition = r.read();
#endif
    evox::init();
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
    speak("Calcvox ready");
}

void loop() {
    if (rotaryTimer.check()) {
        prevPosition = position;
        position = r.read();
        int delta = position - prevPosition;
        if (delta != 0) {
            speak("#!V " + std::to_string(delta));
        }
    }
    if (keypad.isPressed('=')) {
        std::string result = std::to_string(evox::evox(current_equation));
        Serial.println(result.c_str());
        speak(result);
        current_equation = "";
    } else if (keypad.isPressed('a')) {
        if (!current_equation.empty()) {
            current_equation = "";
            speak("All clear");
        } else {
            speak("Empty");
        }
    } else if (keypad.isPressed('d')) {
        if (!current_equation.empty()) {
            std::string last_char = convert_character(current_equation.substr(current_equation.length() - 1));
            current_equation.pop_back();
            speak(last_char);
        } else {
            speak("Empty");
        }
    } else if (keypad.isPressed('A'))
        speak(last_spoken);
    else if (keypad.isPressed('U')) {
        hist.scroll_up();
    } else if (keypad.isPressed('D')) {
        hist.scroll_down();
    } else {
        char key = keypad.getKey();
        if (key != NO_KEY) {
            speak(std::string(1, key));
            current_equation += key;
        }
    }
}
