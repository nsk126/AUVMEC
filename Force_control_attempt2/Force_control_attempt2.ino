#include <Servo.h>


Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;
Servo ESC5;
Servo ESC6;

int PWM[6];

void setup() {
  
  Serial.begin(9600);
  
  ESC1.attach(9,1100,1900);
  ESC2.attach(3,1100,1900);
  ESC3.attach(6,1100,1900);
  ESC4.attach(5,1100,1900);
  ESC5.attach(11,1100,1900);
  ESC6.attach(10,1100,1900);

  PWM[0] = PWM[1] = PWM[2] = PWM[3] = PWM[4] = PWM[5] = 1500;

  ESC1.writeMicroseconds(PWM[0]);
  ESC2.writeMicroseconds(PWM[1]);
  ESC3.writeMicroseconds(PWM[2]);
  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);

  delay(7000);

}


void loop() {

 // T1 - D9 - ESC4
 // T2 - D3 - ESC1
 // T3 - D6 - ESC3
 // T4 - D5 - ESC2
 // T5L - D11 - ESC6
 // T6R - D10 - ESC5

  Sway(1);
  ESC1.writeMicroseconds(PWM[0]);
  ESC2.writeMicroseconds(PWM[1]);
  ESC3.writeMicroseconds(PWM[2]);
  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);
  delay(7500);

  Stall();
  
  ESC1.writeMicroseconds(PWM[0]);
  ESC2.writeMicroseconds(PWM[1]);
  ESC3.writeMicroseconds(PWM[2]);
  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);
  
  delay(5000);
  
  Surge(1);
  ESC1.writeMicroseconds(PWM[0]);
  ESC2.writeMicroseconds(PWM[1]);
  ESC3.writeMicroseconds(PWM[2]);
  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);
  
//  Serial.print(PWM[0]);Serial.print("  ");
//  Serial.print(PWM[1]);Serial.print("  ");
//  Serial.print(PWM[2]);Serial.print("  ");
//  Serial.print(PWM[3]);Serial.print("  ");
//  Serial.print(PWM[4]);Serial.print("  ");
//  Serial.print(PWM[5]);Serial.println();
//  
  delay(7500);
  
  Stall();
  
  ESC1.writeMicroseconds(PWM[0]);
  ESC2.writeMicroseconds(PWM[1]);
  ESC3.writeMicroseconds(PWM[2]);
  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);
  
  delay(5000);

  

}

int force_to_pwm_cw(float force){
  if(force < 0){
    return 1460 + 198 * force + 46.8 * pow(force,2) + 7.19 * pow(force,3); 
  }else if(force > 0){
    return 1539 + 160 * force + (-31.3 * pow(force,2)) + 3.8 * pow(force,3);
  }else{
    return 1500;
  }
}

int force_to_pwm_ccw(float force){
  if (force < 0){  
    return 1461 + 160 * force + 31.3 * pow(force,2) + 3.8 * pow(force,3);
  }else if(force > 0){
    return 1540 + 198 * force + (-46.8 * pow(force,2)) + 7.19 * pow(force,3);
  }
}


void Surge(float force){
  // Total force = 2 * sqrt(2) * F per thruster
  // 0 is T1 1 is T2 are CCW
  PWM[0] = PWM[1] = force_to_pwm_ccw(force/(2.828));
  // 2 is T3 3 is T4 are CW
  PWM[2] = PWM[3] = force_to_pwm_cw(force/(2.828));
}

void Sway(float force){
  // Total force = 2 * sqrt(2) * F per thruster
  PWM[0] = force_to_pwm_ccw(-1 * force / (2.828));
  PWM[1] = force_to_pwm_ccw(force / 2.828);
  PWM[2] = force_to_pwm_cw(force / (2.828));
  PWM[3] = force_to_pwm_cw(-1 * force / 2.828);
  
  
}
void Heave(float force){
  
  PWM[4] = force_to_pwm_cw(-force/2);// T5
  PWM[5] = force_to_pwm_ccw(force/2);// T6
  
}

void Stall(){
  PWM[0] = PWM[1] = PWM[2] = PWM[3] = PWM[4] = PWM[5] = 1500;
}
