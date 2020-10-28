#include <Wire.h>

#define sendSize 8
#define recSize 5

float Kp,Ki,Kd,Depth,h_Thrust,State,hState,vState;
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
  
  data2Send[0] = Depth;
  data2Send[1] = Kp;
  data2Send[2] = Ki;
  data2Send[3] = Kd;
  data2Send[4] = h_Thrust;
  data2Send[5] = State;
  data2Send[6] = hState;
  data2Send[7] = vState;

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
  Depth = getDepth(serialData);
  Kp = getKp(serialData);
  Ki = getKi(serialData);
  Kd = getKd(serialData);
  h_Thrust = get_h_thrust(serialData);
  State = get_status(serialData);
  hState = get_Hstatus(serialData);
  vState = get_Vstatus(serialData);
  
  printdataWeget();
}


float getDepth(String data){
  data.remove(data.indexOf("B"));
  data.remove(data.indexOf("A"),1);
  return data.toFloat();
}
float getKp(String data){
  data.remove(0,data.indexOf("B")+1);
  data.remove(data.indexOf("C"));
  return data.toFloat();
}
float getKi(String data){
  data.remove(0,data.indexOf("C")+1);
  data.remove(data.indexOf("D"));
  return data.toFloat();
}
float getKd(String data){
  data.remove(0,data.indexOf("D")+1);
  data.remove(data.indexOf("E"));
  return data.toFloat();
}
float get_h_thrust(String data){
  data.remove(0,data.indexOf("E")+1);
  data.remove(data.indexOf("F"));
  return data.toFloat();
}
bool get_status(String data){
  data.remove(0,data.indexOf("F")+1);
  data.remove(data.indexOf("G"));
  return data.toFloat();
}
bool get_Hstatus(String data){
  data.remove(0,data.indexOf("G")+1);
  data.remove(data.indexOf("H"));
  return data.toFloat();
}
bool get_Vstatus(String data){
  data.remove(0,data.indexOf("H")+1);
  return data.toInt();
}
