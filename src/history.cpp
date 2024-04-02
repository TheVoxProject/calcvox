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

bool history::scroll_down() {
    if (buff.empty() || pos >= (buff.size() - 1))
        return false;
    pos++;
    TalkSerial.printf("%s = %s", buff[pos].equation.c_str(), buff[pos].result.c_str());
    return true;
}

bool history::scroll_up() {
    if (buff.empty() || pos <= 0)
        return false;
    pos--;
    TalkSerial.printf("%s = %s", buff[pos].equation.c_str(), buff[pos].result.c_str());
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
