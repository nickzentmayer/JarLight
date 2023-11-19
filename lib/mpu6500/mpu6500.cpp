#include "mpu6500.h"

MPU::MPU(uint8_t ad) {
    address = ad;
    Wire.begin();
}

void MPU::writeByte(uint8_t subAddress, uint8_t data) {
  Wire.beginTransmission(address); // Initialize the Tx buffer
  Wire.write(subAddress); // Put slave register address in Tx buffer
  Wire.write(data); // Put data in Tx buffer
  Wire.endTransmission(); // Send the Tx buffer
}

uint8_t MPU::readByte(uint8_t subAddress) {
  uint8_t data; // `data` will store the register data   
  Wire.beginTransmission(address); // Initialize the Tx buffer
  Wire.write(subAddress); // Put slave register address in Tx buffer
  Wire.endTransmission(false); // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, (uint8_t) 1); // Read one byte from slave register address 
  data = Wire.read(); // Fill Rx buffer with result
  return data; // Return data read from slave register
}

void MPU::setupInt() {
  //PWR MGMT 1
    writeByte(0x6B, B10001000);
    //configure INT pin
    writeByte(0x37, B00010000);
    //PWRMGMT 2
    writeByte(0x6C, B00000111);
    //following datasheet ACCELL CONFIG 2
    writeByte(0x1D, B00000001);
    //enable motion int
    writeByte(0x38, B01000000);
    //motion control
    writeByte(0x69, B11000000);
    //threshold
    writeByte(0x1F, MPUTHRESHOLD);
    //wakeup freq
    writeByte(0x1E, B00001110);
    //set cycle to 1
    writeByte(0x6B, B00101000);
}

void MPU::clearInt() {
  readByte(0x38);
}

