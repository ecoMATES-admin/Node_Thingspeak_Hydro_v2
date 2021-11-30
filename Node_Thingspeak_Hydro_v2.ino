#define DEBUG 1
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

/* READ ME
   All data is preceeded by a 'V' character
   The sensor data is preceeded by an 'X' character
   The tank capacity data is perceeded by a 'Y' character

*/

int x = 0;
int i = 0;



// Sensor data
char humValBottom[6] = {0};
char tempValBottom[6] = {0};
char humValTop[6] = {0};
char tempValTop[6] = {0};
char humValOutside[6] = {0};
char tempValOutside[6] = {0};
char waterTempVal[6] = {0};
char phVal[6] = {0};
char ecVal[6] = {0};
char waterLevelVal[6] = {0};


//----------- Wi-Fi Details Ketterstr. ---------//
char ssid[] = "Anna+Franz";   // your network SSID (name)
char pass[] = "71277402108590341747";   // your network password
//-------------------------------------------//

/*
  //----------- Wi-Fi Details Rotwandstr. ---------//
  char ssid[] = "FRITZ!Box 7490";   // your network SSID (name)
  char pass[] = "Wilcd2sMeE&T!";   // your network password
  //-------------------------------------------//*/

//----------- Channel Details -------------//
unsigned long Channel_ID1 = 1570128; // Channel ID air sensors
unsigned long Channel_ID2 = 1570130; // Channel ID water sensors
const int Field_number = 1; // Don't change
const char * WriteAPIKey1 = "FNEHJO2Z0OR5JD38"; // for tempHumValBottom + tempHumValTop + tempHumValOutside
const char * WriteAPIKey2 = "Y4JQ8ADI9BOQPS11"; // for waterTempVal, phVal, ecVal + waterLevelval
// ----------------------------------------//

// integrated frequency
unsigned long time_state = 0; // auxiliary variable
int period_state = 100; // period indicating how often SM is called in ms

WiFiClient  client;

// state machine
enum State {INTERNET, WAIT, READ_VALUE1, GET_VALUE1, READ_VALUE2, GET_VALUE2, READ_VALUE3,
            GET_VALUE3, READ_VALUE4, GET_VALUE4, READ_VALUE5, GET_VALUE5, READ_VALUE6, GET_VALUE6,
            READ_VALUE7, GET_VALUE7, READ_VALUE8, GET_VALUE8, READ_VALUE9, GET_VALUE9, READ_VALUE10,
            GET_VALUE10, UPLOAD_ALL, UPLOAD_WATER, SERIAL_FLUSH
           };
State currentState = WAIT;


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Serial.println("STARTING");
  Serial.println("-------------------------------------------");
  internet();

}

void loop() {
  if (millis() - time_state >= period_state) { // frequency of program execution{
    time_state = millis();

    SM_Wifi(); // state machine
  }
}

void SM_Wifi() {
  char c;
  switch (currentState) {
    case INTERNET:
      if (DEBUG)
        Serial.println("Checking Connection...");
      internet();
      currentState = SERIAL_FLUSH;
      break;

    case READ_VALUE1: // hum bottom
      Serial.println("In value state 1");
      if (Serial.find("a")) {
        currentState = GET_VALUE1;
      }
      break;

    case GET_VALUE1:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = READ_VALUE2;
          if (DEBUG) {
            Serial.print("VALUE 1: ");
            Serial.println(humValBottom);
          }
          i = 0;
          break;
        }
        if (Serial.peek() != 10) {
          humValBottom[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE2: // temp bottom
      if (Serial.find("b")) {
        currentState = GET_VALUE2;
      }
      break;

    case GET_VALUE2:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = READ_VALUE3;
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 2: ");
            Serial.println(tempValBottom);
          }
          break;
        }
        if (Serial.peek() != 10) {
          tempValBottom[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE3: // hum top
      if (Serial.find("c")) {
        currentState = GET_VALUE3;
      }
      break;

    case GET_VALUE3:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = READ_VALUE4;
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 3: ");
            Serial.println(humValTop);
          }
          break;
        }
        if (Serial.peek() != 10) {
          humValTop[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE4: // temp top
      if (Serial.find("d")) {
        currentState = GET_VALUE4;
      }
      break;

    case GET_VALUE4:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = READ_VALUE5;
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 4: ");
            Serial.println(tempValTop);
          }
          break;
        }
        if (Serial.peek() != 10) {
          tempValTop[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE5: // hum outside
      if (Serial.find("e")) {
        currentState = GET_VALUE5;
      }
      break;

    case GET_VALUE5:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = READ_VALUE6;
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 5: ");
            Serial.println(humValOutside);
          }
          break;
        }
        if (Serial.peek() != 10) {
          humValOutside[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE6: // temp outside
      if (Serial.find("f")) {
        currentState = GET_VALUE6;
      }
      break;

    case GET_VALUE6:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = READ_VALUE7;
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 6: ");
            Serial.println(tempValOutside);
          }
          break;
        }
        if (Serial.peek() != 10) {
          tempValOutside[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE7: // temp water
      if (Serial.find("g")) {
        currentState = GET_VALUE7;
      }
      break;

    case GET_VALUE7:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = READ_VALUE8;
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 7: ");
            Serial.println(waterTempVal);
          }
          break;
        }
        if (Serial.peek() != 10) {
          waterTempVal[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE8: // phVal
      if (Serial.find("h")) {
        currentState = GET_VALUE8;
      }
      break;

    case GET_VALUE8:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = READ_VALUE9;
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 8: ");
            Serial.println(phVal);
          }
          break;
        }
        if (Serial.peek() != 10) {
          phVal[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE9: // ecVal
      if (Serial.find("i")) {
        currentState = GET_VALUE9;
      }
      break;

    case GET_VALUE9:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = UPLOAD_ALL;
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 9: ");
            Serial.println(ecVal);
          }
          break;
        }
        if (Serial.peek() != 10) {
          ecVal[i] = Serial.read();
          i++;
        }
      }
      break;

    case READ_VALUE10: // water level
      if (Serial.find("w")) {
        currentState = GET_VALUE10;
      }
      break;

    case GET_VALUE10:
      if (Serial.available()) {
        if (Serial.peek() == 13) {
          currentState = UPLOAD_WATER; // specify here to which channel this will be sent
          i = 0;
          if (DEBUG) {
            Serial.print("VALUE 10: ");
            Serial.println(waterLevelVal);
          }
          break;
        }
        if (Serial.peek() != 10) {
          waterLevelVal[i] = Serial.read();
          i++;
        }
      }
      break;

    case SERIAL_FLUSH: //Function to distinguish between sensor data and tank capacity data AND (in a future implementation) to not loose any data if both data sets arrive at the same time
      //Note for future implementation: Thing Speak's 15 second upload limitation must be taken into account in the state machine
      if (DEBUG)
        Serial.println("SERIAL_FLUSH");
      if (Serial.find("V")) {
        if (Serial.peek() == 'X') {
          currentState = READ_VALUE1;
          if (DEBUG)
            Serial.println("all sensor data input found");
          break;
        }
        if (Serial.peek() == 'Y') {
          currentState = READ_VALUE10;
          if (DEBUG)
            Serial.println("water level input found");
          break;
        }
      }
      currentState = WAIT;
      break;

    case UPLOAD_ALL:
      if (DEBUG)
        Serial.println("Uploading all sensor data...");
      upload_all();
      currentState = SERIAL_FLUSH;
      break;

    case UPLOAD_WATER:
      if (DEBUG)
        Serial.println("Uploading water level value...");
      upload_water();
      currentState = SERIAL_FLUSH;
      break;

    case WAIT:
      if (DEBUG)
        Serial.println("WAIT");
      if (Serial.available()) {
        if (DEBUG)
          Serial.println("Serial input available...");
        currentState = INTERNET;
      }
      break;
  }
}

void internet() {
  if (WiFi.status() != WL_CONNECTED) {
    if (DEBUG) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
    }
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      if (DEBUG)
        Serial.print(".");
      delay(5000);
    }
  }
  if (DEBUG)
    Serial.println("Connected");
}


void upload_all() {
  ThingSpeak.setField(1, humValBottom);
  ThingSpeak.setField(2, tempValBottom);
  ThingSpeak.setField(3, humValTop);
  ThingSpeak.setField(4, tempValTop);
  ThingSpeak.setField(5, humValOutside);
  ThingSpeak.setField(6, tempValOutside);

  x = ThingSpeak.writeFields(Channel_ID1, WriteAPIKey1);
  delay(1000);

  if (DEBUG) {
    if (x == 200)
      Serial.println("Data sent. Code: " + String(x));
    if (x != 200) {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
  ThingSpeak.setField(1, waterTempVal);
  ThingSpeak.setField(2, phVal);
  ThingSpeak.setField(3, ecVal);

  x = ThingSpeak.writeFields(Channel_ID2, WriteAPIKey2);

  //delay(15000);

  if (DEBUG) {
    if (x == 200)
      Serial.println("Data sent. Code: " + String(x));
    if (x != 200) {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
  memset(humValBottom, 0, 6);
  memset(tempValBottom, 0, 6);
  memset(humValTop, 0, 6);
  memset(tempValTop, 0, 6);
  memset(humValOutside, 0, 6);
  memset(tempValOutside, 0, 6);
  memset(waterTempVal, 0, 6);
  memset(phVal, 0, 6);
  memset(ecVal, 0, 6);
}

void upload_water() {
  ThingSpeak.setField(4, waterLevelVal);
  x = ThingSpeak.writeFields(Channel_ID2, WriteAPIKey2);

  delay(1000);

  if (DEBUG) {
    if (x == 200)
      Serial.println("Data sent. Code: " + String(x));
    if (x != 200) {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
  memset(waterLevelVal, 0, 6);
}

void clear_array() {

}
