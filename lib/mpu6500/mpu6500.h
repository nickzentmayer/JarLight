#include <Arduino.h>
#include <Wire.h>

class MPU 
{
    public:
    MPU(uint8_t addr);
    void writeByte(uint8_t subAddress, uint8_t data);
    uint8_t readByte(uint8_t subAddress);
    void setupInt();
    void clearInt();
    private:
    uint8_t address;
};