/*
  UserMethods_h - User Methods
  11.01.2020
  andreas.bolte@colmuspro.de
*/
#ifndef UserMethods_h
#define UserMethods_h

#include "Arduino.h"

int EeAddress = 0;                  //start Address for EEPROM  put and get
unsigned long Timestamp[2] = {0, 0}; // timestamp for waiting times

tmElements_t Tm;                        // controllino time
time_t S_date;

// Commands functions return values
// i = 0: Channel 1 new
// i = 1: Channel 2 new
// i = 2: Channel 1 old
// i = 3: Channel 2 old
// i = 4: Channel 1 no alarm
// i = 5: Channel 2 no alarm

unsigned long TimeStampDelayAfterPowerReturn = 0;                                   // Time stamp that controllino in setup after booting
int PowerReturnDelayActive = 0;                                                     // Booting controllino delay active
int engineEnable = 0;                                                               // No emergencyStop and no PowerReturn
int RainAlarm[CHANNELS * 2] = {0, 0, 1, 1};                                         // Rain Alarm for channel 1/2
int WindAlarm[CHANNELS * 2] = {0, 0, 1, 1};                                         // Wind Alarm for channel 1/2
int RainCommand[CHANNELS * 2] = {0, 0, 1, 1};                                       // Rain command for channel 1/2
int WindMeasureCommand[CHANNELS * 2] = {0, 0, 1, 1};                                // Wind command for channel 1/2
int Alarm[CHANNELS * 2] = {0, 0, 1, 1};                                             // Alarm for channel 1/2
int No_Alarm[CHANNELS * 2] = {0, 0, 0, 0};                                          // No Alarm for channel  1/2
int RemoteControlCommand[CHANNELS * 2] = {1, 1, 0, 0};                              // Channel 1/2 wireless remote access smart home
int WebControlCommand[CHANNELS * 2] = {1, 1, 0, 0};                                 // Channel 1/2 localhost raspberry pi4B
int SolarEnergyCommand[CHANNELS * 3] = {0, 0, 0, 0, 0, 0};                          // Channel 1/2 Solar Energy command
int DayTime = 0;                                                                    // 0 = night, 1 = day
int SunWatchCommand[CHANNELS * 3] = {1, 1, 0, 0, 0, 0};                             // Channel 1/2 sunrise and sunset command
int ScheduleCommand[CHANNELS * 3] = {1, 1, 0, 0, 0, 0};                             // Channel 1/2 schedule command
int WebCommand[CHANNELS * 2] = {1, 1, 0, 0};                                        // Channel 1/2 Web command e.g. a button or ALEXA
int Command[CHANNELS * 2] = {1, 1, 0, 0};                                           // Channel 1/2 calculated command
unsigned long TimeStampEndUp[CHANNELS] = {0, 0};                                    // Channel 1/2 Time stamp end Up
unsigned long AddTimeStampValue[CHANNELS] = {0, 0};                                 // Channel 1/2 Time stamp Add for 0..99 %
unsigned long TimeStampEndDown[CHANNELS] = {0, 0};                                  // Channel 1/2 Time stamp end Down



// process values for digital/analog  inputs
int DO_channel1up = 0;
int DO_channel1down = 0;
int DO_channel2up = 0;
int DO_channel2down = 0;
int DI_rain = 0;
int DI_solarHeatingExtern = 0;
int AI_roomTemperature = 0; // calculated teperature by resistor with ui
float AI_windSpeed = 0;
time_t AV_sRise;
time_t AV_sSet;

// class initialisierung
Jealousy2ChannelConrollinoBoardSettings board;
Rain rain(DI_INTERRUPT_RAIN);
WindMeasure windMeasure(true);
SolarEnergy solarEnergy(true);
SunWatch sunWatch(true);
Schedule schedule(true);
RemoteControl remoteControl(true);
WebControl webControl(true);

void windCount()
{
  // increment the counter
  ++windMeasure.windCounter;
}

void printDate(time_t date)
{
  char buff[20];
  sprintf(buff, "%2d.%02d.%4d %02d:%02d:%02d",
          day(date), month(date), year(date), hour(date), minute(date), second(date));
  Serial.print(buff);
}

String DateToString(time_t date)
{
  char buff[20];
  sprintf(buff, "%2d.%02d.%4d %02d:%02d:%02d",
          day(date), month(date), year(date), hour(date), minute(date), second(date));
  return buff;
}

void useControllinoTime()
{
  Tm.Hour = Controllino_GetHour();             //set the tm structure to 23h31m30s on 13Feb2009
  Tm.Minute = Controllino_GetMinute();
  Tm.Second = Controllino_GetSecond();
  Tm.Day = Controllino_GetDay();
  Tm.Month = Controllino_GetMonth();
  Tm.Year = Controllino_GetYear() + 30;    //tmElements_t.Year is the offset from 1970
  S_date = makeTime(Tm);              // in s_date is a offset in the year!!!!
  Tm.Year = Controllino_GetYear();        // No offset in time_h value !!!!

} // End void useControllinoTime

void putValuesToEEPROM()
{
  EeAddress = 0;
  EEPROM.put(EeAddress, rain);
  EeAddress += sizeof(Rain);
  EEPROM.put(EeAddress, windMeasure);
  EeAddress += sizeof(WindMeasure);
  EEPROM.put(EeAddress, sunWatch);
  EeAddress += sizeof(SunWatch);
  EEPROM.put(EeAddress, schedule);
  EeAddress += sizeof(Schedule);
  EEPROM.put(EeAddress, webControl);
  EeAddress += sizeof(WebControl);
  EEPROM.put(EeAddress, remoteControl);
  EeAddress += sizeof(RemoteControl);  // EEprom.length < EeAddress
} // End void putValuesToEEPROM()

void getValuesFromEEPROM()
{
  EeAddress = 0;
  EEPROM.get(EeAddress, rain);
  EeAddress += sizeof(Rain);
  EEPROM.get(EeAddress, windMeasure);
  EeAddress += sizeof(WindMeasure);
  EEPROM.get(EeAddress, sunWatch);
  EeAddress += sizeof(SunWatch);
  EEPROM.get(EeAddress, schedule);
  EeAddress += sizeof(Schedule);
  EEPROM.get(EeAddress, webControl);
  EeAddress += sizeof(WebControl);
  EEPROM.get(EeAddress, remoteControl);
  EeAddress += sizeof(RemoteControl);
} // End void getValuesFromEEPROM()



#endif
