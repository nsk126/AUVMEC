#include <Wire.h>

int pot1,pot2;

byte Move[2];

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
//
 int pot1 = analogRead(0);
  pot1 = map(pot1,0,1023,0,255);

  int pot2 = analogRead(1);
  pot2 = map(pot2,0,1023,0,255);

  Move[0] = pot1;
  Move[1] = pot2;  

  Serial.print("1 = ");Serial.print(Move[0]);
  Serial.print("  2 = ");Serial.print(Move[1]);Serial.println();

  Wire.beginTransmission(0x01);
  Wire.write(Move,sizeof(Move));
  Wire.endTransmission();
  delay(200);
}
