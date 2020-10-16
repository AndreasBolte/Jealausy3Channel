/*
  UserInterface_h - User Methods
  28.01.2020
  andreas.bolte@colmuspro.de
*/
#ifndef UserInterface_h
#define UserInterface_h

#include "Arduino.h"

int weekDay = 0;

void readNodeRed()
{
  Serial.print("x000: "); Serial.print(DateToString(S_date));           // Node-Red Read
  Serial.print(", x001: "); Serial.print(AI_windSpeed);                 // Node-Red Read
  Serial.print(", x004: "); Serial.print(DateToString(AV_sRise));       // Node-Red Read
  Serial.print(", x005: "); Serial.print(DateToString(AV_sSet));        // Node-Red Read
  Serial.print(", x006: "); Serial.print(weekDay);                      // Node-Red Read
  Serial.print(", x007: "); Serial.print(DI_rain);                      // Node-Red Read
  Serial.print(", x008: "); Serial.print(PowerReturnDelayActive);       // Node-Red boot  delay active
  Serial.print(", x010: "); Serial.print(DO_channel1up);                // Channel 1 engine up
  Serial.print(", x011: "); Serial.print(DO_channel1down);              // Channel 1 engine down
  Serial.print(", x012: "); Serial.print(DO_channel2up);                // Channel 2 engine up
  Serial.print(", x013: "); Serial.print(DO_channel2down);              // Channel 2 engine down


  Serial.print(", x022: "); Serial.print(webControl.enableAlexaSpech);  // enable Alexa Spech Node-red
  Serial.print(", x023: "); Serial.print(webControl.enableWebSpech);    // enable Web Spech Node-red
  Serial.print(", x024: "); Serial.print(webControl.enableWebMessages); // enable Web Messages
  Serial.print(", x025: "); Serial.print(webControl.emergencyStop);     // emergency Stop all engines
  Serial.print(", x030: "); Serial.print(sunWatch.latitude, 6);
  Serial.print(", x031: "); Serial.print(sunWatch.longtitude, 6);
  Serial.print(", x032: "); Serial.print(sunWatch.timezone);
  Serial.print(", x035: "); Serial.print(DayTime);                      // 0: night,  1: day
  Serial.print(", x040: "); Serial.print(DI_solarHeatingExtern);
  Serial.print(", x050: "); Serial.print(AI_roomTemperature );  // calculate teperature by resistor with ui

  for (int i = 0; i < CHANNELS; ++i)
  {
    Serial.print(", x"); Serial.print(i * 100 + 103); Serial.print(": "); Serial.print(RainAlarm[i]);                    // Node-Red Read
    Serial.print(", x"); Serial.print(i * 100 + 104); Serial.print(": "); Serial.print(WindAlarm[i]);                    // Node-Red Read
    Serial.print(", x"); Serial.print(i * 100 + 107); Serial.print(": "); Serial.print(ScheduleCommand[i  + CHANNELS * 2]);
    Serial.print(", x"); Serial.print(i * 100 + 108); Serial.print(": "); Serial.print(RemoteControlCommand[i]);
    Serial.print(", x"); Serial.print(i * 100 + 109); Serial.print(": "); Serial.print(Command[i]);
    Serial.print(", x"); Serial.print(i * 100 + 112); Serial.print(": "); Serial.print(rain.enable[i]);
    Serial.print(", x"); Serial.print(i * 100 + 113); Serial.print(": "); Serial.print(remoteControl.enable[i]);
    Serial.print(", x"); Serial.print(i * 100 + 114); Serial.print(": "); Serial.print(schedule.enable[i]);
    Serial.print(", x"); Serial.print(i * 100 + 116); Serial.print(": "); Serial.print(windMeasure.enable[i]);
    Serial.print(", x"); Serial.print(i * 100 + 119); Serial.print(": "); Serial.print(windMeasure.speedHighLimit[i]);
    Serial.print(", x"); Serial.print(i * 100 + 120); Serial.print(": "); Serial.print(DateToString(schedule.downTime[i]));
    Serial.print(", x"); Serial.print(i * 100 + 121); Serial.print(": "); Serial.print(DateToString(schedule.upTime[i]));
    Serial.print(", x"); Serial.print(i * 100 + 130); Serial.print(": "); Serial.print(rain.delayAlarmSecond[i]);
    Serial.print(", x"); Serial.print(i * 100 + 131); Serial.print(": "); Serial.print(rain.currentDelayAlarmSecond[i]);
    Serial.print(", x"); Serial.print(i * 100 + 140); Serial.print(": "); Serial.print(windMeasure.delayAlarmSecond[i]);
    Serial.print(", x"); Serial.print(i * 100 + 141); Serial.print(": "); Serial.print(windMeasure.currentDelayAlarmSecond[i]);
    Serial.print(", x"); Serial.print(i * 100 + 150); Serial.print(": "); Serial.print(sunWatch.enable[i]);
    Serial.print(", x"); Serial.print(i * 100 + 151); Serial.print(": "); Serial.print(sunWatch.shadowAM_PerCent[i]);
    Serial.print(", x"); Serial.print(i * 100 + 152); Serial.print(": "); Serial.print(sunWatch.shadowPM_PerCent[i]);
    Serial.print(", x"); Serial.print(i * 100 + 153); Serial.print(": "); Serial.print(SunWatchCommand[i  + CHANNELS * 2]);
    Serial.print(", x"); Serial.print(i * 100 + 154); Serial.print(": "); Serial.print(DateToString(sunWatch.upTime[i]));
    Serial.print(", x"); Serial.print(i * 100 + 155); Serial.print(": "); Serial.print(DateToString(sunWatch.downTime[i]));
    Serial.print(", x"); Serial.print(i * 100 + 160); Serial.print(": "); Serial.print(webControl.EngineDurationTimeInSecond[i]);
    Serial.print(", x"); Serial.print(i * 100 + 170); Serial.print(": "); Serial.print(solarEnergy.enable[i]);
    Serial.print(", x"); Serial.print(i * 100 + 171); Serial.print(": "); Serial.print(SolarEnergyCommand[i  + CHANNELS * 2]);
    for (int i1 = 0; i1 < 7; ++i1)
    {
      Serial.print(", x"); Serial.print(i * 100 + 400  + i1); Serial.print(": "); Serial.print(schedule.hourDownTime[i][i1]);
      Serial.print(", x"); Serial.print(i * 100 + 410  + i1); Serial.print(": "); Serial.print(schedule.minutesDownTime[i][i1]);
      Serial.print(", x"); Serial.print(i * 100 + 420  + i1); Serial.print(": "); Serial.print(schedule.hourUpTime[i][i1]);
      Serial.print(", x"); Serial.print(i * 100 + 430  + i1); Serial.print(": "); Serial.print(schedule.minutesUpTime[i][i1]);
    } // End for (int i1 = 0; i1 < 7; ++i1)
  } // End for (int i = 0; i < CHANNELS; ++i)
  Serial.println(", x999 = 999");
} // End void readNodeRed()

void serialAvailable()
{
  String sdata = ""; // Initialised to nothing.
  String str = "y003:65.43";
  String strVal = "0";
  int valInt = 0;
  float valFloat = 0;
  unsigned long valUnsignedLong = 0;
  int answerSend = 0;

  // read the incoming String:
  sdata = Serial.readStringUntil(' ');
  sdata.trim();
  str = sdata.substring(0, 4);
  strVal = sdata.substring(5);
  valInt = strVal.toInt();
  for (unsigned int i = 0; i < strVal.length(); i++)
  {
    char c = strVal.charAt(i);
    if (c < '0' || c > '9') break;
    valUnsignedLong *= 10;
    valUnsignedLong += (c - '0');
  }

  valFloat = strVal.toFloat();

  if (str.equals("y001"))
  {
    readNodeRed();
    answerSend = 1;
  }
  if (str.equals(String("y002")))
  {
    // strVal = "03:1:02:20:19:38:15"
    // (Day of the month, Day of the week, Month, Year, Hour, Minute, Second)
    Controllino_SetTimeDate((strVal[0] - 48) * 10 + (strVal[1] - 48),    // daySet
                            strVal[3] - 48,                                                      // weekDaySet
                            (strVal[5] - 48) * 10 + (strVal[6] - 48),                            // monthSet
                            (strVal[8] - 48) * 10 + (strVal[9] - 48),                            // yearSet
                            (strVal[11] - 48) * 10 + (strVal[12] - 48),                          // hourSet
                            (strVal[14] - 48) * 10 + (strVal[15] - 48),                          // minuteSet
                            (strVal[17] - 48) * 10 + (strVal[18] - 48));                         // secondSet);
  }
  if (str.equals(String("y020"))) putValuesToEEPROM();
  if (str.equals(String("y021"))) getValuesFromEEPROM();
  if (str.equals(String("y022"))) webControl.enableAlexaSpech = valInt;
  if (str.equals(String("y023"))) webControl.enableWebSpech = valInt;
  if (str.equals(String("y024"))) webControl.enableWebMessages = valInt;
  if (str.equals(String("y025"))) webControl.emergencyStop = valInt;
  if (str.equals(String("y030"))) sunWatch.latitude = valFloat;
  if (str.equals(String("y031"))) sunWatch.longtitude = valFloat;
  if (str.equals(String("y032"))) sunWatch.timezone = valInt;
  if (str.equals(String("y033"))) sunWatch.sm.init(sunWatch.timezone, sunWatch.latitude, sunWatch.longtitude);

  for (int i = 0; i < CHANNELS; ++i)
  {
    if (str.equals(String("y") + String(i * 100 + 112))) rain.enable[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 113))) remoteControl.enable[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 114))) schedule.enable[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 116))) windMeasure.enable[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 119))) windMeasure.speedHighLimit[i] = valFloat;
    if (str.equals(String("y") + String(i * 100 + 130))) rain.delayAlarmSecond[i] = valUnsignedLong;
    if (str.equals(String("y") + String(i * 100 + 135))) webControl.valueFromALEXA[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 140))) windMeasure.delayAlarmSecond[i] = valUnsignedLong;
    if (str.equals(String("y") + String(i * 100 + 150))) sunWatch.enable[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 151))) sunWatch.shadowAM_PerCent[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 152))) sunWatch.shadowPM_PerCent[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 170))) solarEnergy.enable[i] = valInt;
    if (str.equals(String("y") + String(i * 100 + 160))) webControl.EngineDurationTimeInSecond[i] = valUnsignedLong;
    for (int i1 = 0; i1 < 7; ++i1)
    {
      if (str.equals(String("y") + String(i * 100 + 400 + i1))) schedule.hourDownTime[i][i1] = valInt;
      if (str.equals(String("y") + String(i * 100 + 410 + i1))) schedule.minutesDownTime[i][i1] = valInt;
      if (str.equals(String("y") + String(i * 100 + 420 + i1))) schedule.hourUpTime[i][i1] = valInt;
      if (str.equals(String("y") + String(i * 100 + 430 + i1))) schedule.minutesUpTime[i][i1] = valInt;
    } // End for (int i1 = 0; i1 < 7; ++i1)
  } // End for (int i = 0; i < CHANNELS; ++i)
  if (answerSend != 1)  Serial.println("1");    // readNodeRed(); // after writing read
} // End void serialAvailable()
#endif
