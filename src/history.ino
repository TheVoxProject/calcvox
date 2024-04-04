#include "history.h"

history::history()
    : pos(0) {}

void history::clear(bool silent) {
    buff.clear();
    pos = 0;
    if (!silent) {
        speak("History cleared");
    }
}

// Adds an item to the history given an equation and result
void history::add_item(const std::string &equation, const std::string &result) {
    buff.push_back({equation, result});
    pos = buff.size() - 1;
}

bool history::scroll_down() {
    if (buff.empty() || pos >= (buff.size() - 1))
        return false;
    pos++;
    std::string message = buff[pos].equation + " = " + buff[pos].result;
    speak(message);
    return true;
}

bool history::scroll_up() {
    if (buff.empty() || pos <= 0)
        return false;
    pos--;
    std::string message = buff[pos].equation + " = " + buff[pos].result;
    speak(message);
    return true;
}

history_item *history::operator[](size_t index) {
    return &buff[index];
}

bool history::set_pos(int new_pos, bool silent) {
    if (pos < 0 || pos >= (buff.size() - 1))
        return false;
    pos = new_pos;
    if (!silent) {
        TalkSerial.printf("%s = %s", buff[pos].equation.c_str(), buff[pos].result.c_str());
    }
    return true;
}

// Moves left, to the previous type of item in the history
void history::move_left() {
    if (type == RESULT) {
        type = EQUATION;
        speak("Equation");
    }
}

// Moves right, to the next type of item in the history
void history::move_right() {
    if (type == EQUATION) {
        type = RESULT;
        speak("Result");
    }
}

// Sets the type of the current item in the history
void history::set_type(history_item_type new_type, bool silent) {
    type = new_type;
    if (!silent) {
        std::string type_str = new_type == EQUATION ? "Equation" : "Result";
        speak(type_str);
    }
}

// Inserts a portion of the focused item into the current equation, dependant on the type selected.
void history::insert_item() {
    if (type == EQUATION) {
        current_equation += buff[pos].equation;
    } else {
        current_equation += buff[pos].result;
    }
    speak("Inserted");
}
