#include <Wire.h>


union floatToBytes {
  byte b[4];
  float f;
} convert;

float var[5];
float pot;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);

  pot = mapfloat(analogRead(0),0,1023,0.14,0.64);

  Serial.print(String(var[0]) + "\t");
  Serial.print(String(var[1]) + "\t");
  Serial.print(String(var[2]) + "\t");
  Serial.print(String(var[3]) + "\t");
  Serial.print(String(var[4]) + "\t");
  Serial.print(String(pot) + "\n");
  

  
}

void receiveEvent(int howMany) {
  static int i = 0;
  while (1 < Wire.available()) { // loop through all but the last
    convert.b[0] = Wire.read();
    convert.b[1] = Wire.read();
    convert.b[2] = Wire.read();
    convert.b[3] = Wire.read();

    var[i] = convert.f;
    i++;
    if(i>=5){i=0;}
  }
}
void requestEvent() {
  convert.f = pot;
  Wire.write(convert.b,4);
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
