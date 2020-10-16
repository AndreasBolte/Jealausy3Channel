/*
  sunWatch_h - Sunrise and sunset function
  03.01.2020
  andreas.bolte@colmuspro.de
*/
#ifndef SunWatch_h
#define SunWatch_h
#include <DS3232RTC.h>
#include <SPI.h>
#include <Controllino.h>
#include "Arduino.h"
#include <sunMoon.h>

class SunWatch
{
  public:

    // https://www.laengengrad-breitengrad.de/adresse-zu-laengengrad-breitengrad-gps-koordinaten
    float latitude = 49.12345;              
    float longtitude = 8.9876;
    int timezone = 60;                        // localtime with UTC difference in minutes

    sunMoon  sm;

    int enable[CHANNELS] = {0, 0};
    time_t sRise;
    time_t sSet;
    time_t highnoon;
    time_t upTime[CHANNELS];                  // calculated up time
    time_t downTime[CHANNELS];                // calculated down time

    int shadowAM_PerCent[CHANNELS] = {40, 40};
    int shadowPM_PerCent[CHANNELS] = {80, 80};

    // methods
    SunWatch(boolean);  // This is the constructor
    time_t getSunRise(time_t);
    time_t getSunSet(time_t);
    int command(time_t, int, int[]);
    
    int getDayTime(time_t);
    // Return 1: Day curent time between sunrise and sunset
    // Return 0: Night curent time between before sunrise or behind sunset 


  private:
  boolean dummy1;
    int commandReturn[CHANNELS] = {0, 0};
    int dayTimeReturn = 0;
    // End  private:
};    // End class SunWatch

// Member functions definitions including constructor
SunWatch::SunWatch(boolean dummy)            //constructor
{
  dummy1 = dummy;
  sm.init(timezone, latitude, longtitude);
}  // end constructor

time_t SunWatch::getSunRise(time_t s_date)
{
  sRise = sm.sunRise(s_date);

  return sRise;
}

time_t SunWatch::getSunSet(time_t s_date)
{
  sSet = sm.sunSet(s_date);
  return sSet;
}

int SunWatch::getDayTime(time_t s_date)
{
  sRise = sm.sunRise(s_date);
  sSet = sm.sunSet(s_date);
  if (s_date > sRise && s_date < sSet) dayTimeReturn = 1;
  else dayTimeReturn = 0;
  return dayTimeReturn;
}

int SunWatch::command(time_t s_date, int i, int alert[])
{
  highnoon = (sRise + sSet) / 2;
  downTime[i] = sRise + ((highnoon - sRise) / 100) * (100 - shadowAM_PerCent[i]);
  upTime[i] = highnoon + ((sSet - highnoon) / 100) * shadowPM_PerCent[i];

  if (enable[i] == 1)
  {
    if (s_date > downTime[i] && s_date < upTime[i]  && alert[i] == 0 )
    {
      commandReturn[i] = COMMAND_DO_DOWN;
    }
    else
    {
      commandReturn[i] = COMMAND_DO_UP;
    }
  }
  else
  {
    commandReturn[i] = COMMAND_DO_NOTHING;
  }
  return commandReturn[i];

}

#endif
