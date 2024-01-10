#include "Timer.h"

int id = 0;

void timerTask(void* pvParameters) {
    Timer* thisTimer = static_cast<Timer*>(pvParameters);
    for(;;) {
        
        //thisTimer->loop();
        vTaskDelay(1000);
    }
}

//chatgpt w
bool isDST(int year, int month, int day, int hour, int offset) {
  if (month < 3 || month > 11) {
    // Not in DST, January, February, and December are definitely not in DST.
    return false;
  }
  if (month > 3 && month < 11) {
    // In DST, April to October are definitely in DST.
    return true;
  }
  
  // Check if it's the second Sunday in March (start of DST) or first Sunday in November (end of DST)
  int previousSunday = day - (offset % 7);
  if ((month == 3 && previousSunday >= 8) || (month == 11 && previousSunday <= 0)) {
    // Within the DST period
    return true;
  }
  
  return false;
}

Timer::Timer(tm t, TimerCallback callBack) {
    cB = callBack;
    triggerTime = t;
}

boolean Timer::begin() {
    if(task != NULL) return false;
        configTime(3600*TIMEZONE, 0, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
        if(getLocalTime(&curTime, 5000)) {
            if(isDST(curTime.tm_year, curTime.tm_mon, curTime.tm_mday, curTime.tm_hour, TIMEZONE)) 
                configTime(3600*TIMEZONE, -3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
            xTaskCreatePinnedToCore(
                    [](void* pvParameters){
                        Timer* thisTimer = static_cast<Timer*>(pvParameters);
                        for(;;) {
                            thisTimer->loop();
                            vTaskDelay(1000);
                        }
                    },
                    String("Timer " + String(id++)).c_str(),
                    2048,
                    this,
                    1,
                    &task,
                    1
                );
                //cB();
            return true;
        }
        return false;
}

void Timer::setTime(tm t) {
    triggerTime = t;
}

void Timer::end() {
    if(task != NULL) vTaskDelete(task);
}

tm Timer::getTime() {
    return curTime;
}

void Timer::loop() {
    getLocalTime(&curTime);
    if(curTime.tm_hour == triggerTime.tm_hour && curTime.tm_min == triggerTime.tm_min) {
        cB();
    vTaskDelay(61000);
    }
}