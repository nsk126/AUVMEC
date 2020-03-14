#include <Servo.h>

//Global

Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;
Servo ESC5;
Servo ESC6;

void setup() {
  
  Serial.begin(9600);
  
  ESC1.attach(3,1100,1900);
  ESC2.attach(5,1100,1900);
  ESC3.attach(6,1100,1900);
  ESC4.attach(9,1100,1900);
  ESC5.attach(10,1100,1900);
  ESC6.attach(11,1100,1900);
//
  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  ESC4.writeMicroseconds(1500);
  ESC5.writeMicroseconds(1500);
  ESC6.writeMicroseconds(1500);

  delay(6000);
}

void loop() {
  

  int pwm1,pwm2,pwm3,pwm4,pwm5,pwm6;
  
  float xval = 0.00;
  float yval = 0.55;

  pwm1 = pwm2 = pwm3 = pwm4 = force_to_pwm(xval/(2*sqrt(2)));
  pwm5 = force_to_pwm(yval/2);
  pwm6 = force_to_pwm(-1 * yval/2);


  ESC1.writeMicroseconds(pwm1);
  ESC2.writeMicroseconds(pwm2);
  ESC3.writeMicroseconds(pwm3);
  ESC4.writeMicroseconds(pwm4);
  ESC5.writeMicroseconds(pwm5);
  ESC6.writeMicroseconds(pwm6);  

  delay(8000);
  
  xval = 0.00;
  yval = 0.45;

  pwm1 = pwm2 = pwm3 = pwm4 = force_to_pwm(xval/(2*sqrt(2)));
  pwm5 = force_to_pwm(yval/2);
  pwm6 = force_to_pwm(-1 * yval/2);
  
  ESC1.writeMicroseconds(pwm1);
  ESC2.writeMicroseconds(pwm2);
  ESC3.writeMicroseconds(pwm3);
  ESC4.writeMicroseconds(pwm4);
  ESC5.writeMicroseconds(pwm5);
  ESC6.writeMicroseconds(pwm6);  

  delay(20000);
  
  xval = 1.00;
  yval = 0.45;

  pwm1 = pwm2 = pwm3 = pwm4 = force_to_pwm(xval/(2*sqrt(2)));
  pwm5 = force_to_pwm(yval/2);
  pwm6 = force_to_pwm(-1 * yval/2);
  
  ESC1.writeMicroseconds(pwm1);
  ESC2.writeMicroseconds(pwm2);
  ESC3.writeMicroseconds(pwm3);
  ESC4.writeMicroseconds(pwm4);
  ESC5.writeMicroseconds(pwm5);
  ESC6.writeMicroseconds(pwm6);  

  delay(30000);

  xval = 0.00;
  yval = 0.45;

  pwm1 = pwm2 = pwm3 = pwm4 = force_to_pwm(xval/(2*sqrt(2)));
  pwm5 = force_to_pwm(yval/2);
  pwm6 = force_to_pwm(-1 * yval/2);
  
  ESC1.writeMicroseconds(pwm1);
  ESC2.writeMicroseconds(pwm2);
  ESC3.writeMicroseconds(pwm3);
  ESC4.writeMicroseconds(pwm4);
  ESC5.writeMicroseconds(pwm5);
  ESC6.writeMicroseconds(pwm6);  

  delay(10000);

  
  ESC1.writeMicroseconds(1500);
  ESC2.writeMicroseconds(1500);
  ESC3.writeMicroseconds(1500);
  ESC4.writeMicroseconds(1500);
  ESC5.writeMicroseconds(1500);
  ESC6.writeMicroseconds(1500);  

  delay(10000);
  
  
 
 
    

}

int force_to_pwm(float force){
  if(force < -0.01){
    return 1460 + 198 * force + 46.8 * pow(force,2) + 7.19 * pow(force,3); 
  }else if(force > 0.01){
    return 1539 + 160 * force + (-31.3 * pow(force,2)) + 3.8 * pow(force,3);
  }else{
    return 1500;
  }
}
