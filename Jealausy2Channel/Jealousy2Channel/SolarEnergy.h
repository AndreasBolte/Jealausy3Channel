/*
  solarEnergy_h - heating and cooling from extern roomtemperature-controller
  31.05.2020
  andreas.bolte@colmuspro.de
*/
#ifndef SolarEnergy_h
#define SolarEnergy_h

class SolarEnergy
{
  public:

    int enable[CHANNELS] = {0, 1};


    // methods
    SolarEnergy(boolean);  // This is the constructor
    int command(int, int, int, int[]);


  private:
  boolean dummy1;
    int commandReturn[CHANNELS] = {0, 0};
    // End  private:
};    // End class SolarEnergy

// Member functions definitions including constructor
SolarEnergy::SolarEnergy(boolean dummy)            //constructor
{
  dummy1 = dummy;
}  // end constructor


int SolarEnergy::command( int pin, int dayTime, int i, int alert[])
{
  // A4+A5 only AnalogInputs
  int heatingExtern = analogRead(pin);
  if (heatingExtern > 100 ) heatingExtern = 1;
  else heatingExtern = 0;

  if (enable[i] == 1)
  {
    if (heatingExtern == 0 && dayTime == 1   && alert[i] == 0)
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
