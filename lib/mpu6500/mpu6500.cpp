#include "mpu6500.h"

MPU::MPU(uint8_t ad) {
    address = ad;
}

void MPU::writeByte(uint8_t subAddress, uint8_t data) {
  Wire.begin();
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
    writeByte(0x6B, readByte(0x6B) & 0x1F);
    writeByte(0x68, 0x07);
    writeByte(0x6C, 0x07);
    writeByte(0x1D, 0x01);
    writeByte(0x38, 0x40);
    writeByte(0x69, 0xC0);
    writeByte(0x1F, 0xAA);
    writeByte(0x1E, 0x02);
    writeByte(0x37, 0x90);
    writeByte(0x6B, readByte(0x6B) | 0x20);
}

