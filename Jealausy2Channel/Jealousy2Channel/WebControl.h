/*
  WebControl_h - Web Control function
  04.03.2020
  andreas.bolte@colmuspro.de
*/
#ifndef WebControl_h
#define WebControl_h
#include <Controllino.h>
#include "Arduino.h"

class WebControl
{
  public:
    int enable[CHANNELS] = {1, 1};
    int valueFromALEXA [CHANNELS] = {0, 0};
    int enableAlexaSpech = 1;
    int enableWebSpech = 1;
    int enableWebMessages = 1;
    int emergencyStop = 1;
    unsigned long EngineDurationTimeInSecond[CHANNELS] = {60, 60};           // Channel 1/2 Engine Duration time in milliseconds

    // calculated values

    // methods
    WebControl(boolean);  // This is the constructor
    int command(int);

  private:
    boolean dummy1;
    int commandReturn[CHANNELS] = {0, 0};
    // End  private:
};  // End class Schedule

// Member functions definitions including constructor
WebControl::WebControl(boolean dummy)            //constructor
{
  dummy1 = dummy;
}  // end constructor

int WebControl::command(int i)
{
  if (enable[i] == 1)
  {
    commandReturn[i] = valueFromALEXA[i];
  }
  else commandReturn[i] = COMMAND_DO_NOTHING;
  return commandReturn[i];
}

#endif
