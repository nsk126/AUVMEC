#include "headers/i2cfunc.h"
#include "headers/mpu9250_lib.h"
#include <Wire.h>

bool checkWHO_AM_I(){

  
  uint8_t i2cbuff[1];
  i2cread_Arr(MPU_9250,WHO_AM_I,1,i2cbuff);
  return i2cbuff[1] == WHO_AM_I_CONTENT;
  
}
