void i2csend(uint8_t *data, uint8_t len){
  Wire.beginTransmission(SLAVE);
  Wire.write(data,len);
  Wire.endTransmission(SLAVE);
}
