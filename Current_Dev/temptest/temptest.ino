#include <Wire.h>
#include "MS5837.h"
#include <Servo.h>

#define M 30.12
#define SLAVE 8
#define Grav 9.80665

// Servo init
Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;
Servo ESC5;
Servo ESC6;

MS5837 sensor;

/* Global */
uint32_t timer;
float prevVel = 0;
float prevDepth = 0;

float errorPos_I = 0;
float errorVel_I = 0;
float errorPos_D = 0;
float errorVel_D = 0;
float errorPos_prev = 0;
float errorVel_prev = 0;

int PWM[6]; // PWM array for 6 thrusters


// PID parameters
float Kpp = 0.5; // no units
float Kip = 0; // 1/s
float Kdp = 0.01; // s
float Kpv = 0; // no units
float Kiv = 0; // 1/s
float Kdv = 0; // s
float neu_g = 0.50;

/* T1 - D9 - ESC4
 * T2 - D3 - ESC1
 * T3 - D6 - ESC3
 * T4 - D5 - ESC2
 * T5L - D11 - ESC6
 * T6R - D10 - ESC5
 */

union floatToBytes {
  byte b[4];
  float f;
} convert;
 
void setup() {
  Serial.begin(9600);
  Wire.begin();

  /* Pressure Sensor initiation - Will not work without this */
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar02: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  sensor.setModel(MS5837::MS5837_02BA); // For Bar02
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

//  ESC1.attach(9, 1100, 1900);
//  ESC2.attach(3, 1100, 1900);
//  ESC3.attach(6, 1100, 1900);
//  ESC4.attach(5, 1100, 1900);
  ESC5.attach(11, 1100, 1900);
  ESC6.attach(10, 1100, 1900);

  PWM[0] = PWM[1] = PWM[2] = PWM[3] = PWM[4] = PWM[5] = 1500; // Initial calibration

//  ESC1.writeMicroseconds(PWM[0]);
//  ESC2.writeMicroseconds(PWM[1]);
//  ESC3.writeMicroseconds(PWM[2]);
//  ESC4.writeMicroseconds(PWM[3]);
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);

  delay(120000);// 7s delay as per Blue Robotics

  /* Timer */
  timer = micros();
}

  
void loop() {
  /* update Bar02 */
  sensor.read();

  /* System inputs */
  float givenPos = 0.3;
  float givenVel = 0;
  float givenAcc = 0;
  
  float currentDepth = sensor.depth() + 0.6386; // in meters
  
  double dt = (double)(micros() - timer)/1000000;
  timer = micros();

  float currentVel = (currentDepth - prevDepth)/dt;  
  float accel = (currentVel - prevVel)/dt;

  /* Error */
  float errorPos = givenPos - currentDepth;
  float errorVel = givenVel - currentVel;
  if(errorPos<0){errorVel = -1 * errorVel;}
  errorPos_I = (errorPos_I + errorPos)*dt;
  errorVel_I = (errorVel_I + errorVel)*dt;

  errorPos_D = (errorPos - errorPos_prev)/dt;
  errorVel_D = (errorVel - errorVel_prev)/dt;

  float Acc_now = givenAcc + 
                 ( Kpv*errorVel + Kiv*errorVel_I + Kdv*errorVel_D ) + 
                 ( Kpp*errorPos + Kip*errorPos_I + Kdp*errorPos_D ) ; 

//  Wire.requestFrom(8,4);
//  while(Wire.available()){
//    convert.b[0] = Wire.read();
//    convert.b[1] = Wire.read();
//    convert.b[2] = Wire.read();
//    convert.b[3] = Wire.read();
//
//    neu_g = convert.f;
//  }

  float g = neu_g * Grav;
  float Thrust_gen = Thrust(Acc_now,currentVel,g);

  Heave(Thrust_gen,0);

  
  //Update
  prevDepth = currentDepth;
  prevVel = currentVel;
  
}
float Drag(float vel){
  return 73.573*(vel) - 1.1255 + 0.0876;
}

float Thrust(float acc, float vel, float g){
  return (M * acc + Drag(vel) * vel + g) / Grav;  
}

int force_to_pwm_cw(float force){
  if(force < 0){
    return 1464 + 169.2 * force + 34.8 * pow(force,2) + 4.506 * pow(force,3); 
  }else if(force > 0){
    return 1536 + 134.7 * force - 22.07 * pow(force,2) + 2.183 * pow(force,3);
  }else{
    return 1500;//done
  }
}
int force_to_pwm_ccw(float force){
  if (force < 0){  
    return 1536 - 169.2 * force - 34.8 * pow(force,2) - 4.506 * pow(force,3);
  }else if(force > 0){
    return 1464 - 134.7 * force + 22.07 * pow(force,2) + -2.183 * pow(force,3);
  }else{
    return 1500;
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
  PWM[4] = constrain(PWM[4],1380,1616);
  PWM[5] = constrain(PWM[5],1380,1616);
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

void i2csend(uint8_t *data, uint8_t len){
  Wire.beginTransmission(SLAVE);
  Wire.write(data,len);
  Wire.endTransmission(SLAVE);
}
