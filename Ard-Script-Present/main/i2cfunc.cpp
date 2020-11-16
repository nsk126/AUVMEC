#include "headers/i2cfunc.h"
#include <Wire.h>

#define MPU_9250 0x68
#define WHO_AM_I 0x75
#define WHO_AM_I_CONTENT 0x71

void i2cread_Arr(int slaveAddr, int PointAddr, int Regnum, uint8_t *data){

  // Connect to Slaver Device
  Wire.beginTransmission(slaveAddr);

  // Pointer to device addr
  Wire.write(PointAddr);
  Wire.endTransmission(false);

  //Req data from slave
  Wire.requestFrom(slaveAddr, Regnum, true);

  for(uint8_t i = 0; i < Regnum; i++){
    data[i] = Wire.read();
  }
}
