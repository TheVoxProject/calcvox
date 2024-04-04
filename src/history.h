#pragma once

#include "calcvox.h"
#include <string>
#include <vector>

// Represents a single item in the calculation history.
struct history_item {
    std::string equation;
    std::string result;
};

class history {
public:
    history();
    void clear(bool silent = false);
    void add_item(const std::string& equation, const std::string& result);
    bool scroll_up();
    bool scroll_down();
    history_item *operator[](size_t index);
    bool set_pos(int new_pos, bool silent = false);

private:
    std::vector<history_item> buff;
    std::vector<history_item>::size_type pos;
};
