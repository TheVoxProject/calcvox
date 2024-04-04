#pragma once

#include "calcvox.h"
#include <string>
#include <vector>

// Represents the types of items to be inserted into the history.
enum history_item_type {
    EQUATION,
    RESULT
};

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
    void move_left();
    void move_right();
    void set_type(history_item_type new_type, bool silent = false);
    void insert_item();

private:
    std::vector<history_item> buff;
    // Current position in the buffer.
    std::vector<history_item>::size_type pos;
    // Type of the current item.
    history_item_type type;
};
