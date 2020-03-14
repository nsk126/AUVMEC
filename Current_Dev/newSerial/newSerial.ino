#include <Wire.h>

/* GLOBAL */
String serialData;
float Kp,Ki,Kd,Depth,G,currentD;

union floatToBytes {
  byte b[4];
  float f;
} convert;

void setup() {
  Wire.begin(8);
  Serial.begin(115200);
  Serial.setTimeout(10);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  // For a RV
  randomSeed(analogRead(0));
}

void requestEvent() {
  convert.f = Kp;
  Wire.write(convert.b,4);
  convert.f = Ki;
  Wire.write(convert.b,4);
  convert.f = Kd;
  Wire.write(convert.b,4);
  convert.f = Depth;
  Wire.write(convert.b,4);
  convert.f = G;
  Wire.write(convert.b,4);
    
}

void loop() {
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { 
    convert.b[0] = Wire.read();
    convert.b[1] = Wire.read();
    convert.b[2] = Wire.read();
    convert.b[3] = Wire.read();
    currentD = convert.f;
  }
}

void sendData(){
//  float x = float(random(0,200))/100;
  Serial.println(currentD);
}
void serialEvent(){
  serialData = Serial.readString();
  Kp = getKp(serialData);
  Ki = getKi(serialData);
  Kd = getKd(serialData);
  Depth = getDepth(serialData);
  G = getG(serialData);


  sendData();
}

float getKp(String data){
  data.remove(data.indexOf("Y"));
  data.remove(data.indexOf("X"),1);
  return data.toFloat();
}
float getKi(String data){
  data.remove(0,data.indexOf("Y")+1);
  data.remove(data.indexOf("Z"));
  return data.toFloat();
}
float getKd(String data){
  data.remove(0,data.indexOf("Z")+1);
  data.remove(data.indexOf("W"));
  return data.toFloat();
}
float getDepth(String data){
  data.remove(0,data.indexOf("W")+1);
  data.remove(data.indexOf("U"));
  return data.toFloat();
}
float getG(String data){
  data.remove(0,data.indexOf("U")+1);
  return data.toFloat();
}
void prin(){
  Serial.print(String(Kp)+"\t");
  Serial.print(String(Ki)+"\t");
  Serial.print(String(Kd)+"\t");
  Serial.print(String(Depth)+"\t");
  Serial.print(String(G)+"\n");
  
}
