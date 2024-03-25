#include "menu.h"

menu* current_menu = nullptr;

menu::menu(const std::string& intro_msg) {
    intro = intro_msg;
    current_menu = this;
}

menu* menu::add_item(const std::string& title, const std::string& id, bool clickable) {
    items.push_back({title, id, clickable});
    return this;
}

int menu::loop() {
    if (key_pressed("="))
        TalkSerial.println("You are in a menu.");
    return -1;
}
