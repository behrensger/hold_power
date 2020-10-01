#ifndef HOLDER_H
#define HOLDER_H

#include <Time.h>
#include "UsageMode.h"
#include "led.h"
#include "Display.h"

class Holder
{
  public:


  private:
    int sensitivity; //how resposive is the foto cell to light in Percent
    int lightAmount; //how much light has the foto cell gotten in Percent
    UsageMode_t previousUsageMode; //set the the mode in which the box is working
    UsageMode_t usageMode; //set the the mode in which the box is working
    time_t  timer; //default 0
    time_t  modeStartTime; //default 0, time when a mode has been startet
    time_t  timerA; //Modus specific times
    time_t  timerB; //Modus specific times
    time_t  timerC; //Modus specific times
    time_t  timerD; //Modus specific times
    unsigned long lastMillis;
    unsigned long currentMillis;
    unsigned long countMillis;
    LED LED_Red;
    LED LED_Yellow;
    LED LED_Green;
    Display display;

  public:

    //default constructor
    Holder():
      sensitivity(50),
      lightAmount(0),
      previousUsageMode(CONFIGURE_SENSITIVITY),
      usageMode(TEST_HOLDING_FORCE),
      timer(0),
      modeStartTime(0),
      timerA(0),
      timerB(0),
      timerC(0),
      timerD(0),
      lastMillis(0),
      currentMillis(0),
      countMillis(0),
      LED_Red(false),
      LED_Yellow(false),
      LED_Green(false),
      display()
    {
    }


    virtual ~Holder() {};

    //Getter / Setter
    int getSensitivity() const
    {
      return sensitivity;
    };
    void setSensitivity(const int s); // in Percent


    int getLightAmount() const
    {
      return lightAmount;
    };
    void setLightAmount(const int a);// in Percent


    UsageMode_t getUsageMode() const;
    UsageMode_t getPreviousUsageMode() const;
    void setUsageMode(const UsageMode_t m);

    void setLED_Status_Green(const bool b);
    void setLED_Status_Yellow(const bool b);
    void setLED_Status_Red(const bool b);

    bool getLED_Status_Green(void) const;
    bool getLED_Status_Yellow(void) const;
    bool getLED_Status_Red(void) const;

    void setLastMillis(const unsigned long);
    void setCountMillis(const unsigned long);
    void setCurrentMillis(const unsigned long);

    const Display& getDisplay() const {
      return display;
    };

    unsigned long getLastMillis(void) const
    {
      return lastMillis;
    };
    unsigned long getCountMillis(void) const
    {
      return countMillis;
    };
    unsigned long getCurrentMillis(void) const
    {
      return currentMillis;
    };

    void setTimer(const time_t now);
    time_t getTimer(void) const
    {
      return timer;
    };

    void setTimerA(const time_t now);
    time_t getTimerA(void) const
    {
      return timerA;
    };

    void setTimerB(const time_t now);
    time_t getTimerB(void) const
    {
      return timerB;
    };

    void setTimerC(const time_t now);
    time_t getTimerC(void) const
    {
      return timerC;
    };

    void setTimerD(const time_t d);
    time_t getTimerD(void) const
    {
      return timerD;
    };

    void setModeStartTime(const time_t now);
    time_t getModeStartTime(void) const;


  private:
    Holder(const Holder&) = delete; //no copy constructor
};

#endif // HOLDER_H
