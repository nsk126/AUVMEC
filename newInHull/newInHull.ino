#include <MS5837.h>
#include <Wire.h>
#include <Servo.h>

#define arrsize(x) sizeof(x)/sizeof(x[0])
#define Out 0x08
#define recSize 8
#define sendSize 5
#define IMU 0x68


/* T1 - D9 - ESC4
 * T2 - D3 - ESC1
 * T3 - D6 - ESC3
 * T4 - D5 - ESC2
 * T5L - D11 - ESC6
 * T6R - D10 - ESC5
 */
 
//Horizontal vThrusters T1 T2 T3 T4
Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;

//Vertical vThrusters T5 T6
Servo ESC5;
Servo ESC6;

//Depth Sensor Object
MS5837 sensor;

//pwm array, esc output
uint16_t pwm[6];

// global variables
uint32_t timer;
float desiredDepth;
float errI,errD,error_prev;
float Kp;
float Ki;
float Kd;

float vThrust;
float hThrust;

float recData[recSize];
float sendData[sendSize];
uint8_t imu_buffer[14];

float accelX,accelY,accelZ;

union floatToBytes {
  byte b[4];
  float f;
}convert;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!sensor.init()) {
   Serial.println("Init failed!");
   Serial.println("Are SDA/SCL connected correctly?");
   Serial.println("Blue Robotics Bar02: White=SDA, Green=SCL");
   Serial.println("\n\n\n");
   delay(1000);
 }
 
  sensor.setModel(MS5837::MS5837_02BA); // For Bar02
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)

	ESC1.attach(3,1100,1900);
	ESC2.attach(5,1100,1900);
	ESC3.attach(6,1100,1900);
	ESC4.attach(9,1100,1900);
	ESC5.attach(11,1100,1900);
	ESC6.attach(10,1100,1900);
	
  for(uint8_t i=0;i<6;i++){
  	pwm[i] = 1500;
  }
	ESC_write();
	delay(10);
	
	timer = micros();
}

void loop() {
	sensor.read();
	float currentDepth = sensor.depth() + 0.6953; // in meters
//	Serial.print(String(currentDepth)+"  ");
  sendData[0] = currentDepth;

  //Read From Tether
  i2cMasterRead(Out,4 * recSize);
//  sMoniter();
  
  
  
  desiredDepth = recData[0];
  Kp = recData[1];
  Ki = recData[2];
  Kd = recData[3];
  hThrust = recData[4];

  bool State = recData[5];
  bool hState = recData[6];
  bool vState = recData[7];
  
	
	double dt = (double)(micros() - timer)/1000000;
	timer = micros();
	
	//Vertical controller
	float error = desiredDepth - currentDepth;
//	Serial.print(String(error)+"  ");

	// PID controller
	// Kp*error + Ki*errI + Kd*errd
	errI += error * dt;
//  Serial.print(String(errI)+"  ");

	errD = (error - error_prev)/dt;
//	Serial.print(String(errD)+"  ");
  
	vThrust = Kp*error + Ki*errI + Kd*errD;
//	Serial.print(String(vThrust)+"  ");
	
	// bool AW1 = (vThrust != constrain(vThrust,-5,5));
	// bool AW2 = (error/abs(error)) ==	(vThrust/abs(vThrust));
	// bool Clamp = AW1 && AW2;
	// if(Clamp == true){
		// Ki = 0; 
	// }

  //Heave
	vThrust = constrain(vThrust,-5,5);	
  sendData[3] = vThrust;
//  Serial.print(String(vThrust)+"  ");
  pwm[4] = force_to_pwm_cw(-vThrust/2);// T5
  pwm[5] = force_to_pwm_ccw(-vThrust/2);// T6

  //Surge
  hThrust = constrain(hThrust,-5,5);
  // Total force = 2 * sqrt(2) * F per thruster
  // 0 is T1 1 is T2 are CCW
  pwm[0] = pwm[1] = force_to_pwm_ccw(hThrust/(2.828));
  // 2 is T3 3 is T4 are CW
  pwm[2] = pwm[3] = force_to_pwm_cw(hThrust/(2.828));

  //Horizontal Stuff
  imuRead(0x3B,imu_buffer,6);
  accelX = (int16_t)((imu_buffer[0] << 8) | imu_buffer[1]);
  accelY = (int16_t)((imu_buffer[2] << 8) | imu_buffer[3]);
  accelZ = (int16_t)((imu_buffer[4] << 8) | imu_buffer[5]);

  accelX = float(accelX) / 16384;
  accelY = float(accelY) / 16384;
  accelZ = float(accelZ) / 16384;

  
  
//	Serial.print(String(accelX)+"  ");
//  Serial.print(String(accelY)+"  ");
//  Serial.print(String(accelZ)+"  ");	

  if(vState == 0){
    vstall();
    errI = 0;
    errD = 0;
  }
  if(hState == 0){
    Hstall();
  }
  if(State == 0){
    stall();
    errI = 0;
    errD = 0;
  }
  
  sendData[1] = errI;
  sendData[2] = errD;
  
  sendData[4] = accelY;

  for(uint8_t i=0;i<sendSize;i++){
    Serial.print(String(sendData[i])+"\t");
  }Serial.println();
  

	//writing to all ESC
  ESC_write();

  //writng to tether
  i2cMasterWrite(Out);
  
	//update
	error_prev = error;
	

}

void stall(){
  for(uint8_t i=0;i<arrsize(pwm);i++){
    pwm[i] = 1500;
  }
}
void vstall(){
  pwm[4] = pwm[5] = 1500;
}
void Hstall(){
  pwm[0] = pwm[1] = pwm[2] = pwm[3] = 1500;
}
void ESC_write(){
	ESC1.writeMicroseconds(pwm[0]);
	ESC2.writeMicroseconds(pwm[1]);
	ESC3.writeMicroseconds(pwm[2]);
	ESC4.writeMicroseconds(pwm[3]);
	ESC5.writeMicroseconds(pwm[4]);
	ESC6.writeMicroseconds(pwm[5]);
}

int force_to_pwm_cw(float force){
  if(force < 0){
    return 1464 + 169.2 * force + 34.8 * pow(force,2) + 4.506 * pow(force,3); 
  }else if(force > 0){
    return 1536 + 134.7 * force - 22.07 * pow(force,2) + 2.183 * pow(force,3);
  }else{
    return 1500;
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

/* Tether */
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



/* IMU */
void imuRead(uint8_t Reg, uint8_t data[], uint8_t nbytes){
  Wire.beginTransmission(IMU);
  Wire.write(Reg);
  Wire.endTransmission(false);

  Wire.requestFrom(IMU,nbytes,true);
  for(uint8_t i=0;i<nbytes;i++){
    if(Wire.available()){
      data[i] = Wire.read();
    }
  } 
}

void imuWrite(uint8_t Reg, uint8_t data[], uint8_t len){
  Wire.beginTransmission(IMU);
  Wire.write(Reg);
  Wire.write(data,len);
  Wire.endTransmission(true);
}

void sMoniter(){
  for(uint8_t i=0;i<recSize;i++){
    Serial.print(String(recData[i],3)+"\t");
  }Serial.println();
}
