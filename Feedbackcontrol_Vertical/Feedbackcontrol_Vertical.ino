#include <Wire.h>
#include "MS5837.h"
#include <Servo.h>

// Servo init
Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;
Servo ESC5;
Servo ESC6;

//Pressure sensor object
MS5837 P;

// PID parameters
#define Kp 0
#define Ki 0
#define Kd 0

#define Ti 0
#define Td 0

// Global 
int PWM[6]; // PWM array for 6 thrusters
const float dt = 0.01;
float depthd = 0;
float depthi = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  while (!P.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  P.setModel(MS5837::MS5837_02BA);
  P.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  ESC1.attach(9,1100,1900);
  ESC2.attach(3,1100,1900);
  ESC3.attach(6,1100,1900);
  ESC4.attach(5,1100,1900);
  ESC5.attach(11,1100,1900);
  ESC6.attach(10,1100,1900);

  PWM[0] = PWM[1] = PWM[2] = PWM[3] = PWM[4] = PWM[5] = 1500; // Initial calibration

  ESC1.writeMicroseconds(PWM[0]);
  ESC2.writeMicroseconds(PWM[1]);
  ESC3.writeMicroseconds(PWM[2]);
  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);

  delay(7000);// 7s delay as per Blue Robotics
}

void loop() {
 // T1 - D9 - ESC4
 // T2 - D3 - ESC1
 // T3 - D6 - ESC3
 // T4 - D5 - ESC2
 // T5L - D11 - ESC6
 // T6R - D10 - ESC5

  P.read();
  float Depth = P.depth() + 0.675038; // in meters
  float required_Depth = 1.00; // set a particular value 

  //Prop Gain
  float prop = Kp * (required_Depth - Depth);

  //Derivative Gain
  float deri = Kd * Td * (Depth - depthd)/dt;

  //Integral Gain
  float inte = Ki * depthi/Ti;

  float AUV_Heave = Plant((prop + deri + inte));
  Heave(AUV_Heave,0);

  depthd = Depth;
  depthi += Depth;
}

float Plant(float pos){
  return 0; // replace with plant equations Position --> Thrust in kgf
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
void Surge(float force,int duration){
  // Total force = 2 * sqrt(2) * F per thruster
  // 0 is T1 1 is T2 are CCW
  PWM[0] = PWM[1] = force_to_pwm_ccw(force/(2.828));
  // 2 is T3 3 is T4 are CW
  PWM[2] = PWM[3] = force_to_pwm_cw(force/(2.828));
  ESC_write();
  delay(duration);
}
void Sway(float force,int duration){
  // Total force = 2 * sqrt(2) * F per thruster
  PWM[0] = force_to_pwm_ccw(-1 * force / (2.828));
  PWM[1] = force_to_pwm_ccw(force / 2.828);
  PWM[2] = force_to_pwm_cw(force / (2.828));
  PWM[3] = force_to_pwm_cw(-1 * force / 2.828);  
  ESC_write();
  delay(duration);
}
void Heave(float force,int duration){  
  PWM[4] = force_to_pwm_cw(-force/2);// T5
  PWM[5] = force_to_pwm_ccw(force/2);// T6  
  ESC_write();
  delay(duration);
}
void Yaw(float force,int duration){
  PWM[0] = force_to_pwm_ccw(force/4); 
  PWM[1] = force_to_pwm_ccw(-force/4);
  PWM[2] = force_to_pwm_cw(force/4);
  PWM[3] = force_to_pwm_cw(-force/4);
  ESC_write();
  delay(duration);
}
void Stall(int duration){
  PWM[0] = PWM[1] = PWM[2] = PWM[3] = PWM[4] = PWM[5] = 1500;
  ESC_write();
  delay(duration);
}
void Hstall(int duration){
  PWM[0] = PWM[1] = PWM[2] = PWM[3] = 1500;
  ESC_write();
  delay(duration);
}
void ESC_write(){
  ESC1.writeMicroseconds(PWM[0]);
  ESC2.writeMicroseconds(PWM[1]);
  ESC3.writeMicroseconds(PWM[2]);
  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);
}
void PWM_print(){
  Serial.print(PWM[0]);Serial.print("  ");
  Serial.print(PWM[1]);Serial.print("  ");
  Serial.print(PWM[2]);Serial.print("  ");
  Serial.print(PWM[3]);Serial.print("  ");
  Serial.print(PWM[4]);Serial.print("  ");
  Serial.print(PWM[5]);Serial.println();

}
