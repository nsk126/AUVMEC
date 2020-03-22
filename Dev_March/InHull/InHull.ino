#include <Wire.h>
#include "MS5837.h"
#include <Servo.h>

#define Out 0x08
#define recSize 6
#define sendSize 6
#define Grav 9.80665
#define M 30.12

Servo ESC5;
Servo ESC6;

MS5837 sensor;

uint32_t timer;
float prevVel = 0;
float prevDepth = 0;
float givenPos;

float errorPos_I = 0;
float errorVel_I = 0;
float errorPos_D = 0;
float errorVel_D = 0;
float errorPos_prev = 0;
float errorVel_prev = 0;

int PWM[6]; // PWM array for 6 thrusters

/* T1 - D9 - ESC4
 * T2 - D3 - ESC1
 * T3 - D6 - ESC3
 * T4 - D5 - ESC2
 * T5L - D11 - ESC6
 * T6R - D10 - ESC5
 */

// PID parameters
float Kp = 0; // no units
float Ki = 0; // 1/s
float Kd = 0; // s
float neu_g = 0.35;

float recData[recSize];
float sendData[sendSize];

//float sendData[sendSize] = {
//  1.431,9.867,5.332,7.234
//};

//byte statusLed    = 13;
//byte sensorInterrupt = 0;  // 0 = digital pin 2
//byte sensorPin       = 2;
//float calibrationFactor = 0.45/0.57;
//volatile byte pulseCount;  
//float flowRate;
//unsigned int flowMilliLitres;
//unsigned long totalMilliLitres;
//float vel;
//unsigned long oldTime;

union floatToBytes {
	byte b[4];
	float f;
}convert;

void setup(){
	Wire.begin();
	Serial.begin(9600);
  Serial.println("Start");
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar02: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  sensor.setModel(MS5837::MS5837_02BA); // For Bar02
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  ESC5.attach(11, 1100, 1900);
  ESC6.attach(10, 1100, 1900);
  PWM[0] = PWM[1] = PWM[2] = PWM[3] = PWM[4] = PWM[5] = 1500; // Initial calibration
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);


//  pinMode(statusLed, OUTPUT);
//  digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
//  
//  pinMode(sensorPin, INPUT);
//  digitalWrite(sensorPin, HIGH);
//
//  pulseCount        = 0;
//  flowRate          = 0.0;
//  flowMilliLitres   = 0;
//  totalMilliLitres  = 0;
//  oldTime           = 0;
//  vel=0;
//  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
//  
//  delay(5000);
  timer = micros();
}

void loop(){
  
  sensor.read();
  
	i2cMasterRead(Out,4 * recSize);
//	sMoniter();

//  if((millis() - oldTime) > 1000)    // Only process counters once per second
//  { 
//    detachInterrupt(sensorInterrupt);
//    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
//    oldTime = millis();
//    flowMilliLitres = (flowRate / 60) * 1000;
//    vel=flowRate*10/(3.14*1.905*1.905*60);
//    totalMilliLitres += flowMilliLitres;
//      
//    unsigned int frac;
//    sendData[2] = vel;
//    pulseCount = 0;
//    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
//  }


/* System inputs */
  givenPos = recData[0];
  Kp = recData[1];
  Ki = recData[2];
  Kd = recData[3];
  neu_g = recData[4];
  
  float givenVel = 0;
  float givenAcc = 0;
  
  float currentDepth = sensor.depth() + 0.6853; // in meters
//  Serial.println(currentDepth);
  sendData[0] = currentDepth;
  double dt = (double)(micros() - timer)/1000000;
  timer = micros();

  float currentVel = (currentDepth - prevDepth)/dt;
  sendData[1] = currentVel;
  float accel = (currentVel - prevVel)/dt;

  /* Error */
  float errorPos = givenPos - currentDepth;
  float errorVel = givenVel - currentVel;
  if(errorPos<0){errorVel = -1 * errorVel;}
  errorPos_I = (errorPos_I + errorPos)*dt;
  errorVel_I = (errorVel_I + errorVel)*dt;

  errorPos_D = (errorPos - errorPos_prev)/dt;
  errorVel_D = (errorVel - errorVel_prev)/dt;

  float Acc_now = givenAcc +( Kp*errorPos + Ki*errorPos_I + Kd*errorPos_D ) ; 
  
  float g = neu_g * Grav;
  float Thrust_gen = Thrust(Acc_now,currentVel,g);
  
  sendData[3] = Thrust_gen;
  
  if(recData[5] == 1.0){
    Heave(Thrust_gen,0);
  }else if(recData[5] == 0.0){
    Vstall(0);
  }
  
  sendData[4] = PWM[4];
  sendData[5] = PWM[5];
  
//  Serial.print(PWM[4]);Serial.print("\t");Serial.println(PWM[5]);
  
  //Update
  prevDepth = currentDepth;
  prevVel = currentVel;

  
  i2cMasterWrite(Out);
}

//void pulseCounter(){
//  pulseCount++;
//}

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

void Heave(float force,int duration){  
  PWM[4] = force_to_pwm_cw(-force/2);// T5
  PWM[5] = force_to_pwm_ccw(-force/2);// T6
  PWM[4] = constrain(PWM[4],1344,1656);
  PWM[5] = constrain(PWM[5],1344,1656);
  ESC_write();
  delay(duration);
}
void ESC_write(){
  ESC5.writeMicroseconds(PWM[4]);
  ESC6.writeMicroseconds(PWM[5]);
}
void i2cMasterRead(uint8_t address, uint8_t nbytes){
	Wire.requestFrom(address,nbytes);
	while(Wire.available()){
		for(uint8_t j=0;j<recSize;j++){
			for(uint8_t i=0;i<4;i++){
				convert.b[i] = Wire.read();
			}recData[j] = convert.f;
		}
	}
}

void i2cMasterWrite(uint8_t address){
	Wire.beginTransmission(address);
	for(uint8_t i=0;i<sendSize;i++){
		convert.f = sendData[i];
		Wire.write(convert.b,4);
	}Wire.endTransmission(true);
}

void sMoniter(){
	for(uint8_t i=0;i<recSize;i++){
		Serial.print(String(recData[i],3)+"\t");
	}Serial.println();
}
void Vstall(int duration){
  PWM[4] = PWM[5] = 1500;
  ESC_write();
  delay(duration);
}
