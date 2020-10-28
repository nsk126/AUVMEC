void set_PWR_MGMT(){
  Wire.beginTransmission(IMU);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission(true);
}

void i2cWrite(uint8_t Addr, uint8_t regAddr, uint8_t *data, uint8_t len){
  Wire.beginTransmission(Addr);
  Wire.write(regAddr);
  Wire.write(data,len);
  Wire.endTransmission(true);  
}

void i2cRead(uint8_t Addr, uint8_t regAddr, uint8_t *data, uint8_t len){
  Wire.beginTransmission(Addr);
  Wire.write(regAddr);
  Wire.endTransmission(false);
  Wire.requestFrom(Addr,len,true);
  for(uint8_t i=0; i < len; i++){
    if(Wire.available()){
      data[i] = Wire.read();
    }
  }
}

void i2cwriteByte(uint8_t Addr, uint8_t regAddr, uint8_t data){
  Wire.beginTransmission(Addr);
  Wire.write(regAddr);
  Wire.write(data);
  Wire.endTransmission(true); 
}
