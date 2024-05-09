#include <Controllino.h>
#include <SoftwareSerial.h>


// board: Controllino Mini
// Date: 04.05.2024
// Andreas Bolte
// info@colmuspro.de

// Integer constants for digital outputs
const int DO_CHANNEL1UP = CONTROLLINO_D0;    //DO: Digital output CHANNEL 1 UP
const int DO_CHANNEL1DOWN = CONTROLLINO_D1;  //D1: Digital output CHANNEL 1 DOWN
const int DO_CHANNEL2UP = CONTROLLINO_D2;    //D2: Digital output CHANNEL 2 UP
const int DO_CHANNEL2DOWN = CONTROLLINO_D3;  //D3: Digital output CHANNEL 2 DOWN
const int DO_WIND_LED = CONTROLLINO_D5;      //D5: Digital output LED WIND
const int DO_RAIN_LED = CONTROLLINO_D6;      //D5: Digital output LED RAIN

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
int DI_RC_Channel1UpOld = 0;
int DI_RC_Channel1UpFlank1 = 0;
int DI_RC_Channel1UpFlank0 = 0;
int DI_RC_Channel1Down = 0;  //Remote Controll Channel 1 Down
int DI_RC_Channel1DownOld = 0;
int DI_RC_Channel1DownFlank1 = 0;
int DI_RC_Channel1DownFlank0 = 0;
int DI_RC_Channel2Up = 0;  //Remote Controll Channel 1 Up
int DI_RC_Channel2UpOld = 0;
int DI_RC_Channel2UpFlank1 = 0;
int DI_RC_Channel2UpFlank0 = 0;
int DI_RC_Channel2Down = 0;  //Remote Controll Channel 1 Down
int DI_RC_Channel2DownOld = 0;
int DI_RC_Channel2DownFlank1 = 0;
int DI_RC_Channel2DownFlank0 = 0;

int DI_Wind = 0;  //Wind
int DI_Rain = 0;  //Rain
int DI_RainOld = 0;
int DI_RainFlank1 = 0;
int DI_RainFlank0 = 0;
int windCounter = 0;

long int i = 0;        // Loop counter ca 120 s
long int i0 = 100000;  // fix i0
long int i1 = 200000;  // fix i1
long int i2 = 300000;  // fix i2
long int i3 = 400000;  // fix i3
long int i4 = 500000;  // fix i4
long int i5 = 600000;  // fix i5
long int i6 = 700000;  // fix i6
long int iMax = 700000;

// values for windSpeed
unsigned long timeNew = 0;
unsigned long timeOld = 0;
float windSpeed = 0;
const float SPEED_PRO_IMPULS = 1.2;  // from the description Vron other sensor 1,2 schwizer 2.4
long int j = 0;                           // Loop counter ca 120 s
long int jMax = 100000;

void windCount() {
  // increment the counter
  ++windCounter;
}



void setup() {
  // put your setUP code here, to run once:

  // initialize necessary pin as output pin
  pinMode(CONTROLLINO_D0, OUTPUT);
  pinMode(CONTROLLINO_D1, OUTPUT);
  pinMode(CONTROLLINO_D2, OUTPUT);
  pinMode(CONTROLLINO_D3, OUTPUT);
  pinMode(CONTROLLINO_D5, OUTPUT);
  pinMode(CONTROLLINO_D6, OUTPUT);

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





  // initialize serial communication at 38400 bits per second:
  Serial.begin(57600);

}  //End void setup()

void loop() {

  // send data only when available:
  if (Serial.available() > 0) {
    //serialAvailable();  // UserInterface.h
    String sdata = "";  // Initialised to nothing.
    String str = "y003:65.43";
    String strVal = "0";
    int valInt = 0;
    float valFloat = 0;
    unsigned long valUnsignedLong = 0;
    int answerSend = 0;

    // read the incoming String:
    sdata = Serial.readStringUntil(' ');
    sdata.trim();
    str = sdata.substring(0, 4);
    strVal = sdata.substring(5);
    valInt = strVal.toInt();
    for (unsigned int i = 0; i < strVal.length(); i++) {
      char c = strVal.charAt(i);
      if (c < '0' || c > '9') break;
      valUnsignedLong *= 10;
      valUnsignedLong += (c - '0');
    }

    valFloat = strVal.toFloat();

    if (str.equals(String("y101"))) DO_Channel1Up = valInt;
    if (str.equals(String("y102"))) DO_Channel1Down = valInt;
    if (str.equals(String("y103"))) DO_Channel2Up = valInt;
    if (str.equals(String("y104"))) DO_Channel2Down = valInt;
    if (str.equals(String("y105"))) DO_Wind_Led = valInt;
    if (str.equals(String("y106"))) DO_Rain_Led = valInt;



    if (answerSend != 1) Serial.println("1");  // readNodeRed(); // after writing read
  }                                            // End if (Serial.available() > 0)

  // read all inputs
  DI_RC_Channel1Up = digitalRead(DI_CHANNEL1UP_RC);
  DI_RC_Channel1Down = digitalRead(DI_CHANNEL1DOWN_RC);
  DI_RC_Channel2Up = digitalRead(DI_CHANNEL2UP_RC);
  DI_RC_Channel2Down = digitalRead(DI_CHANNEL2DOWN_RC);
  DI_Rain = digitalRead(DI_INTERRUPT_RAIN);

  // DI_RC_Channel1Up flanks High and Low x001:1
  if ((DI_RC_Channel1Up == 1) && (DI_RC_Channel1UpOld == 0)) DI_RC_Channel1UpFlank1 = 1;
  else DI_RC_Channel1UpFlank1 = 0;
  if (DI_RC_Channel1UpFlank1 == 1) Serial.println("x001:1");
  if ((DI_RC_Channel1Up == 0) && (DI_RC_Channel1UpOld == 1)) DI_RC_Channel1UpFlank0 = 1;
  else DI_RC_Channel1UpFlank0 = 0;
  DI_RC_Channel1UpOld = DI_RC_Channel1Up;
  if (DI_RC_Channel1UpFlank0 == 1) Serial.println("x001:0");

  // DI_RC_Channel1Down flanks High and Low x002:1
  if ((DI_RC_Channel1Down == 1) && (DI_RC_Channel1DownOld == 0)) DI_RC_Channel1DownFlank1 = 1;
  else DI_RC_Channel1DownFlank1 = 0;
  if (DI_RC_Channel1DownFlank1 == 1) Serial.println("x002:1");
  if ((DI_RC_Channel1Down == 0) && (DI_RC_Channel1DownOld == 1)) DI_RC_Channel1DownFlank0 = 1;
  else DI_RC_Channel1DownFlank0 = 0;
  DI_RC_Channel1DownOld = DI_RC_Channel1Down;
  if (DI_RC_Channel1DownFlank0 == 1) Serial.println("x002:0");

  // DI_RC_Channel2Up flanks High and Low x003:1
  if ((DI_RC_Channel2Up == 1) && (DI_RC_Channel2UpOld == 0)) DI_RC_Channel2UpFlank1 = 1;
  else DI_RC_Channel2UpFlank1 = 0;
  if (DI_RC_Channel2UpFlank1 == 1) Serial.println("x003:1");
  if ((DI_RC_Channel2Up == 0) && (DI_RC_Channel2UpOld == 1)) DI_RC_Channel2UpFlank0 = 1;
  else DI_RC_Channel2UpFlank0 = 0;
  DI_RC_Channel2UpOld = DI_RC_Channel2Up;
  if (DI_RC_Channel2UpFlank0 == 1) Serial.println("x003:0");

  // DI_RC_Channel2Down flanks High and Low x004:1
  if ((DI_RC_Channel2Down == 1) && (DI_RC_Channel2DownOld == 0)) DI_RC_Channel2DownFlank1 = 1;
  else DI_RC_Channel2DownFlank1 = 0;
  if (DI_RC_Channel2DownFlank1 == 1) Serial.println("x004:1");
  if ((DI_RC_Channel2Down == 0) && (DI_RC_Channel2DownOld == 1)) DI_RC_Channel2DownFlank0 = 1;
  else DI_RC_Channel2DownFlank0 = 0;
  DI_RC_Channel2DownOld = DI_RC_Channel2Down;
  if (DI_RC_Channel2DownFlank0 == 1) Serial.println("x004:0");

  // DI_Rain flanks High and Low x005:1
  if ((DI_Rain == 1) && (DI_RainOld == 0)) DI_RainFlank1 = 1;
  else DI_RainFlank1 = 0;
  if (DI_RainFlank1 == 1) Serial.println("x005:1");
  if ((DI_Rain == 0) && (DI_RainOld == 1)) DI_RainFlank0 = 1;
  else DI_RainFlank0 = 0;
  DI_RainOld = DI_Rain;
  if (DI_RainFlank0 == 1) Serial.println("x005:0");



  //send by polling time
  i = i + 1;
  if (DI_RC_Channel1Up == 0 && i == i0) Serial.println("x001:0");
  if (DI_RC_Channel1Up == 1 && i == i0) Serial.println("x001:1");
  if (DI_RC_Channel1Down == 0 && i == i1) Serial.println("x002:0");
  if (DI_RC_Channel1Down == 1 && i == i1) Serial.println("x002:1");
  if (DI_RC_Channel2Up == 0 && i == i2) Serial.println("x003:0");
  if (DI_RC_Channel2Up == 1 && i == i2) Serial.println("x003:1");
  if (DI_RC_Channel2Down == 0 && i == i3) Serial.println("x004:0");
  if (DI_RC_Channel2Down == 1 && i == i3) Serial.println("x004:1");
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
  if (DO_Channel1Up == 1) digitalWrite(DO_CHANNEL1UP, HIGH);
  else digitalWrite(DO_CHANNEL1UP, LOW);
  if (DO_Channel1Down == 1) digitalWrite(DO_CHANNEL1DOWN, HIGH);
  else digitalWrite(DO_CHANNEL1DOWN, LOW);
  if (DO_Channel2Up == 1) digitalWrite(DO_CHANNEL2UP, HIGH);
  else digitalWrite(DO_CHANNEL2UP, LOW);
  if (DO_Channel2Down == 1) digitalWrite(DO_CHANNEL2DOWN, HIGH);
  else digitalWrite(DO_CHANNEL2DOWN, LOW);

  // // Controll Alarm-LED’s
  if (DO_Wind_Led == 1) digitalWrite(DO_WIND_LED, HIGH);
  else digitalWrite(DO_WIND_LED, LOW);
  if (DO_Rain_Led == 1) digitalWrite(DO_RAIN_LED, HIGH);
  else digitalWrite(DO_RAIN_LED, LOW);


}  // End void loop()
