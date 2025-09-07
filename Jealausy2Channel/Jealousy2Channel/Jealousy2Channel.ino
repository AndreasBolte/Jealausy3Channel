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

const uint8_t numCoils = 20;
const uint8_t numDiscreteInputs = 20;
const uint8_t numHoldingRegisters = 20;
const uint8_t numInputRegisters = 20;

bool coils[numCoils];
bool discreteInputs[numDiscreteInputs];
// uint16_t Unsigned: 0 to +65,535
uint16_t holdingRegisters[numHoldingRegisters];
uint16_t inputRegisters[numInputRegisters];

// two holding Registers with float
float x[10];
float windCounter = 0.0;
float windSpeed = 0.0;
const float SPEED_PRO_IMPULS = 1.2;  // from the description Vron other sensor 1,2 schwizer 2.4
int period = 10000;
unsigned long time_now = 0;



void windCount() {
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
  pinMode(CONTROLLINO_A0, INPUT_PULLUP);
  pinMode(CONTROLLINO_A1, INPUT_PULLUP);
  pinMode(CONTROLLINO_A2, INPUT_PULLUP);
  pinMode(CONTROLLINO_A3, INPUT_PULLUP);
  // A4+A5 only AnalogInputs
  pinMode(CONTROLLINO_A4, INPUT);
  pinMode(CONTROLLINO_A5, INPUT);

  // initialize necessary pin as interrupt pin
  pinMode(CONTROLLINO_IN0, INPUT_PULLUP);
  pinMode(CONTROLLINO_IN1, INPUT_PULLUP);

  //pinMode(DI_INTERRUPT_WIND, INPUT);
  attachInterrupt(digitalPinToInterrupt(CONTROLLINO_IN0), windCount, RISING);
  //pinMode(DI_INTERRUPT_RAIN, INPUT);

  modbus.configureCoils(coils, numCoils);
  modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs);
  modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);

  MODBUS_SERIAL.begin(MODBUS_BAUD, MODBUS_CONFIG);
  modbus.begin(MODBUS_UNIT_ID, MODBUS_BAUD, MODBUS_CONFIG);
    time_now += period;

}  //End void setup()

void loop() {

if(millis() >= time_now + period){
        windSpeed = (float)((windCounter * SPEED_PRO_IMPULS * 3.6) / (period) * 1000);  // convert milis to seconds, unit km/h
        windCounter = 0;   
        time_now += period;     
    }
  x[0] = windSpeed;
  
  holdingRegisters[0] = f_2uint_int2(x[0]);  // split the float into 2 unsigned integers
  holdingRegisters[1] = f_2uint_int1(x[0]);
  
  // read all digital input
  discreteInputs[0] = digitalRead(CONTROLLINO_A0);
  discreteInputs[1] = digitalRead(CONTROLLINO_A1);
  discreteInputs[2] = digitalRead(CONTROLLINO_A2);
  discreteInputs[3] = digitalRead(CONTROLLINO_A3);
  discreteInputs[4] = LOW;
  discreteInputs[5] = LOW;
  discreteInputs[6] = digitalRead(CONTROLLINO_IN0);
  discreteInputs[7] = digitalRead(CONTROLLINO_IN1);
  // read all analog input
  inputRegisters[4] = map(analogRead(CONTROLLINO_A4), 0, 1023, 0, 255);
  inputRegisters[5] = map(analogRead(CONTROLLINO_A5), 0, 1023, 0, 255);

  modbus.poll();

  // write all Engines
  if (coils[0] || discreteInputs[0]) {  //DO: Digital output CHANNEL 1 UP
    digitalWrite(CONTROLLINO_D0, HIGH);
  }
  if (!coils[0] && !discreteInputs[0]) {  //DO: Digital output CHANNEL 1 UP
    digitalWrite(CONTROLLINO_D0, LOW);
  }
  if (coils[1] || discreteInputs[1]) {  //D1: Digital output CHANNEL 1 DOWN
    digitalWrite(CONTROLLINO_D1, HIGH);
  }
  if (!coils[1] && !discreteInputs[1]) {  //D1: Digital output CHANNEL 1 DOWN
    digitalWrite(CONTROLLINO_D1, LOW);
  }
  if (coils[2] || discreteInputs[2]) {  //D2: Digital output CHANNEL 2 DOWN
    digitalWrite(CONTROLLINO_D2, HIGH);
  }
  if (!coils[2] && !discreteInputs[2]) {  //D2: Digital output CHANNEL 2 DOWN
    digitalWrite(CONTROLLINO_D2, LOW);
  }
  if (coils[3] || discreteInputs[3]) {  //D3: Digital output CHANNEL 2 DOWN
    digitalWrite(CONTROLLINO_D3, HIGH);
  }
  if (!coils[3] && !discreteInputs[3]) {  //D3: Digital output CHANNEL 2 DOWN
    digitalWrite(CONTROLLINO_D3, LOW);
  }

  // write Alarm-LED’s
  if (coils[4]) {  //D4: Digital output LED WIND
    digitalWrite(CONTROLLINO_D4, HIGH);
  }
  if (!coils[4]) {  //D4: Digital output LED WIND
    digitalWrite(CONTROLLINO_D4, LOW);
  }
  if (coils[5]) {   //D5: Digital output LED Rain
    digitalWrite(CONTROLLINO_D5, HIGH);
  }
  if (!coils[5]) { //D5: Digital output LED Rain
    digitalWrite(CONTROLLINO_D5, LOW);
  }
}  // End void loop()
