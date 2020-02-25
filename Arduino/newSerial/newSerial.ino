/* GLOBAL */
String serialData;
float Kp,Ki,Kd,Depth,G;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  // For a RV
  randomSeed(analogRead(0));
}

void loop() {
}

void sendData(){
  float x = float(random(0,200))/100;
  Serial.println(x);
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
