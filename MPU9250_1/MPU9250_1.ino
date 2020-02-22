#include <Wire.h>

#define IMU 0x68
#define PWR_MGMT_1 0x6B
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H 0x3B 
#define INT_PIN_CFG 0x37

#define AK8963 0x0C 
#define CNTL1 0x0A
#define HXL 0x03


double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
double MagX, MagY, MagZ;
int16_t tempRaw;

uint8_t i2cBuffer[14];

void setup() {
  Serial.begin(9600);

  Wire.begin();

  /*
   * Enable 20MHz Oscillator
   * Disable Sleep & Standby Modes
   */
  set_PWR_MGMT();
  /*
   * Setting Gyro, Accel Configurations
   * FS_SEL = 0 
   * Gyro = ±250º/s = 131 LSB/(º/s)
   * AFS_SEL=0
   * Accel = ±2g = 16,384 LSB/g
   */
  i2cBuffer[0] = 0x00; // Gyro
  i2cBuffer[1] = 0x00; // Accel
  i2cWrite(IMU,GYRO_CONFIG,i2cBuffer,2);  

  /*Enable MAG*/
  i2cwriteByte(IMU,INT_PIN_CFG,0X02);
  /*
   * Write to Control1 Register, CNTL1
   * Enable 16-Bit output
   * Enable Continuous measurement mode 2
   */
  i2cwriteByte(AK8963,CNTL1,0X16);
}

void loop() {
  /* Update Accel & Gyro Registers */
  i2cRead(IMU,ACCEL_XOUT_H,i2cBuffer,14);
  accX = (int16_t)((i2cBuffer[0] << 8) | i2cBuffer[1]);
  accY = (int16_t)((i2cBuffer[2] << 8) | i2cBuffer[3]);
  accZ = (int16_t)((i2cBuffer[4] << 8) | i2cBuffer[5]);
  tempRaw = (int16_t)((i2cBuffer[6] << 8) | i2cBuffer[7]);
  gyroX = (int16_t)((i2cBuffer[8] << 8) | i2cBuffer[9]);
  gyroY = (int16_t)((i2cBuffer[10] << 8) | i2cBuffer[11]);
  gyroZ = (int16_t)((i2cBuffer[12] << 8) | i2cBuffer[13]);;
  /* Scaling */
  accX = accX/16384;
  accY = accY/16384;
  accZ = accZ/16384;
  gyroX = gyroX/131;
  gyroY = gyroY/131;
  gyroZ = gyroZ/131;

  /*Mag*/
//  i2cRead(AK8963,0x02,i2cBuffer,1);
//  Serial.println(i2cBuffer[0],BIN);
  i2cRead(AK8963,HXL,i2cBuffer,6);
  MagX = (int16_t)((i2cBuffer[0] << 8) | i2cBuffer[1]);
  MagY = (int16_t)((i2cBuffer[2] << 8) | i2cBuffer[3]);
  MagZ = (int16_t)((i2cBuffer[4] << 8) | i2cBuffer[5]); 

//  Serial.print(accX);Serial.print("\t");
//  Serial.print(accY);Serial.print("\t");
//  Serial.print(accZ);Serial.print("\t");
//  Serial.print(tempRaw);Serial.print("\t");
//  Serial.print(gyroX);Serial.print("\t");
//  Serial.print(gyroY);Serial.print("\t");
//  Serial.print(gyroZ);Serial.println("\t");
  Serial.print(MagX);Serial.print("\t"); 
  Serial.print(MagY);Serial.print("\t"); 
  Serial.print(MagZ);Serial.println("\t"); 
  
}
