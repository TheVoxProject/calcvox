#pragma once

#include "evox.hpp"
#include "history.h"
#include <Arduino.h>
#include <Encoder.h>
#include <Metro.h>
#include <cstring>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#define TalkSerial Serial4 //rx 16, tx 17

bool key_pressed(const std::string &key);
