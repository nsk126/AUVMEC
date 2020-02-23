#include <Servo.h>

int x = 1500;
int y = 1500;
int a = 1;

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;

void setup() {
  Serial.begin(9600);
  
  ESC1.attach(3,1100,1900);
  ESC2.attach(5,1100,1900);
  ESC3.attach(9,1100,1900);
  ESC4.attach(6,1100,1900);

  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  ESC4.writeMicroseconds(1500);
  delay(15000);
}

void loop() {

  for(int i = 1500;i<1700;i+=4){
    x += 4;
    y -= 4;
    ESC1.writeMicroseconds(x);
    ESC2.writeMicroseconds(x);
    ESC3.writeMicroseconds(y);
    ESC4.writeMicroseconds(y);
    Serial.print("ESC 1 2 = ");Serial.print(x);
    Serial.print("\t");
    Serial.print("ESC 3 4 = ");Serial.println(y);
    delay(300);
}

  for(int i = 1500;i<1700;i+=4){
    x -= 4;
    y += 4;
    ESC1.writeMicroseconds(x);
    ESC2.writeMicroseconds(x);
    ESC3.writeMicroseconds(y);
    ESC4.writeMicroseconds(y);
    Serial.print("ESC 1 2 = ");Serial.print(x);
    Serial.print("\t");
    Serial.print("ESC 3 4 = ");Serial.println(y);
    delay(300);
}


  


}



// while(a){
//    
//    ESC1.writeMicroseconds(x);
//    ESC2.writeMicroseconds(x);
//    ESC3.writeMicroseconds(y);
//    ESC4.writeMicroseconds(y);
//
//    x += 4;
//    y -= 4;
//    
//    Serial.print("ESC 1 2 = ");Serial.print(x);
//    Serial.print("\t");
//    Serial.print("ESC 3 4 = ");Serial.println(y);
//    delay(300);
//
//    if(x > 1700 || y < 1300){
//      while(1){
//        ESC1.writeMicroseconds(1500);
//        ESC2.writeMicroseconds(1500);
//        ESC3.writeMicroseconds(1500);
//        ESC4.writeMicroseconds(1500);
//      }
//    }
//  }
