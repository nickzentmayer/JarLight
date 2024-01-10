#pragma once

#include <Timer.h>
#include <FS.h>

boolean saveTimer(FS fs, String name, uint8_t hr, uint8_t min);
bool saveTimer(FS fs, String name, tm time);
tm getTimer(FS fs, String name);
