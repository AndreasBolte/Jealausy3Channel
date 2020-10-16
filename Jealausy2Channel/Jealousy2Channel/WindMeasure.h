/*
  WindMeasure_h - WindMeasure Alarm
  28.12.2019
  andreas.bolte@colmuspro.de
*/
#ifndef WindMeasure_h
#define WindMeasure_h

#include "Arduino.h"



class WindMeasure
{
  public:
    unsigned long timeNew = now();
    unsigned long timeOld = now();

    unsigned long alarmTimestamp[CHANNELS] = {0, 0};
    unsigned long delayTimestamp[CHANNELS] = {0, 0};

    unsigned long delayAlarmSecond[CHANNELS] = {3600, 3600};     // alarm reset delay
    unsigned long currentDelayAlarmSecond[CHANNELS] = {0, 0};    // Current alarm reset delay

    float windSpeed = 0;

    const float SPEED_PRO_IMPULS = 1.2;       // from the description Vron other sensor 1,2 schwizer 2.4

    // Define a global volatile value for the counted impuls from wind sensor
    volatile long windCounter = 0;
    int alarmReturn[CHANNELS] = {0, 0};
    int i = 0;
    int enable[CHANNELS] = {0, 1};

    float speedHighLimit[CHANNELS] = {45.0, 45.0};
    // methods
    int getAlarm(int, int);
    float getWindMeasureSpeed();
    int command(int, int[]);
    WindMeasure(boolean);  // This is the constructor

  private:
  boolean dummy1;
    int commandReturn[CHANNELS] = {0, 0};
};   // End class WindMeasure

// Member functions definitions including constructor
WindMeasure::WindMeasure(boolean dummy)             //constructor
{
dummy1 = dummy;
}  // end constructor

int WindMeasure::getAlarm( int i , int powerReturnDelayActive)
{
  delayTimestamp[i] = now() + delayAlarmSecond[i];
  currentDelayAlarmSecond[i] = 0;
  alarmReturn[i] = 0;
  if (enable[i] == 1 && windSpeed >= speedHighLimit[i])
  {
    alarmReturn[i] = 1;
    alarmTimestamp[i] = delayTimestamp[i];
  }
  if (enable[i] == 1 && windSpeed <= speedHighLimit[i] && delayTimestamp[i] <  alarmTimestamp[i] + delayAlarmSecond[i])
  {
    alarmReturn[i] = 1;
    currentDelayAlarmSecond[i] =  delayAlarmSecond[i] - (delayTimestamp[i] - alarmTimestamp[i]);
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

float WindMeasure::getWindMeasureSpeed()
{
  timeNew = millis();
  windSpeed = (float)((windCounter * SPEED_PRO_IMPULS * 3.6) / ((timeNew - timeOld))*1000); // convert milis to seconds, unit km/h
  windCounter = 0;
  timeOld = timeNew;

  return windSpeed;
}

int WindMeasure::command(int i, int alert[])
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
