/*
  Rain_h - Rain Alarm
  28.12.2019
  andreas.bolte@colmuspro.de
*/
#ifndef Rain_h
#define Rain_h

#include "Arduino.h"

class Rain
{
  public:
    unsigned long alarmTimestamp[CHANNELS] = {0, 0};
    unsigned long delayTimestamp[CHANNELS] = {0, 0};

    unsigned long delayAlarmSecond[CHANNELS] = {3600, 3600};     // alarm reset delay
    unsigned long currentDelayAlarmSecond[CHANNELS] = {0, 0};    // Current alarm reset delay

    int pinRain = 0;
    int enable[CHANNELS] = {0, 1};
    // methods
    int getAlarm(int, int, int);
    int command(int, int[]);

    Rain(int pin);  // This is the constructor

  private:
    int pin;
    int alarmReturn[CHANNELS] = {0, 0};
    int commandReturn[CHANNELS] = {0, 0};
    // End  private:
};        //  End class Rain

// Member functions definitions including constructor
Rain::Rain( int pin)            //constructor
{
  pinRain = pin;
}  // end constructor


int Rain::getAlarm(int pin, int i, int powerReturnDelayActive)
{
  int rainInput = digitalRead(pin);    // read DI_INTERRUPT_RAIN
  delayTimestamp[i] = now() + delayAlarmSecond[i];
  currentDelayAlarmSecond[i] = 0;
  alarmReturn[i] = 0;
  if (enable[i] == 1 && rainInput == HIGH)
  {
    alarmReturn[i] = 1;
    alarmTimestamp[i] = delayTimestamp[i];
  }
  if (enable[i] == 1 && rainInput == LOW && delayTimestamp[i] <  alarmTimestamp[i] + delayAlarmSecond[i])
  {
    alarmReturn[i] = 1;
    currentDelayAlarmSecond[i] = delayAlarmSecond[i] - (delayTimestamp[i] - alarmTimestamp[i]) ;
  }
  if (delayTimestamp[i] >  alarmTimestamp[i] + delayAlarmSecond[i])
  {
    alarmReturn[i] = 0;
    alarmTimestamp[i] = 0;
    currentDelayAlarmSecond[i] = 0;
  }
  if (enable[i] == 0 || powerReturnDelayActive == 1)
  {
    alarmReturn[i] = 0;
    alarmTimestamp[i] = 0;
    currentDelayAlarmSecond[i] = 0;
  }
  return alarmReturn[i];
}

int Rain::command(int i, int alert[])
{
  if ((enable[i] == 1 && alert[i] == 0) || enable[i] == 0 )
  {
    commandReturn[i] = COMMAND_DO_NOTHING;
  }
  else
  {
    commandReturn[i] = COMMAND_DO_UP;
  }
  return commandReturn[i];
}
#endif
