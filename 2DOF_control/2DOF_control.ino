#include <Servo.h>
#include <Wire.h>

//Global
int pwm_1;
int usec_x1;
int usec_x2;
int usec_y;
byte Move[2];

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;
Servo ESC5;
Servo ESC6;

void setup() {

  Wire.begin(0x01);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
  
  ESC1.attach(3,1100,1900);
  ESC2.attach(5,1100,1900);
  ESC3.attach(6,1100,1900);
  ESC4.attach(9,1100,1900);
  ESC5.attach(11,1100,1900);
  ESC6.attach(12,1100,1900);

  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  ESC4.writeMicroseconds(1500);
  ESC5.writeMicroseconds(1500);
  ESC6.writeMicroseconds(1500);

  delay(5000);

}

void loop() {

  usec_x1 = map(Move[0],0,255,1200,1800);
  usec_x2 = 3000 - usec_x1;
  usec_y = map(Move[1],0,255,1200,1800);

  Serial.print(usec_x1);Serial.print(" ");Serial.print(usec_x2);Serial.print(" ");Serial.print(usec_y);Serial.print(" ");
  Serial.println();
 
  
  ESC1.writeMicroseconds(usec_x1);
  ESC2.writeMicroseconds(usec_x1);
  ESC3.writeMicroseconds(usec_x2);
  ESC4.writeMicroseconds(usec_x2);
  ESC5.writeMicroseconds(usec_y);
  ESC6.writeMicroseconds(usec_y);
  
  delay(200);

}

void receiveEvent(int howMany){
  while (1 < Wire.available()) { // loop through all but the last
    for(int i = 0; i < howMany;i++){
      Move[i] = Wire.read(); 
//      Serial.print(Move[i]);Serial.print(" ");  
    }
//    int c = Wire.read();Serial.print(c);
//    Serial.println();
  }

}
