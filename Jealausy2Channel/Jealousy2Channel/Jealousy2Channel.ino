#include <Controllino.h>
#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>
#include "ConvertFloatToUint16_t.h"

// board: Controllino Mini
// Date: 28.08.2025
// Andreas Bolte
// info@colmuspro.de

#define MODBUS_BAUD 57600
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_UNIT_ID 10
#define MODBUS_SERIAL Serial

ModbusRTUSlave modbus(MODBUS_SERIAL);

const uint8_t numCoils = 10;
const uint8_t numDiscreteInputs = 10;
const uint8_t numHoldingRegisters = 10;
// const byte numHoldingRegisters = 10;
const uint8_t numInputRegisters = 10;

bool coils[numCoils];
bool discreteInputs[numDiscreteInputs];
// uint16_t Unsigned: 0 to +65,535
uint16_t holdingRegisters[numHoldingRegisters];
uint16_t inputRegisters[numInputRegisters];

// Test holding Registers with float
float x1 = -6543.234;
float x2 = 22.34;
float x3 = 789543.12;
float x4 = 0.065;




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
const int AI_SOLAR_HEATING_EXTERN = CONTROLLINO_A4;  //A4: Analog Input Heating Controll
const int AI_ROOM_TEMPERATURE = CONTROLLINO_A5;      //A5: Analog Input temperature

bool DO_Channel1Up = 0;    //DO: Digital output CHANNEL 1 UP
bool DO_Channel1Down = 0;  //D1: Digital output CHANNEL 1 DOWN
bool DO_Channel2Up = 0;    //D2: Digital output CHANNEL 2 UP
bool DO_Channel2Down = 0;  //D3: Digital output CHANNEL 2 DOWN
bool DO_Wind_Led = 0;      //D4: Digital output LED WIND
bool DO_Rain_Led = 0;      //D5: Digital output LED RAIN

int DI_RC_Channel1Up = 0;    //Remote Controll Channel 1 Up
int DI_RC_Channel1Down = 0;  //Remote Controll Channel 1 Down
int DI_RC_Channel2Up = 0;    //Remote Controll Channel 1 Up
int DI_RC_Channel2Down = 0;  //Remote Controll Channel 1 Down

int DI_Wind = 0;  //Wind
int DI_Rain = 0;  //Rain
int windCounter = 0;

long int i = 0;        // Loop counter ca 120 s
long int i0 = 2000;    // fix i4
long int i1 = 4000;    // fix i4
long int i2 = 6000;    // fix i4
long int i3 = 80000;   // fix i4
long int i4 = 100000;  // fix i4
long int i5 = 200000;  // fix i5
long int i6 = 300000;  // fix i6
long int iMax = 300000;

// values for windSpeed
unsigned long timeNew = 0;
unsigned long timeOld = 0;
float windSpeed = 0;
const float SPEED_PRO_IMPULS = 1.2;  // from the description Vron other sensor 1,2 schwizer 2.4
long int j = 0;                      // Loop counter ca 120 s
long int jMax = 100000;

String inputString = "";
bool stringComplete = false;

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

  modbus.configureCoils(coils, numCoils);
  modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs);
  modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);

  MODBUS_SERIAL.begin(MODBUS_BAUD, MODBUS_CONFIG);
  modbus.begin(MODBUS_UNIT_ID, MODBUS_BAUD, MODBUS_CONFIG);

}  //End void setup()

void loop() {

  windSpeed = 17.43f;

  holdingRegisters[0] = f_2uint_int2(windSpeed);  // split the float into 2 unsigned integers
  holdingRegisters[1] = f_2uint_int1(windSpeed);
  holdingRegisters[2] = f_2uint_int2(x1);  // Test
  holdingRegisters[3] = f_2uint_int1(x1);
  holdingRegisters[4] = f_2uint_int2(x2);  // Test
  holdingRegisters[5] = f_2uint_int1(x2);
  holdingRegisters[6] = f_2uint_int2(x3);  // Test
  holdingRegisters[7] = f_2uint_int1(x3);
  holdingRegisters[8] = f_2uint_int2(x4);  // Test
  holdingRegisters[9] = f_2uint_int1(x4);


  // read all inputs
  DI_RC_Channel1Up = digitalRead(DI_CHANNEL1UP_RC);
  DI_RC_Channel1Down = digitalRead(DI_CHANNEL1DOWN_RC);
  DI_RC_Channel2Up = digitalRead(DI_CHANNEL2UP_RC);
  DI_RC_Channel2Down = digitalRead(DI_CHANNEL2DOWN_RC);
  DI_Rain = digitalRead(DI_INTERRUPT_RAIN);

  //send by polling time
  i = i + 1;

  DO_Channel1Up = coils[0];    //DO: Digital output CHANNEL 1 UP
  DO_Channel1Down = coils[1];  //D1: Digital output CHANNEL 1 DOWN
  DO_Channel2Up = coils[2];    //D2: Digital output CHANNEL 2 UP
  DO_Channel2Down = coils[3];  //D3: Digital output CHANNEL 2 DOWN
  DO_Wind_Led = coils[4];      //D4: Digital output LED WIND
  DO_Rain_Led = coils[5];      //D5: Digital output LED RAIN


  if (DI_RC_Channel1Up == 1 && i == i0) discreteInputs[0] = HIGH;
  if (DI_RC_Channel1Up == 0 && i == i0) discreteInputs[0] = LOW;
  if (DI_RC_Channel1Down == 1 && i == i1) discreteInputs[1] = HIGH;
  if (DI_RC_Channel1Down == 0 && i == i1) discreteInputs[1] = LOW;
  if (DI_RC_Channel2Up == 1 && i == i2) discreteInputs[2] = HIGH;
  if (DI_RC_Channel2Up == 0 && i == i2) discreteInputs[2] = LOW;
  if (DI_RC_Channel2Down == 1 && i == i3) discreteInputs[3] = HIGH;
  if (DI_RC_Channel2Down == 0 && i == i3) discreteInputs[3] = LOW;
  if (DI_Rain == 0 && i == i4) discreteInputs[4] = LOW;
  if (DI_Rain == 1 && i == i4) discreteInputs[4] = HIGH;
  discreteInputs[5] = LOW;    // TEST
  discreteInputs[6] = HIGH;   // TEST
  discreteInputs[7] = LOW;    // TEST
  discreteInputs[8] = HIGH;   // TEST
  discreteInputs[9] = LOW;    // TEST
  modbus.poll();

  if (i == i6) {
    timeNew = millis();
    windSpeed = (float)((windCounter * SPEED_PRO_IMPULS * 3.6) / ((timeNew - timeOld)) * 1000);  // convert milis to seconds, unit km/h
    windCounter = 0;
    timeOld = timeNew;
  }  //end if (i == i6)
  if (i >= iMax) i = 0;

  // // write all Engines
  if (DO_Channel1Up == 1 || DI_RC_Channel1Up == 1) digitalWrite(DO_CHANNEL1UP, HIGH);
  else digitalWrite(DO_CHANNEL1UP, LOW);
  if (DO_Channel1Down == 1 || DI_RC_Channel1Down == 1) digitalWrite(DO_CHANNEL1DOWN, HIGH);
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
