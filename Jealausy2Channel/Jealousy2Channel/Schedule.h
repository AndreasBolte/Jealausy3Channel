/*
  schedule_h - Schedule function
  06.01.2020
  andreas.bolte@colmuspro.de
*/
#ifndef Schedule_h
#define Schedule_h

#include <DS3232RTC.h>
#include <SPI.h>
#include <Controllino.h>
#include "Arduino.h"

class Schedule
{
  public:
    int enable[CHANNELS] = {0, 0};

    int hourDownTime[CHANNELS][8];
    int minutesDownTime[CHANNELS][8];
    int hourUpTime[CHANNELS][8];
    int minutesUpTime[CHANNELS][8];

    // calculated values
    time_t downTime[CHANNELS];                // calculated down time
    time_t upTime[CHANNELS];                  // calculated up time
    int weekDay = 0;

    // methods
    Schedule(boolean);  // This is the constructor
    int command(time_t, int, int[]);

  private:
    tmElements_t tm;                        // controllino time
    int commandReturn[CHANNELS] = {0, 0};
     boolean dummy1;
    // End  private:
};  // End class Schedule

// Member functions definitions including constructor
Schedule::Schedule(boolean dummy)            //constructor
{
  dummy1 = dummy;
  for (int i = 0; i < CHANNELS; ++i)
  {
    //  i1: Sunday = 0 Saturday = 6
    for (int i1 = 0; i1 < 7; ++i1)
    {
      // Schedule for down command
      hourDownTime[i][i1] = 8;
      minutesDownTime[i][i1] = 30;
      // Schedule for up command
      hourUpTime[i][i1] = 17; //   Sunday
      minutesUpTime[i][i1] = 30;
    }          // End for (int i1 = 0; i1 < 7; ++i1)
  }        // End for (int i = 0; i < CHANNELS; ++i)
}  // end constructor


int Schedule::command(time_t s_date, int i, int alert[])
{
  weekDay = Controllino_GetWeekDay();
  // Serial.print("schedule.weekDay "); Serial.println(weekDay);

  tm.Day = Controllino_GetDay();
  tm.Month = Controllino_GetMonth();
  tm.Year = Controllino_GetYear() + 30;                    //tmElements_t.Year is the offset from 1970 TODO Controll Year

  // down time
  tm.Hour = hourDownTime[i][weekDay];
  tm.Minute = minutesDownTime[i][weekDay];
  tm.Second = 0;
  downTime[i] = makeTime(tm);               // in s_date is a offset in the year!!!!

  // up time
  tm.Hour = hourUpTime[i][weekDay];
  tm.Minute = minutesUpTime[i][weekDay];
  upTime[i] = makeTime(tm);                   // in s_date is a offset in the year!!!!

  if ((enable[i] == 1) && upTime[i] > downTime[i])
  {
    if (s_date > downTime[i] && s_date < upTime[i] && alert[i] == 0)
    {
      commandReturn[i] = COMMAND_DO_DOWN;
    }
    else
    {
      commandReturn[i] = COMMAND_DO_UP;
    }
  }
    if ((enable[i] == 1) && upTime[i] <= downTime[i])
  {
     if (s_date < upTime[i] && s_date > downTime[i] && alert[i] == 0)
    {
      commandReturn[i] = COMMAND_DO_DOWN;
    }
    else
    {
      commandReturn[i] = COMMAND_DO_UP;
    }
  }
  if (enable[i] == 0)
  {
    commandReturn[i] = COMMAND_DO_NOTHING;
  }
  return commandReturn[i];
}

#endif
