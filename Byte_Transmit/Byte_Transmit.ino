#include <Wire.h>

int pot1,pot2;

byte Move[4];

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {

  int pot1 = analogRead(0);
  int pot2 = analogRead(1);
  int pwm1,pwm2,pwm3,pwm4,pwm5,pwm6;

  float xforce = mapfloat(pot1,0,1023,-8.2024,8.2024);
  float yforce = mapfloat(pot2,0,1023,-5.8,5.8);

  Serial.print("X = ");Serial.print(xforce);Serial.print("  Y = ");Serial.print(yforce);
  pwm1 = pwm2 = pwm3 = pwm4 = force_to_pwm(xforce/(2*sqrt(2)));
  Serial.print("  1,2,3,4 = ");Serial.print(pwm1);Serial.print("  5 = ");

  pwm5 = force_to_pwm(yforce/2);
  pwm6 = force_to_pwm(-1 * yforce/2);

  Serial.print(pwm5);Serial.print("  6 = ");Serial.print(pwm6);Serial.println();

  Move[0] = pot1 >> 8; //pot1h 
  Move[1] = pot1 & 0xFF;//pot1l 
  Move[2] = pot2 >> 8;//pot2h 
  Move[3] = pot2 & 0xFF;//pot2l 

  Wire.beginTransmission(0x01);
  Wire.write(Move,sizeof(Move));
  Wire.endTransmission();

  delay(50);
}


float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
