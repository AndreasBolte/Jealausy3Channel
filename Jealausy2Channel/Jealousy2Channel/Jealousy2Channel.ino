#include <SPI.h>
#include <Controllino.h>
#include <EEPROM.h>
#include <Time.h>
#include "Jealousy2ChannelConrollinoBoardSettings.h"
#include "Rain.h"
#include "WindMeasure.h"
#include "SolarEnergy.h"
#include "SunWatch.h"
#include "Schedule.h"
#include "RemoteControl.h"
#include "WebControl.h"
#include "UserMethods.h"
#include "UserInterface.h"
#include "SolarEnergy.h"


// board: Controllino Mini
// Date: 15.12.2019
// Andreas Bolte
// info@colmuspro.de


void setup()
{
  // put your setUP code here, to run once:

  // initialize necessary pin as interrupt pin
  pinMode(DI_INTERRUPT_WIND, INPUT);
  attachInterrupt(digitalPinToInterrupt(DI_INTERRUPT_WIND), windCount, RISING);

  // initialize serial communication at 38400 bits per second:
  Serial.begin(38400);

  Controllino_RTC_init();
  useControllinoTime();

  if (false) putValuesToEEPROM();      // true = factory settings: use it when changes in libraries
  else getValuesFromEEPROM();

  sunWatch.sm.init(sunWatch.timezone, sunWatch.latitude, sunWatch.longtitude);
  AV_sRise = sunWatch.getSunRise(S_date);
  AV_sSet = sunWatch.getSunSet(S_date);

  TimeStampDelayAfterPowerReturn = now() + 60;                               // 1 minute boot delay for Node-Red
  PowerReturnDelayActive = 1;
}

void loop()
{

  // send data only when you receive data:
  if (Serial.available() > 0)
  {
    serialAvailable();    // UserInterface.h
  } // End if (Serial.available() > 0)

  if (now() > TimeStampDelayAfterPowerReturn) PowerReturnDelayActive = 0;

  useControllinoTime();
  weekDay = Controllino_GetWeekDay();
  // read all inputs
  remoteControl.DI_channelUp[0] = digitalRead(DI_CHANNEL1UP);
  remoteControl.DI_channelDown[0] = digitalRead(DI_CHANNEL1DOWN);
  remoteControl.DI_channelUp[1] = digitalRead(DI_CHANNEL2UP);
  remoteControl.DI_channelDown[1] = digitalRead(DI_CHANNEL2DOWN);

  // every 6 seconds get wind speed
  if (now() - Timestamp[1] > 6)
  {
    AI_windSpeed = windMeasure.getWindMeasureSpeed();
    Timestamp[1] = now();
  } // End every 60 seconds get wind speed

  // A4+A5 only AnalogInputs
  DI_rain = digitalRead(DI_INTERRUPT_RAIN);    // read DI_INTERRUPT_RAIN;
  DI_solarHeatingExtern = analogRead(AI_SOLAR_HEATING_EXTERN);
  if (DI_solarHeatingExtern > 100 ) DI_solarHeatingExtern = 1;
  else DI_solarHeatingExtern = 0;
  AI_roomTemperature = analogRead(AI_ROOM_TEMPERATURE); // calculated teperature by resistor with ui

  AV_sRise = sunWatch.getSunRise(S_date);
  AV_sSet = sunWatch.getSunSet(S_date);

  for (int i = 0; i < CHANNELS; ++i)
  {
    // class methods for channel 0/1
    RainAlarm[i] = rain.getAlarm(DI_INTERRUPT_RAIN, i, PowerReturnDelayActive);
    if (PowerReturnDelayActive == 0) WindAlarm[i] = windMeasure.getAlarm(i, PowerReturnDelayActive);
    else WindAlarm[i] = 0;
    Alarm[i] = RainAlarm[i] + WindAlarm[i];
    RainCommand[i] = rain.command(i, Alarm);
    WindMeasureCommand[i] = windMeasure.command(i, Alarm);
    SolarEnergyCommand[i] = solarEnergy.command(AI_SOLAR_HEATING_EXTERN, S_date > sunWatch.downTime[i] && S_date < sunWatch.upTime[i], i, Alarm);
    SolarEnergyCommand[i  + CHANNELS * 2]  = solarEnergy.command(AI_SOLAR_HEATING_EXTERN, S_date > sunWatch.downTime[i] && S_date < sunWatch.upTime[i], i, No_Alarm);
    SunWatchCommand[i] = sunWatch.command(S_date, i, Alarm);
    SunWatchCommand[i  + CHANNELS * 2]  = sunWatch.command(S_date, i, No_Alarm);
    DayTime = sunWatch.getDayTime(S_date);
    ScheduleCommand[i] = schedule.command(S_date, i, Alarm);
    ScheduleCommand[i  + CHANNELS * 2]  = schedule.command(S_date, i, No_Alarm);
    RemoteControlCommand[i] = remoteControl.command(i);
    WebControlCommand[i] = webControl.command(i);
    // RainCommand
    if (RainCommand[i] == COMMAND_DO_UP && RainCommand[i  + CHANNELS] != RainCommand[i])
    {
      TimeStampEndDown[i] = 0;
      TimeStampEndUp[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    if (RainCommand[i] == COMMAND_DO_DOWN && RainCommand[i  + CHANNELS] != RainCommand[i])
    {
      TimeStampEndUp[i] = 0;
      TimeStampEndDown[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    RainCommand[i  + CHANNELS] = RainCommand[i];                // old = new

    // WindMeasureCommand
    if (WindMeasureCommand[i] == COMMAND_DO_UP && WindMeasureCommand[i  + CHANNELS] != WindMeasureCommand[i])
    {
      TimeStampEndDown[i] = 0;
      TimeStampEndUp[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    // WindMeasureCommand
    if (WindMeasureCommand[i] == COMMAND_DO_DOWN && WindMeasureCommand[i  + CHANNELS] != WindMeasureCommand[i])
    {
      TimeStampEndUp[i] = 0;
      TimeStampEndDown[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    WindMeasureCommand[i  + CHANNELS] = WindMeasureCommand[i];                // old = new
    // SolarEnergyCommand
    if (SolarEnergyCommand[i] == COMMAND_DO_UP && SolarEnergyCommand[i  + CHANNELS] != SolarEnergyCommand[i])
    {
      TimeStampEndDown[i] = 0;
      TimeStampEndUp[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    if (SolarEnergyCommand[i] == COMMAND_DO_DOWN && SolarEnergyCommand[i  + CHANNELS] != SolarEnergyCommand[i])
    {
      TimeStampEndUp[i] = 0;
      TimeStampEndDown[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    SolarEnergyCommand[i  + CHANNELS] = SolarEnergyCommand[i];                // old = new
    // SunWatchCommand
    if (SunWatchCommand[i] == COMMAND_DO_UP && SunWatchCommand[i  + CHANNELS] != SunWatchCommand[i])
    {
      TimeStampEndDown[i] = 0;
      TimeStampEndUp[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    if (SunWatchCommand[i] == COMMAND_DO_DOWN && SunWatchCommand[i  + CHANNELS] != SunWatchCommand[i])
    {
      TimeStampEndUp[i] = 0;
      TimeStampEndDown[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    SunWatchCommand[i  + CHANNELS] = SunWatchCommand[i];                // old = new

    // ScheduleCommand
    if (ScheduleCommand[i] == COMMAND_DO_UP && ScheduleCommand[i  + CHANNELS] != ScheduleCommand[i])
    {
      TimeStampEndDown[i] = 0;
      TimeStampEndUp[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    if (ScheduleCommand[i] == COMMAND_DO_DOWN && ScheduleCommand[i  + CHANNELS] != ScheduleCommand[i])
    {
      TimeStampEndUp[i] = 0;
      TimeStampEndDown[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    ScheduleCommand[i  + CHANNELS] = ScheduleCommand[i];                // old = new

    // Remote Control
    if (RemoteControlCommand[i] == COMMAND_DO_UP && RemoteControlCommand[i  + CHANNELS] != RemoteControlCommand[i])
    {
      TimeStampEndDown[i] = 0;
      if (Alarm[i] == 0) TimeStampEndUp[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    if (RemoteControlCommand[i] != COMMAND_DO_UP && RemoteControlCommand[i  + CHANNELS] != RemoteControlCommand[i])
    {
      TimeStampEndUp[i] = 0;   // stop timer when Remote command not active
    }
    if (RemoteControlCommand[i] == COMMAND_DO_DOWN && RemoteControlCommand[i  + CHANNELS] != RemoteControlCommand[i])
    {
      TimeStampEndUp[i] = 0;
      if (Alarm[i] == 0) TimeStampEndDown[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    if (RemoteControlCommand[i] != COMMAND_DO_DOWN && RemoteControlCommand[i  + CHANNELS] != RemoteControlCommand[i])
    {
      TimeStampEndDown[i] = 0;   // stop timer when Remote command not active
    }
    RemoteControlCommand[i  + CHANNELS] = RemoteControlCommand[i];                // old = new

    // Web Control 0 and 100 %
    if (WebControlCommand[i] == 0  && WebControlCommand[i  + CHANNELS] != WebControlCommand[i])
    {
      TimeStampEndDown[i] = 0;
      if (Alarm[i] == 0) TimeStampEndUp[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }

    if (WebControlCommand[i] == 100  && WebControlCommand[i  + CHANNELS] != WebControlCommand[i])
    {
      TimeStampEndUp[i] = 0;
      if (Alarm[i] == 0) TimeStampEndDown[i] = webControl.EngineDurationTimeInSecond[i] + now();
    }
    // Web Control 1..99 %
    if (WebControlCommand[i] <  WebControlCommand[i  + CHANNELS] && WebControlCommand[i] != 0 && WebControlCommand[i] != 100  && WebControlCommand[i  + CHANNELS] != WebControlCommand[i])
    {
      TimeStampEndDown[i] = 0;
      AddTimeStampValue[i] = (WebControlCommand[i  + CHANNELS] - WebControlCommand[i]) * webControl.EngineDurationTimeInSecond[i];
      AddTimeStampValue[i] = AddTimeStampValue[i] / 100;
      if (Alarm[i] == 0) TimeStampEndUp[i] = AddTimeStampValue[i] + now();
    }
    if (WebControlCommand[i] > WebControlCommand[i  + CHANNELS] && WebControlCommand[i] != 0 && WebControlCommand[i] != 100  && WebControlCommand[i  + CHANNELS] != WebControlCommand[i])
    {
      TimeStampEndUp[i] = 0;
      AddTimeStampValue[i] = (WebControlCommand[i] - WebControlCommand[i  + CHANNELS]) * webControl.EngineDurationTimeInSecond[i];
      AddTimeStampValue[i] = AddTimeStampValue[i] / 100;
      if (Alarm[i] == 0) TimeStampEndDown[i] = AddTimeStampValue[i] + now();
    }
    WebControlCommand[i  + CHANNELS] = WebControlCommand[i];                // old = new

  } // End for (int i = 0; i < CHANNELS; ++i)

  if (webControl.emergencyStop == 0 && PowerReturnDelayActive == 0 ) engineEnable = 1;
  if (webControl.emergencyStop == 1 || PowerReturnDelayActive == 1 ) engineEnable = 0;
  if (TimeStampEndUp[0] > now())
  {
    if (engineEnable == 1) digitalWrite(DO_CHANNEL1UP, HIGH);
    digitalWrite(DO_CHANNEL1DOWN, LOW);
    DO_channel1up = 1;
    DO_channel1down = 0;
    Command[0] = COMMAND_DO_UP;
    // TimeStampEndUpMemory[0] = 0;
  }
  if (TimeStampEndDown[0] > now())
  {
    digitalWrite(DO_CHANNEL1UP, LOW);
    if (engineEnable == 1) digitalWrite(DO_CHANNEL1DOWN, HIGH);
    DO_channel1up = 0;
    DO_channel1down = 1;
    Command[0] = COMMAND_DO_DOWN;
  }
  if (TimeStampEndUp[0] < now() && TimeStampEndDown[0] < now())
  {
    digitalWrite(DO_CHANNEL1UP, LOW);
    digitalWrite(DO_CHANNEL1DOWN, LOW);
    DO_channel1up = 0;
    DO_channel1down = 0;
  }
  // Channel 2
  if (TimeStampEndUp[1] > now())
  {
    if (engineEnable == 1) digitalWrite(DO_CHANNEL2UP, HIGH);
    digitalWrite(DO_CHANNEL2DOWN, LOW);
    DO_channel2up = 1;
    DO_channel2down = 0;
    Command[1] = COMMAND_DO_UP;
  }
  if (TimeStampEndDown[1] > now())
  {
    digitalWrite(DO_CHANNEL2UP, LOW);
    if (engineEnable == 1) digitalWrite(DO_CHANNEL2DOWN, HIGH);
    DO_channel2up = 0;
    DO_channel2down = 1;
    Command[1] = COMMAND_DO_DOWN;
  }
  if (TimeStampEndUp[1] < now() && TimeStampEndDown[1] < now())
  {
    digitalWrite(DO_CHANNEL2UP, LOW);
    digitalWrite(DO_CHANNEL2DOWN, LOW);
    DO_channel2up = 0;
    DO_channel2down = 0;
  }

  // Controll Alarm-LED’s
  if (RainAlarm[0] == 1 || RainAlarm[1] == 1) digitalWrite(DO_RAIN, HIGH);
  else digitalWrite(DO_RAIN, LOW);
  if (WindAlarm[0] == 1 || WindAlarm[1] == 1) digitalWrite(DO_WIND, HIGH);
  else digitalWrite(DO_WIND, LOW);

  // once a day safe configuration to EEPROM
  if (now() - Timestamp[1] > 86400)
  {
    putValuesToEEPROM();
    Timestamp[1] = now();
  } // End once a day safe configuration to EEPROM
}           // End void loop()
