#pragma once

#include "calcvox.h"
#include <string>
#include <vector>

// Represents a single item in the menu.
struct menu_item {
	std::string title;
	std::string id; // Used for determining what item is clicked.
	bool clickable;
};

class menu {
public:
	menu(const std::string& intro_msg);
	menu* add_item(const std::string& title, const std::string& id, bool clickable = true);
	int loop();

private:
	std::vector<menu_item> items;
	std::string intro;
};

extern menu* current_menu; // nullptr if no menu is currently active.
