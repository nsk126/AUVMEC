#include <Servo.h>
#include <Wire.h>

//Global
int pwm_1;
int usec;

Servo ESC1;
Servo ESC2;

void setup() {

  Wire.begin(0x01);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
  
  ESC1.attach(3,1100,1900);
  ESC2.attach(5,1100,1900);
  delay(5000);

}

void loop() {

  usec = map(pwm_1,0,255,1200,1800);

  Serial.println(usec);
  
  ESC1.writeMicroseconds(usec);
  ESC2.writeMicroseconds(usec);
  
  delay(100);

}

void receiveEvent(int howMany){
  int x = Wire.read();
  pwm_1 = x;
}
