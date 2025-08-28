#include <Controllino.h>
#include <SoftwareSerial.h>


// board: Controllino Mini
// Date: 28.08.2025
// Andreas Bolte
// info@colmuspro.de

// Integer constants for digital outputs
const int DO_CHANNEL1UP = CONTROLLINO_D0;    //DO: Digital output CHANNEL 1 UP
const int DO_CHANNEL1DOWN = CONTROLLINO_D1;  //D1: Digital output CHANNEL 1 DOWN
const int DO_CHANNEL2UP = CONTROLLINO_D2;    //D2: Digital output CHANNEL 2 UP
const int DO_CHANNEL2DOWN = CONTROLLINO_D3;  //D3: Digital output CHANNEL 2 DOWN
const int DO_WIND_LED = CONTROLLINO_D4;      //D4: Digital output LED WIND
const int DO_RAIN_LED = CONTROLLINO_D5;      //D5: Digital output LED RAIN

// Integer constants for digital/analog  inputs
const int DI_CHANNEL1UP_RC = CONTROLLINO_A0;    //AO: Digital Input CHANNEL 1 UP from Remote Controll
const int DI_CHANNEL1DOWN_RC = CONTROLLINO_A1;  //A1: Digital Input CHANNEL 1 DOWN from Remote Controll
const int DI_CHANNEL2UP_RC = CONTROLLINO_A2;    //A2: Digital Input CHANNEL 2 UP from Remote Controll
const int DI_CHANNEL2DOWN_RC = CONTROLLINO_A3;  //A3: Digital Input CHANNEL 2 DOWN from Remote Controll

// Integer constants for interupts inputs
const int DI_INTERRUPT_WIND = CONTROLLINO_IN0;  //IN0: Interrupt Input WIND
const int DI_INTERRUPT_RAIN = CONTROLLINO_IN1;  //IN1: Interrupt Input RAIN

// A4+A5 only AnalogInputs
const int AI_SOLAR_HEATING_EXTERN = CONTROLLINO_A4;  //A5: Analog Input Heating Controll
const int AI_ROOM_TEMPERATURE = CONTROLLINO_A5;      //A5: Analog Input temperature

// Integer constants
int DO_Channel1Up = 0;     //DO: Digital output CHANNEL 1 UP
int DO_Channel1Down = 0;   //D1: Digital output CHANNEL 1 DOWN
int DO_Channel2Up = 0;     //D2: Digital output CHANNEL 2 UP
int DO_Channel2Down = 0;   //D3: Digital output CHANNEL 2 DOWN
int DO_Wind_Led = 0;       //D5: Digital output LED WIND
int DO_Rain_Led = 0;       //D5: Digital output LED RAIN
int DI_RC_Channel1Up = 0;  //Remote Controll Channel 1 Up
int DI_RC_Channel1Down = 0;  //Remote Controll Channel 1 Down
int DI_RC_Channel2Up = 0;  //Remote Controll Channel 1 Up
int DI_RC_Channel2Down = 0;  //Remote Controll Channel 1 Down

int DI_Wind = 0;  //Wind
int DI_Rain = 0;  //Rain
int windCounter = 0;

long int i = 0;        // Loop counter ca 120 s
long int i0 = 2000;  // fix i4
long int i1 = 4000;  // fix i4
long int i2 = 6000;  // fix i4
long int i3 = 80000;  // fix i4
long int i4 = 100000;  // fix i4
long int i5 = 200000;  // fix i5
long int i6 = 300000;  // fix i6
long int iMax = 300000;

// values for windSpeed
unsigned long timeNew = 0;
unsigned long timeOld = 0;
float windSpeed = 0;
const float SPEED_PRO_IMPULS = 1.2;  // from the description Vron other sensor 1,2 schwizer 2.4
long int j = 0;                           // Loop counter ca 120 s
long int jMax = 100000;

String inputString = "";
bool stringComplete = false;

void windCount() {
  // increment the counter
  ++windCounter;
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
      break;
    } else {
      inputString += inChar;
    }
  }
}



void setup() {
  // put your setUP code here, to run once:

  // initialize necessary pin as output pin
  pinMode(CONTROLLINO_D0, OUTPUT);
  pinMode(CONTROLLINO_D1, OUTPUT);
  pinMode(CONTROLLINO_D2, OUTPUT);
  pinMode(CONTROLLINO_D3, OUTPUT);
  pinMode(CONTROLLINO_D4, OUTPUT);
  pinMode(CONTROLLINO_D5, OUTPUT);

  // initialize necessary pin as input pin
  pinMode(CONTROLLINO_A0, INPUT);
  pinMode(CONTROLLINO_A1, INPUT);
  pinMode(CONTROLLINO_A2, INPUT);
  pinMode(CONTROLLINO_A3, INPUT);
  // A4+A5 only AnalogInputs
  // initialize necessary pin as interrupt pin
  pinMode(DI_INTERRUPT_WIND, INPUT);
  attachInterrupt(digitalPinToInterrupt(DI_INTERRUPT_WIND), windCount, RISING);
  pinMode(DI_INTERRUPT_RAIN, INPUT);


  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);

}  //End void setup()

void loop() {

if (stringComplete) {
    if (inputString.equals(String("y101:0"))) DO_Channel1Up = 0;
    if (inputString.equals(String("y101:1"))) DO_Channel1Up = 1;
    if (inputString.equals(String("y102:0"))) DO_Channel1Down = 0;
    if (inputString.equals(String("y102:1"))) DO_Channel1Down = 1;
    if (inputString.equals(String("y103:0"))) DO_Channel2Up = 0;
    if (inputString.equals(String("y103:1"))) DO_Channel2Up = 1;
    if (inputString.equals(String("y104:0"))) DO_Channel2Down = 0;
    if (inputString.equals(String("y104:1"))) DO_Channel2Down = 1;
    if (inputString.equals(String("y105:0"))) DO_Wind_Led = 0;
    if (inputString.equals(String("y105:1"))) DO_Wind_Led = 1;
    if (inputString.equals(String("y106:0"))) DO_Rain_Led =0;
    if (inputString.equals(String("y106:1"))) DO_Rain_Led =1;
    inputString = "";
    stringComplete = false;
  }
    
   // read all inputs
  DI_RC_Channel1Up = digitalRead(DI_CHANNEL1UP_RC);
  DI_RC_Channel1Down = digitalRead(DI_CHANNEL1DOWN_RC);
  DI_RC_Channel2Up = digitalRead(DI_CHANNEL2UP_RC);
  DI_RC_Channel2Down = digitalRead(DI_CHANNEL2DOWN_RC);
  DI_Rain = digitalRead(DI_INTERRUPT_RAIN);

  //send by polling time
  i = i + 1;
  if (DO_Channel1Up == 1 && i == i0) Serial.println("x101:1");
  if (DO_Channel1Up == 0 && i == i0) Serial.println("x101:0");
  if (DO_Channel1Down == 1 && i == i1) Serial.println("x102:1");
  if (DO_Channel1Down == 0 && i == i1) Serial.println("x102:0");
  if (DO_Channel2Up == 1 && i == i2) Serial.println("103:1");
  if (DO_Channel2Up == 0 && i == i2) Serial.println("x103:0");
  if (DO_Channel2Down == 1 && i == i3) Serial.println("x104:1");
  if (DO_Channel2Down == 0 && i == i3) Serial.println("x104:0");

  if (DI_RC_Channel1Up == 1 && i == i0) Serial.println("x001:1");
  if (DI_RC_Channel1Up == 0 && i == i0) Serial.println("x001:0");
  if (DI_RC_Channel1Down == 1 && i == i1) Serial.println("x002:1");
  if (DI_RC_Channel1Down == 0 && i == i1) Serial.println("x002:0");
  if (DI_RC_Channel2Up == 1 && i == i2) Serial.println("x003:1");
  if (DI_RC_Channel2Up == 0 && i == i2) Serial.println("x003:0");
  if (DI_RC_Channel2Down == 1 && i == i3) Serial.println("x004:1");
  if (DI_RC_Channel2Down == 0 && i == i3) Serial.println("x004:0");
  if (DI_Rain == 0 && i == i4) Serial.println("x005:0");
  if (DI_Rain == 1 && i == i5) Serial.println("x005:1");
  if (i == i6) {
    timeNew = millis();
    windSpeed = (float)((windCounter * SPEED_PRO_IMPULS * 3.6) / ((timeNew - timeOld)) * 1000);  // convert milis to seconds, unit km/h
    windCounter = 0;
    timeOld = timeNew;
    Serial.print("x006:");
    Serial.println(windSpeed);  // Node-Red Read
 }  //end if (j == jMax)
 if (i >= iMax) i = 0;

  // // write all Engines
  if (DO_Channel1Up == 1 || DI_RC_Channel1Up == 1) digitalWrite(DO_CHANNEL1UP, HIGH);
  else digitalWrite(DO_CHANNEL1UP, LOW);
  if (DO_Channel1Down == 1 ||DI_RC_Channel1Down == 1) digitalWrite(DO_CHANNEL1DOWN, HIGH);
  else digitalWrite(DO_CHANNEL1DOWN, LOW);
  if (DO_Channel2Up == 1 || DI_RC_Channel2Up == 1) digitalWrite(DO_CHANNEL2UP, HIGH);
  else digitalWrite(DO_CHANNEL2UP, LOW);
  if (DO_Channel2Down == 1 || DI_RC_Channel2Down == 1) digitalWrite(DO_CHANNEL2DOWN, HIGH);
  else digitalWrite(DO_CHANNEL2DOWN, LOW);

  // // Controll Alarm-LED’s
  if (DO_Wind_Led == 1) digitalWrite(DO_WIND_LED, HIGH);
  else digitalWrite(DO_WIND_LED, LOW);
  if (DO_Rain_Led == 1) digitalWrite(DO_RAIN_LED, HIGH);
  else digitalWrite(DO_RAIN_LED, LOW);


}  // End void loop()
