#include "StateMachine.h"
#include <stdlib.h>
#include "Arduino.h"
//#include <iostream>
//using namespace std;

/*
  State 4 - funktioniert gar nicht
  state 5 - zeigt falsches ergebnis
*/

StateMachine::~StateMachine()
{
  //dtor
}



void StateMachine::run(const Holder& holder) const
{
  switch (holder.getUsageMode())
  {
    case TEST_HOLDING_FORCE:
      switch (holder.getPreviousUsageMode())
      {
        case TEST_HOLDING_FORCE:
          TEST_HOLDING_FORCE_run(holder);
          break;

        default:
          TEST_HOLDING_FORCE_start(holder);
          break;
      }

      break;

    case TRAIN_HOLDING_FORCE:
      switch (holder.getPreviousUsageMode())
      {
        case TRAIN_HOLDING_FORCE:
          TRAIN_HOLDING_FORCE_run(holder);
          break;

        default:
          TRAIN_HOLDING_FORCE_start(holder);
          break;
      }

      break;

    case TRAIN_HOLDING_STEADINESS:
      switch (holder.getPreviousUsageMode())
      {
        case TRAIN_HOLDING_STEADINESS:
          TRAIN_HOLDING_STEADINESS_run(holder);
          break;

        default:
          TRAIN_HOLDING_STEADINESS_start(holder);
          break;
      }

      break;

    case TRAIN_TIMESERIES:
      switch (holder.getPreviousUsageMode())
      {
        case TRAIN_TIMESERIES:
          TRAIN_TIMESERIES_run(holder);
          break;

        default:
          TRAIN_TIMESERIES_start(holder);
          break;
      }

      break;

    case TRAIN_FIRST_SHOT:
      switch (holder.getPreviousUsageMode())
      {
        case TRAIN_FIRST_SHOT:
          TRAIN_FIRST_SHOT_run(holder);
          break;

        default:
          TRAIN_FIRST_SHOT_start(holder);
          break;
      }

      break;

    case CONFIGURE_SENSITIVITY:
      switch (holder.getPreviousUsageMode())
      {
        case CONFIGURE_SENSITIVITY:
          CONFIGURE_SENSITIVITY_run(holder);
          break;

        default:
          CONFIGURE_SENSITIVITY_start(holder);
          break;
      }

      break;

    default: //silently do nothing.
      break;
  }
}

bool StateMachine::isLaserOn(const Holder& h) const
{
  return (h.getLightAmount() >= h.getSensitivity());
}

bool StateMachine::isStillModeStartWaitingTime(const Holder& holder) const
{
  if ((holder.getTimer() - holder.getModeStartTime()) <= MODE_START_WAITING_TIME)
    return true;
  else
    return false;
}



/**
   15" Wartezeit Rot. Anzeige des letzten Ergebnisses
    3" Wartezeit Gelb. Anzeige 0
    x" Wartezeit Gelb. Anzeige 0. Wird beendet sobald Laser gemessen wird.
    x" Grün, Solange Laser leuchtet. Anzeige der erreichten Ergebnisses (Sek)
*/
void StateMachine::TEST_HOLDING_FORCE_run(const Holder& holder) const
{
  if (isStillModeStartWaitingTime(holder))
  {
    return; //do nothing if still waiting time
  }


  const time_t now = holder.getTimer();
  const bool laserDetected = isLaserOn(holder);
  const int waiting_time = 15;
  const int yellow_time = 3;

  //timerB = Starttime phase
  //timerA = count time
  holder.getDisplay().setDisplayMode(Display::SHOW_MIN_SEC);
  if (holder.getLED_Status_Green() == true && holder.getLED_Status_Red() == true && holder.getLED_Status_Yellow() == true)
  {
    //start yellow phase
    holder.setLED_Status_Red(false);
    holder.setLED_Status_Yellow(true);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
    holder.setTimerA(0);
    holder.getDisplay().setValue(0);
  }
  else if (holder.getLED_Status_Yellow() == true && (now - holder.getTimerB()) < yellow_time)
  {
    //do nothing just wait
  }
  else if (holder.getLED_Status_Yellow() == true && (now - holder.getTimerB()) >= yellow_time && laserDetected)
  {
    //yellow + laser + waiting time is over --> start green
    holder.setTimerB(now);
    holder.setTimerA(now);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(true);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Green() == true && laserDetected)
  {
    //store time and go on
    holder.setTimerA(now - holder.getTimerB());
    holder.getDisplay().setValue(holder.getTimerA());

  }
  else if (holder.getLED_Status_Green() == true && !laserDetected)
  {
    holder.setTimerA(now - holder.getTimerB());
    holder.getDisplay().setValue(holder.getTimerA());
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);

  }
  else if (holder.getLED_Status_Red() == true && (now - holder.getTimerB()) < waiting_time)
  {
    //do nothing
  }
  else if (holder.getLED_Status_Red() == true && (now - holder.getTimerB()) >= waiting_time)
  {
    holder.setLED_Status_Red(false);
    holder.setLED_Status_Yellow(true);
    holder.setTimerB(now);
    holder.setTimerA(0);
    holder.getDisplay().setValue(0);
  }


}


/**
   Benutzung:
   27" warten in rot Anzeige = Anzahl der erfolgreichen runden.
    3" warten in gelb, Anzeige = 0
   30" Trainingszeit, Anzeige der verstrichenen Zeit.
   wenn min 25" Laser im Ziel, dann war die runde erfolgreich.
*/
void StateMachine::TRAIN_HOLDING_FORCE_run(const Holder& holder) const
{
  const int timeRedPhase = 27;
  const int timeYellowPhase = 3;
  const int timeGreenPhase = 30;
  const unsigned int minMillisForSuccess = 25 * 1000;

  const time_t now = holder.getTimer();


  if (isStillModeStartWaitingTime(holder))
  {
    return; //do nothing if still waiting time
  }

  //TimerA == counts the round. first round == 0.
  //TimerB == startTime current phase
  //TimerC == counts successfull runs
  //

  if (holder.getLED_Status_Green() && holder.getLED_Status_Red() && holder.getLED_Status_Yellow())
  {
    //start first red phase
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
    holder.getDisplay().setDisplayMode(Display::SHOW_SUCCESS_ROUND);
    holder.getDisplay().setValue(0);
    holder.setTimerB(now);

  }
  else if (holder.getLED_Status_Red() && (now - holder.getTimerB()) < timeRedPhase)
  {
    //stay in red, do nothing

  }
  else if (holder.getLED_Status_Red() && (now - holder.getTimerB()) >= timeRedPhase)
  {
    //start yellow
    holder.setLED_Status_Red(false);
    holder.setLED_Status_Yellow(true);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
    holder.getDisplay().setDisplayMode(Display::SHOW_CLEAR);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Yellow() && (now - holder.getTimerB()) < timeYellowPhase)
  {
    //stay in yellow, do nothing

  }
  else if (holder.getLED_Status_Yellow() && (now - holder.getTimerB()) >= timeYellowPhase)
  {
    //start green
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(true);
    holder.setTimerB(now);
    holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
    holder.getDisplay().setValue(0);
    holder.setCountMillis(0);
    holder.setLastMillis(isLaserOn(holder) ? holder.getCurrentMillis() : 0);

  }
  else if (holder.getLED_Status_Green() && (now - holder.getTimerB()) < timeGreenPhase)
  {
    //stay in green
    if (isLaserOn(holder))
    {
      if (holder.getLastMillis() > 0)
      {
        holder.setCountMillis(holder.getCountMillis() + (holder.getCurrentMillis() - holder.getLastMillis()));
      }

      holder.setLastMillis(holder.getCurrentMillis());

    }
    else
    {
      if (holder.getLastMillis() > 0)
      {
        holder.setCountMillis(holder.getCountMillis() + (holder.getCurrentMillis() - holder.getLastMillis()));
      }

      holder.setLastMillis(0);
    }

    holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
    holder.getDisplay().setValue(holder.getCountMillis());

  }
  else if (holder.getLED_Status_Green() && (now - holder.getTimerB()) >= timeGreenPhase)
  {
    //start red, leave green
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);

    if (holder.getLastMillis() > 0)
    {
      holder.setCountMillis(holder.getCountMillis() + (holder.getCurrentMillis() - holder.getLastMillis()));
    }

    //evaluate the results
    if (holder.getCountMillis() >= minMillisForSuccess)
    {
      holder.setTimerC(holder.getTimerC() + 1);
    }
    holder.setTimerA(holder.getTimerA() + 1);
    if (holder.getTimerA() > 99) {
      //max 99 rounds
      holder.setTimerA(0);
      holder.setTimerC(0);
    }

    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
    holder.getDisplay().setDisplayMode(Display::SHOW_SUCCESS_ROUND);
    holder.getDisplay().setValue((holder.getTimerC() * 100) + holder.getTimerA());
    holder.setTimerB(now);
  }

}


/**
   Benutzung:
   15" warten in rot Anzeige = Anzeige der erfolgreichen Runden.
    3" warten in gelb, Anzeige = 0
   12" Trainingszeit, Anzeige der verstrichenen Zeit.
   wenn min 11.5" Laser im Ziel, dann war die Runde erfolgreich.
*/
void StateMachine::TRAIN_HOLDING_STEADINESS_run(const Holder& holder) const
{
  if (isStillModeStartWaitingTime(holder))
  {
    return; //do nothing if still waiting time
  }



  const int timeYellowPhase = 3;
  const int timeGreenPhase = 10;
  const int timeRedPhase = 30 - timeYellowPhase - timeGreenPhase; //Gesamtzyklus 30" = 2 Runden pro Minute, 50 Rounden (Trainingssession dauert also 25 Minuten)
  const unsigned long minMillisForSuccess = (timeGreenPhase - 1) * 1000;
  time_t now = holder.getTimer();


  if (isStillModeStartWaitingTime(holder))
  {
    return; //do nothing if still waiting time
  }

  //TimerA == counts the round. first round == 0.
  //TimerB == startTime current phase
  //TimerC == counts successfull runs
  //

  if (holder.getLED_Status_Green() && holder.getLED_Status_Red() && holder.getLED_Status_Yellow())
  {
    //start first red phase
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
    holder.getDisplay().setDisplayMode(Display::SHOW_SUCCESS_ROUND);
    holder.getDisplay().setValue(0);
    holder.setTimerB(now);

  }
  else if (holder.getLED_Status_Red() && (now - holder.getTimerB()) < timeRedPhase)
  {
    //stay in red, do nothing

  }
  else if (holder.getLED_Status_Red() && (now - holder.getTimerB()) >= timeRedPhase)
  {
    //start yellow
    holder.setLED_Status_Red(false);
    holder.setLED_Status_Yellow(true);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
    holder.getDisplay().setDisplayMode(Display::SHOW_CLEAR);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Yellow() && (now - holder.getTimerB()) < timeYellowPhase)
  {
    //stay in yellow, do nothing

  }
  else if (holder.getLED_Status_Yellow() && (now - holder.getTimerB()) >= timeYellowPhase)
  {
    //start green
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(true);
    holder.setTimerB(now);
    holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
    holder.getDisplay().setValue(0);
    holder.setCountMillis(0);
    holder.setLastMillis(isLaserOn(holder) ? holder.getCurrentMillis() : 0);

  }
  else if (holder.getLED_Status_Green() && (now - holder.getTimerB()) < timeGreenPhase)
  {
    //stay in green
    if (isLaserOn(holder))
    {
      if (holder.getLastMillis() > 0)
      {
        holder.setCountMillis(holder.getCountMillis() + (holder.getCurrentMillis() - holder.getLastMillis()));
      }

      holder.setLastMillis(holder.getCurrentMillis());

    }
    else
    {
      if (holder.getLastMillis() > 0)
      {
        holder.setCountMillis(holder.getCountMillis() + (holder.getCurrentMillis() - holder.getLastMillis()));
      }

      holder.setLastMillis(0);
    }

    holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
    holder.getDisplay().setValue(holder.getCountMillis());

  }
  else if (holder.getLED_Status_Green() && (now - holder.getTimerB()) >= timeGreenPhase)
  {
    //start red, leave green
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);

    if (holder.getLastMillis() > 0)
    {
      holder.setCountMillis(holder.getCountMillis() + (holder.getCurrentMillis() - holder.getLastMillis()));
    }

    //evaluate the results
    if (holder.getCountMillis() >= minMillisForSuccess)
    {
      holder.setTimerC(holder.getTimerC() + 1);
    }
    holder.setTimerA(holder.getTimerA() + 1);
    if (holder.getTimerA() > 99) {
      //max 99 rounds
      holder.setTimerA(0);
      holder.setTimerC(0);
    }

    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
    holder.getDisplay().setDisplayMode(Display::SHOW_SUCCESS_ROUND);
    holder.getDisplay().setValue((holder.getTimerC() * 100) + holder.getTimerA());
    holder.setTimerB(now);
  }
}


/**
   Benutzung:
   30" warten in rot Anzeige = 0 zur Vorbereitung
    7" warten in gelb, Anzeige = der erfolgreichen Runden.
    3" Schusszeit in grün. Anzeige 0
    Erfolgreich ist eine Runde wenn die letzte Sekunde beleuchtet wurde

*/
void StateMachine::TRAIN_TIMESERIES_run(const Holder& holder) const
{
  const int timeRedPhase = 10;
  const int timeYellowPhase = 7;
  const int timeGreenPhase = 3;
  //const unsigned int minMillisForSuccess = 25 * 1000;
  time_t now = holder.getTimer();


  if (isStillModeStartWaitingTime(holder))
  {
    return; //do nothing if still waiting time
  }

  //TimerA == counts the round. first round == 0.
  //TimerB == startTime current phase
  //TimerC == counts successfull runs
  //


  if (holder.getLED_Status_Green() && holder.getLED_Status_Red() && holder.getLED_Status_Yellow())
  {
    //start first red phase
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
    holder.getDisplay().setDisplayMode(Display::SHOW_CLEAR);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Red() && (now - holder.getTimerB()) < timeRedPhase)
  {
    //stay in red, do nothing
    holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);

  }
  else if (holder.getLED_Status_Red() && (now - holder.getTimerB()) >= timeRedPhase)
  {
    //start yellow
    holder.setLED_Status_Red(false);
    holder.setLED_Status_Yellow(true);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
    holder.getDisplay().setDisplayMode(Display::SHOW_CLEAR);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Yellow() && (now - holder.getTimerB()) < timeYellowPhase)
  {
    //    holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
    //holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Yellow() && (now - holder.getTimerB()) >= timeYellowPhase)
  {
    //start green
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(true);
    holder.setTimerB(now);
    holder.setCountMillis(holder.getCurrentMillis());
    holder.getDisplay().setDisplayMode(Display::SHOW_CLEAR);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Green() && (now - holder.getTimerB()) < timeGreenPhase)
  {
    //remember shoot time but stay in green
    if (isLaserOn(holder) && holder.getDisplay().getValue() == 0)
    {
      holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
      holder.getDisplay().setValue(holder.getCurrentMillis() - holder.getCountMillis());
    }
  }
  else if (holder.getLED_Status_Green() && (now - holder.getTimerB()) >= timeGreenPhase)
  {
    //start red, leave green
    //holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
    //holder.getDisplay().setValue(0);
    holder.setLED_Status_Red(false);
    holder.setLED_Status_Yellow(true);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
  }

}


/**
  Train first shot:
  10" red = break; shows the last shot time
  3"+random yellow = ready display shows 0
  max 30" green till first shot. display shows 0
*/
void StateMachine::TRAIN_FIRST_SHOT_run(const Holder& holder) const
{
  const int timeRedPhase = 10;
  const int timeYellowPhase = 3;
  const int timeGreenPhase = 30;
  //const unsigned int minMillisForSuccess = 25 * 1000;
  time_t now = holder.getTimer();
  const int random = timeYellowPhase + (rand() % timeYellowPhase);


  if (isStillModeStartWaitingTime(holder))
  {
    return; //do nothing if still waiting time
  }

  //TimerA == counts the round. first round == 0.
  //TimerB == startTime current phase
  //TimerC == counts successfull runs
  //


  if (holder.getLED_Status_Green() && holder.getLED_Status_Red() && holder.getLED_Status_Yellow())
  {
    //start first red phase
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
    holder.getDisplay().setDisplayMode(Display::SHOW_CLEAR);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Red() && (now - holder.getTimerB()) < timeRedPhase)
  {
    //stay in red, do nothing
    holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);

  }
  else if (holder.getLED_Status_Red() && (now - holder.getTimerB()) >= timeRedPhase)
  {
    //start yellow
    holder.setLED_Status_Red(false);
    holder.setLED_Status_Yellow(true);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
    holder.getDisplay().setDisplayMode(Display::SHOW_CLEAR);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Yellow() && (now - holder.getTimerB()) < random)
  {
    //stay in yellow, do nothing

  }
  else if (holder.getLED_Status_Yellow() && (now - holder.getTimerB()) >= random)
  {
    //start green
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(true);
    holder.setTimerB(now);
    holder.setCountMillis(holder.getCurrentMillis());
    holder.getDisplay().setDisplayMode(Display::SHOW_CLEAR);
    holder.getDisplay().setValue(0);

  }
  else if (holder.getLED_Status_Green() && (now - holder.getTimerB()) < timeGreenPhase)
  {
    //stay in green
    if (isLaserOn(holder))
    {
      holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
      holder.getDisplay().setValue(holder.getCurrentMillis() - holder.getCountMillis());
      holder.setLED_Status_Red(true);
      holder.setLED_Status_Yellow(false);
      holder.setLED_Status_Green(false);
      holder.setTimerB(now);
    }
  }
  else if (holder.getLED_Status_Green() && (now - holder.getTimerB()) >= timeGreenPhase)
  {
    //start red, leave green
    holder.getDisplay().setDisplayMode(Display::SHOW_SEC_HUN);
    holder.getDisplay().setValue(0);
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
    holder.setTimerB(now);
  }

}

void StateMachine::CONFIGURE_SENSITIVITY_run(const Holder& holder) const
{
  if (isStillModeStartWaitingTime(holder))
  {
    return; //do nothing if still waiting time
  }

  if (holder.getLightAmount() >= holder.getSensitivity())
  {
    //laser detected
    holder.setLED_Status_Red(false);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(true);
  }
  else
  {
    //no laser detected
    holder.setLED_Status_Red(true);
    holder.setLED_Status_Yellow(false);
    holder.setLED_Status_Green(false);
  }
  holder.getDisplay().setDisplayMode(Display::SHOW_VALUE);
  holder.getDisplay().setValue(holder.getSensitivity());
  Serial.print("d");
  Serial.print(holder.getSensitivity());
  Serial.println();

}

void StateMachine::initModus(const Holder& holder) const
{
  holder.setModeStartTime(holder.getTimer());
  holder.setLED_Status_Red(true);
  holder.setLED_Status_Yellow(true);
  holder.setLED_Status_Green(true);

  holder.getDisplay().setDisplayMode(Display::SHOW_USAGEMODE);
  holder.getDisplay().setValue(holder.getUsageMode());

  holder.setTimerA(0);
  holder.setTimerB(0);
  holder.setTimerC(0);
  holder.setTimerD(0);
  holder.setTimerB(holder.getTimer());

  holder.setUsageMode(holder.getUsageMode()); //overwrite prev. mode

}


void StateMachine::TEST_HOLDING_FORCE_start(const Holder& holder) const
{
  initModus(holder);
}
void StateMachine::TRAIN_HOLDING_FORCE_start(const Holder& holder) const
{
  initModus(holder);
}
void StateMachine::TRAIN_HOLDING_STEADINESS_start(const Holder& holder) const
{
  initModus(holder);
}
void StateMachine::TRAIN_TIMESERIES_start(const Holder& holder) const
{
  initModus(holder);
}
void StateMachine::TRAIN_FIRST_SHOT_start(const Holder& holder) const
{
  initModus(holder);
}
void StateMachine::CONFIGURE_SENSITIVITY_start(const Holder& holder) const
{
  initModus(holder);
}
