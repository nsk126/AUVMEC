
const byte numChars = 23;
char receivedChars[numChars];   // an array to store the received data
float var[4];
boolean newData = false;


void setup() {
    Serial.begin(115200);
    Serial.println("<Arduino is ready>");
    randomSeed(analogRead(0));
}

void loop() {
    recvWithEndMarker();
    showNewData();
    
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
        //Serial.write(receivedChars);Serial.write("\n");
        char* token = strtok(receivedChars,",");
        var[0] = atof(token);
        token = strtok(NULL,",");
        var[1] = atof(token);
        token = strtok(NULL,",");
        var[2] = atof(token);
        token = strtok(NULL,",");
        var[3] = atof(token);

        float randno = random(201)/100.0;
//        Serial.write("%f",randno);Serial.write(">");
         Serial.println(randno);

        newData = false;
    }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
