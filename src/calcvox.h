#ifndef CALCVOX_H
#define CALCVOX_H

#include <Arduino.h>
#include <Encoder.h>
#include <Metro.h>
#include <cstring>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <keypad.h>

#define TalkSerial Serial4 //rx 16, tx 17
#define UseRotary
#define RotaryA 20
#define RotaryB 21
#define RotaryButton 22
#define Buzzer 18

extern std::string current_equation;

const byte ROWS = 6;
const byte COLUMNS = 8;
byte rows[ROWS] = {6, 5, 4, 3, 2, 1};
byte cols[COLUMNS] = {25, 26, 27, 28, 29, 30, 31, 32};
char keys[ROWS][COLUMNS] = {
    {'l', 'o', 'r', 'q', '^', 'd', 'U', 'a'},
    {'p', 'S', 'C', 'T', '/', 'L', 's', 'R'},
    {'e', '7', '8', '9', '*', 'u', 'D', 'A'},
    {'>', '4', '5', '6', '-', 'B', 'u', 'u'},
    {'x', '1', '2', '3', '+', 'u', 'u', 'u'},
    {'O', '0', '.', '-', '=', '(', ')', 'u'}
};

Keypad keypad = Keypad(makeKeymap(keys), rows, cols, ROWS, COLUMNS);

void speak(const std::string &text);
bool key_pressed(const std::string &key);
void beep(int frequency, int duration);

#endif
