#pragma once
#include <Arduino.h>
#include <time.h>
#include "config.h"
#include <TimeSaver.h>

#ifndef TIMER_H
#define TIMER_H


typedef void (*TimerCallback) (void);

class Timer {
    public:
    Timer(tm t, TimerCallback callBack);
    boolean begin();
    void setTime(tm t);
    tm getTime();
    void end();
    private:
    void loop();
    tm curTime;
    tm triggerTime;
    TimerCallback cB;
    TaskHandle_t task = NULL;
};
#endif