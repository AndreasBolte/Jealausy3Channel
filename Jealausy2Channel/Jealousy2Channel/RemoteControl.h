/*
  remoteControl_h - Remote Control function
  06.01.2020
  andreas.bolte@colmuspro.de
*/
#ifndef RemoteControl_h
#define RemoteControl_h

// #include <DS3232RTC.h>
#include <Controllino.h>
#include "Arduino.h"

class RemoteControl
{
  public:
    int enable[CHANNELS] = {1, 1};
    int DI_channelUp[CHANNELS] = {0, 0};
    int DI_channelDown[CHANNELS] = {0, 0};

    // calculated values

    // methods
    RemoteControl(boolean);  // This is the constructor
    int command(int);

  private:
  boolean dummy1;
   int commandReturn[CHANNELS] = {0, 0};
    // End  private:
};  // End class Schedule

// Member functions definitions including constructor
RemoteControl::RemoteControl(boolean dummy)            //constructor
{
  dummy1 = dummy;
}  // end constructor


int RemoteControl::command(int i)
{
  if (enable[i] == 1)
  {
    // Integer constants for digital/analog  inputs
    const int DI_CHANNEL1UP = CONTROLLINO_A0;               //AO: Digital Input CHANNEL 1 UP from Remote Controll
    const int DI_CHANNEL1DOWN = CONTROLLINO_A1;             //A1: Digital Input CHANNEL 1 DOWN from Remote Controll
    const int DI_CHANNEL2UP = CONTROLLINO_A2;               //A2: Digital Input CHANNEL 2 UP from Remote Controll
    const int DI_CHANNEL2DOWN = CONTROLLINO_A3;             //A3: Digital Input CHANNEL 2 DOWN from Remote Controll

    // read all inputs
    DI_channelUp[0] = digitalRead(DI_CHANNEL1UP);
    DI_channelDown[0] = digitalRead(DI_CHANNEL1DOWN);
    DI_channelUp[1] = digitalRead(DI_CHANNEL2UP);
    DI_channelDown[1] = digitalRead(DI_CHANNEL2DOWN);

    if (DI_channelUp[i] == HIGH )
    {
      commandReturn[i] = COMMAND_DO_UP;
    }
    if (DI_channelDown[i] == HIGH )
    {
      commandReturn[i] = COMMAND_DO_DOWN;
    }
    if (DI_channelUp[i] == LOW  && DI_channelDown[i] == LOW)
    {
      commandReturn[i] = COMMAND_DO_NOTHING;
    }
  }         // End if (enable[i])
  else
  {
    commandReturn[i] = COMMAND_DO_NOTHING;
  }
  return commandReturn[i];
}

#endif
