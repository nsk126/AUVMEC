#include <Wire.h>

const byte numChars = 23;
char receivedChars[numChars];   // an array to store the received data
float var[4];
boolean newData = false;
// PC --> Serial interface --> Feedback control

typedef union {
  byte b[4];
  float v;
} converter;
converter f;

typedef union {
  byte b[4];
  float v;
} sender;
sender s;

void setup() {
    Serial.begin(115200);    
    Wire.begin(8);
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
}

void loop() {
    Serial.println(1);
    recvWithEndMarker();
    showNewData();
}

void receiveEvent(int howMany) {
  static uint8_t i = 0; 
  while (1 < Wire.available()) { // loop through all but the last
    s.b[i] = Wire.read(); // receive byte as a character
    i++;
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '>';
    char rc;
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        char* token = strtok(receivedChars,",");
        var[0] = atof(token);
        token = strtok(NULL,",");
        var[1] = atof(token);
        token = strtok(NULL,",");
        var[2] = atof(token);
        token = strtok(NULL,",");
        var[3] = atof(token);
        newData = false;
    }
}
void requestEvent() {
  f.v = var[0];
  Wire.write(f.b,4);
  f.v = var[1];
  Wire.write(f.b,4);
  f.v = var[2];
  Wire.write(f.b,4);
  f.v = var[3];
  Wire.write(f.b,4);
}
