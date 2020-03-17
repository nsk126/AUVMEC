#include <Wire.h>

#define sendSize 6
#define recSize 3

float Kp,Ki,Kd,Depth,G,State;
float data2Send[sendSize];
float data2get[recSize];
String serialData;

union floatToBytes {
	byte b[4];
	float f;
}convert;
 
void setup(){
	Wire.begin(0x08);
	Wire.onRequest(requestEvent);
	Wire.onReceive(receiveEvent);
	
	Serial.begin(115200);
  Serial.setTimeout(10);
}

void loop(){
	delay(10);
  
  data2Send[0] = Kp;
  data2Send[1] = Ki;
  data2Send[2] = Kd;
  data2Send[3] = Depth;
  data2Send[4] = G;
  data2Send[5] = State;
}

void requestEvent(){
  for(uint8_t i=0;i<sendSize;i++){
    convert.f = data2Send[i];
    Wire.write(convert.b,4);
  }
}

void receiveEvent(int howMany){
	while(1<Wire.available()){
    for(uint8_t j=0;j<recSize;j++){
      for(uint8_t i=0;i<4;i++){
        convert.b[i] = Wire.read();  
      }data2get[j] = convert.f;
    }
	}
}

void printdataWeget(){
  for(uint8_t i=0;i<recSize;i++){
    Serial.print(String(data2get[i],3)+",");
  }Serial.println();
}

void serialEvent(){
  serialData = Serial.readString();
  Kp = getKp(serialData);
  Ki = getKi(serialData);
  Kd = getKd(serialData);
  Depth = getDepth(serialData);
  G = getG(serialData);
  State = getState(serialData);
  printdataWeget();
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
  data.remove(data.indexOf("A"));
  return data.toFloat();
}
bool getState(String data){
  data.remove(0,data.indexOf("A")+1);
  return data.toInt();
}
