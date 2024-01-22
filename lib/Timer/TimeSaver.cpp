#include "TimeSaver.h"

bool saveTimer(FS fs, String name, uint8_t hr, uint8_t min) {
    File tmr;
    String path = "/" + name + ".tmr";
    if(fs.exists(path)) if(!fs.remove(path)) return false;
    tmr = fs.open(path, FILE_WRITE);
    tmr.printf("%.2u:%.2u", hr, min);
    Serial.printf("%.2u:%.2u", hr, min);
    tmr.close();
    return true;
}

bool saveTimer(FS fs, String name, tm time) {
    File tmr;
    String path = "/" + name + ".tmr";
    if(fs.exists(path)) if(!fs.remove(path)) return false;
    tmr = fs.open(path, FILE_WRITE);
    tmr.printf("%.2d:%.2d", time.tm_hour, time.tm_min);
    tmr.close();
    return true;
}

tm getTimer(FS fs, String name) {
    String path = "/" + name + ".tmr";
    tm time;
    time.tm_hour = 69;
    if(!fs.exists(path)) return time;
    File tmr = fs.open(path);
    if(tmr.available()) {
        String temp = tmr.readString();
        tmr.close();
        time.tm_hour = temp.substring(0, temp.indexOf(':')).toInt();
        time.tm_min = temp.substring(temp.indexOf(':') + 1).toInt();
    }
    return time;
}