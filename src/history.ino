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
