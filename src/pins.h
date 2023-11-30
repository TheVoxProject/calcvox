#pragma once

#if defined(CALCVOX_PROTOTYPE)
const byte ROWS = 3;
const byte COLUMNS = 5;
byte cols[COLUMNS] = {23, 22, 14, 32, 15};
byte rows[ROWS] = {33, 27, 12};
std::string keys[COLUMNS][ROWS] = {
    {".", "0", "="},
    {"1", "2", "3"},
    {"4", "5", "6"},
    {"7", "8", "9"},
    {"+", "-", "all_clear"}
};

#elif defined(CALCVOX_H1)
const byte ROWS = 8;
const byte COLUMNS = 8;
byte cols[COLUMNS] = {3, 19, 18, 5, 4, 33, 15, 32};
byte rows[ROWS] = {34, 39, 36, 14, 22, 23, 1, 21};
std::string keys[COLUMNS][ROWS] = {
    {"quit", "options", "p1", "p2", "p3", "p4", "p5", "p6"},
    {"log base", "sqrt", "stat/plot", "y=", "win/zoom", "calc", "graph/trace", "p7"},
    {"log", "^", "sin", "cos", "tan", "repeat", "read current", "dpad right"},
    {"ln", "x^2", "(", ")", "/", "dpad left", "dpad up", "p8"},
    {"e", "7", "8", "9", "*", "clear", "dpad center", "p9"},
    {"pi", "4", "5", "6", "-", "math", "dpad down", "vars"},
    {"x", "1", "2", "3", "+", "stat", "del", "p0"},
    {"on", "0", ".", "(-)", "=", "2nd", "apps", "alpha"}
};

#elif defined(CALCVOX_H2)

#endif
