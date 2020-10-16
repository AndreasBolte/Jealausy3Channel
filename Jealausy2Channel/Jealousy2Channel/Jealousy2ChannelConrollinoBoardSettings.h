/*
  Jealousy2ChannelConrollinoBoardSettings.h - Library for Board Settings.
  28.12.2019
  andreas.bolte@colmuspro.de
*/
#ifndef Jealousy2ChannelConrollinoBoardSettings_h
#define Jealousy2ChannelConrollinoBoardSettings_h

#include "Arduino.h"
// https://www.tutorialspoint.com/cplusplus/cpp_constructor_destructor.htm

const int CHANNELS = 2;                    // array size (0,1) for channel 0/1

// Return values from functions for channel 1 and 2 jealousy
const int COMMAND_DO_NOTHING = 0;
const int COMMAND_DO_UP = 1;
const int COMMAND_DO_DOWN = 2;

// Integer constants for digital outputs
const int DO_CHANNEL1UP = CONTROLLINO_D0;               //DO: Digital output CHANNEL 1 UP
const int DO_CHANNEL1DOWN = CONTROLLINO_D1;             //D1: Digital output CHANNEL 1 DOWN
const int DO_CHANNEL2UP = CONTROLLINO_D2;               //D2: Digital output CHANNEL 2 UP
const int DO_CHANNEL2DOWN = CONTROLLINO_D3;             //D3: Digital output CHANNEL 2 DOWN
const int DO_WIND = CONTROLLINO_D4;                     //D4: Digital output WIND
const int DO_RAIN = CONTROLLINO_D5;                     //D5: Digital output RAIN

// Integer constants for digital/analog  inputs
const int DI_CHANNEL1UP = CONTROLLINO_A0;               //AO: Digital Input CHANNEL 1 UP from Remote Controll
const int DI_CHANNEL1DOWN = CONTROLLINO_A1;             //A1: Digital Input CHANNEL 1 DOWN from Remote Controll
const int DI_CHANNEL2UP = CONTROLLINO_A2;               //A2: Digital Input CHANNEL 2 UP from Remote Controll
const int DI_CHANNEL2DOWN = CONTROLLINO_A3;             //A3: Digital Input CHANNEL 2 DOWN from Remote Controll
// A4+A5 only AnalogInputs
const int AI_SOLAR_HEATING_EXTERN = CONTROLLINO_A4;     //A5: Analog Input Heating Controll
const int AI_ROOM_TEMPERATURE = CONTROLLINO_A5;         //A5: Analog Input temperature

// Integer constants for interupts inputs
const int DI_INTERRUPT_WIND = CONTROLLINO_IN0;           //IN0: Interrupt Input WIND
const int DI_INTERRUPT_RAIN = CONTROLLINO_IN1;           //IN1: Interrupt Input RAIN

class Jealousy2ChannelConrollinoBoardSettings
{
  public:

    // constructors
    Jealousy2ChannelConrollinoBoardSettings();  // This is the constructor

  private:

};        // End class Jealousy2ChannelConrollinoBoardSettings

// Member functions definitions including constructor
Jealousy2ChannelConrollinoBoardSettings::Jealousy2ChannelConrollinoBoardSettings(void)
{
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
  pinMode(CONTROLLINO_IN1, INPUT);
}
#endif
