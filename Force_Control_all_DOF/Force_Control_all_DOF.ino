#include <Servo.h>
#include <Wire.h>

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;
Servo ESC5;
Servo ESC6;

byte Move[4];
int PWM[6];


void setup() {
  Wire.begin(0x01);
  Wire.onReceive(receiveEvent);

  ESC1.attach(3,1100,1900);
  ESC2.attach(5,1100,1900);
  ESC3.attach(6,1100,1900);
  ESC4.attach(9,1100,1900);
  ESC5.attach(10,1100,1900);
  ESC6.attach(11,1100,1900);

  Serial.begin(9600);
  Serial.println("Ready .. !");

  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  ESC4.writeMicroseconds(1500);
  ESC5.writeMicroseconds(1500);
  ESC6.writeMicroseconds(1500);

  delay(5000);
}

void loop() {

  // 4-DOF
  // Required Motion -> Surge,Heave,Sway,Yaw.
  // Max force in any DOF  = F * 2 * sqrt(2) = 8.2024 , -8.2024

  int pwm1,pwm2,pwm3,pwm4,pwm5,pwm6;
  
  float xval = mapfloat((Move[0] << 8 | Move[1] & 0xFF),0,1023,-8.2024,8.2024);
  float yval = mapfloat((Move[2] << 8 | Move[3] & 0xFF),0,1023,-5.8,5.8);
  Serial.print(xval);Serial.print("\t");Serial.print(yval);Serial.print("  ");

  pwm1 = pwm2 = pwm3 = pwm4 = force_to_pwm(xval/(2*sqrt(2)));
  Serial.print(pwm1);Serial.print("  ");

  pwm5 = force_to_pwm(yval/2);
  pwm6 = force_to_pwm(-1 * yval/2);

  Serial.print(pwm5);Serial.print("  ");Serial.print(pwm6);Serial.println();

  ESC1.writeMicroseconds(pwm1);
  ESC2.writeMicroseconds(pwm2);
  ESC3.writeMicroseconds(pwm3);
  ESC4.writeMicroseconds(pwm4);
  ESC5.writeMicroseconds(pwm5);
  ESC6.writeMicroseconds(pwm6);
  
}
  


int force_to_pwm(float force){
  if(force < -0.2){
    return 1460 + 198 * force + 46.8 * pow(force,2) + 7.19 * pow(force,3); 
  }else if(force > 0.2){
    return 1539 + 160 * force + (-31.3 * pow(force,2)) + 3.8 * pow(force,3);
  }else{
    return 1500;
  }
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

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
