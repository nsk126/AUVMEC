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
float givenPos;

int PWM[6]; // PWM array for 6 thrusters


float Kpp,Kip,Kdp,Kpv,Kiv,Kdv;

// PID parameters
//float Kpp = 0.3; // no units
//float Kip = 0; // 1/s
//float Kdp = 0; // s
//float Kpv = 0; // no units
//float Kiv = 0; // 1/s
//float Kdv = 0; // s
float neu_g;

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

  delay(7000);// 7s delay as per Blue Robotics

  /* Timer */
  timer = micros();
}

  
void loop() {
  /* update Bar02 */
  sensor.read();

  /* System inputs */
//  float givenPos = 0.4;
  float givenVel = 0;
  float givenAcc = 0;
  
  float currentDepth = sensor.depth() + 0.644354; // in meters
  
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

  Wire.requestFrom(8,4);
  while(Wire.available()){
    convert.b[0] = Wire.read();
    convert.b[1] = Wire.read();
    convert.b[2] = Wire.read();
    convert.b[3] = Wire.read();
    Kpp = convert.f;
  }
  Wire.requestFrom(8,4);
  while(Wire.available()){
    convert.b[0] = Wire.read();
    convert.b[1] = Wire.read();
    convert.b[2] = Wire.read();
    convert.b[3] = Wire.read();
    Kip = convert.f;
  }
  Wire.requestFrom(8,4);
  while(Wire.available()){
    convert.b[0] = Wire.read();
    convert.b[1] = Wire.read();
    convert.b[2] = Wire.read();
    convert.b[3] = Wire.read();
    Kdp = convert.f;
  }
  Wire.requestFrom(8,4);
  while(Wire.available()){
    convert.b[0] = Wire.read();
    convert.b[1] = Wire.read();
    convert.b[2] = Wire.read();
    convert.b[3] = Wire.read();
    givenPos = convert.f;
  }
  Wire.requestFrom(8,4);
  while(Wire.available()){
    convert.b[0] = Wire.read();
    convert.b[1] = Wire.read();
    convert.b[2] = Wire.read();
    convert.b[3] = Wire.read();
    neu_g = convert.f;
  }

  Serial.print(String(Kpp)+" ");
  Serial.print(String(Kip)+" ");
  Serial.print(String(Kdp)+" ");
  Serial.print(String(givenPos)+" ");
  Serial.print(String(neu_g)+"\n");
  
  float g = neu_g * Grav;
  float Thrust_gen = Thrust(Acc_now,currentVel,g);

  Heave(Thrust_gen,0);

  //Sending float 1
  convert.f = currentDepth;
  i2csend(convert.b,4);

//  //Sending float 2
//  convert.f = errorPos;
//  i2csend(convert.b,4);
//
//  //Sending float 3
//  convert.f = errorVel;
//  i2csend(convert.b,4);
//
//  //Sending float 4
//  convert.f = Acc_now;
//  i2csend(convert.b,4);
//
//  //Sending float 5  
//  convert.f = Thrust_gen;
//  i2csend(convert.b,4);
  
  //Update
  prevDepth = currentDepth;
  prevVel = currentVel;
  
}
float Drag(float vel){
  return 73.573*(vel) - 1.1255;//+ 0.0876
}

float Thrust(float acc, float vel, float g){
  return (M * acc + Drag(vel) * vel + g) / Grav;  
}
