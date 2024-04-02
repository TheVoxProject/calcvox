#pragma once

#ifdef CALCVOX_H2
#include <keypad.h>

const byte ROWS = 6;
const byte COLUMNS = 8;
byte rows[ROWS] = {6, 5, 4, 3, 2, 1};
byte cols[COLUMNS] = {25, 26, 27, 28, 29, 30, 31, 32};
char keys[ROWS][COLUMNS] = {
	{'l', 'o', 'r', 's', '^', 'd', 'U', 'a'},
	{'p', 'S', 'C', 'T', '/', 'L', 's', 'R'},
	{'e', '7', '8', '9', '*', 'u', 'D', 'A'},
	{'>', '4', '5', '6', '-', 'u', 'u', 'u'},
	{'x', '1', '2', '3', '+', 'u', 'u', 'u'},
	{'O', '0', '.', '-', '=', '(', ')', 'u'}};

Keypad keypad = Keypad(makeKeymap(keys), rows, cols, ROWS, COLUMNS);

#define UseRotary
#define RotaryA 20
#define RotaryB 21
#define RotaryButton 22
#define Buzzer 18 // not implemented yet
#endif
