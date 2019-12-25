#include <Servo.h>


Servo ESC1;

Servo ESC2;
Servo ESC3;

Servo ESC4;



void setup() {
  // put your setup code here, to run once:
  ESC1.attach(9,1100,1900);
  ESC2.attach(3,1100,1900);
  ESC3.attach(6,1100,1900);
  ESC4.attach(5,1100,1900);

  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  ESC4.writeMicroseconds(1500);

  delay(7000);
  
}

void loop() {
  ESC1.writeMicroseconds(1600);
  ESC2.writeMicroseconds(1600);
  ESC3.writeMicroseconds(1600);
  ESC4.writeMicroseconds(1600);

  delay(50);
}
