#pragma once

#include <Arduino.h>
#include <Encoder.h>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <sstream>
#include <iomanip>
#include "evox.hpp"
#include <Metro.h>
#include "history.h"

#define TalkSerial Serial4 //rx 16, tx 17

bool key_pressed(const std::string& key);
