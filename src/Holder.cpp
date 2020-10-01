#include "Holder.h"
#include "Helper.h"


//Getter / Setter
void Holder::setSensitivity(const int s)
{
  sensitivity = Helper::constraint(0, s, 100);
} // in Percent

void Holder::setLightAmount(const int s)
{
  lightAmount = Helper::constraint(0, s, 100);
} // in Percent

UsageMode_t Holder::getUsageMode() const
{
  return usageMode;
}

UsageMode_t Holder::getPreviousUsageMode() const
{
  return previousUsageMode;
}

void Holder::setUsageMode(const UsageMode_t m)
{
  previousUsageMode = usageMode;
  usageMode = m;
}

void Holder::setLED_Status_Green(bool b)
{
  LED_Green.setStatus(b);
}

void Holder::setLED_Status_Yellow(bool b)
{
  LED_Yellow.setStatus(b);
}

void Holder::setLED_Status_Red(bool b)
{
  LED_Red.setStatus(b);
}

bool Holder::getLED_Status_Green(void) const
{
  return LED_Green.getStatus();
}

bool Holder::getLED_Status_Yellow(void) const
{
  return LED_Yellow.getStatus();
}

bool Holder::getLED_Status_Red(void) const
{
  return LED_Red.getStatus();
}

void Holder::setTimer(const time_t now)
{
  if (now < 0)
    timer = 0;
  else
    timer = now;
}

void Holder::setModeStartTime(const time_t now)
{
  if (now < 0)
    modeStartTime = 0;
  else
    modeStartTime = now;
}

time_t Holder::getModeStartTime(void) const
{
  return modeStartTime;
}

void Holder::setTimerA(const time_t now)
{
  if (now < 0)
    timerA = 0;
  else
    timerA = now;
}

void Holder::setTimerB(const time_t now)
{
  if (now < 0)
    timerB = 0;
  else
    timerB = now;
}

void Holder::setTimerC(const time_t now)
{
  if (now < 0)
    timerC = 0;
  else
    timerC = now;
}

void Holder::setTimerD(const time_t now)
{
  if (now < 0)
    timerD = 0;
  else
    timerD = now;
}

void Holder::setLastMillis(const unsigned long milli)
{
  lastMillis = milli;
}

void Holder::setCountMillis(const unsigned long milli)
{
  countMillis = milli;
}

void Holder::setCurrentMillis(const unsigned long milli)
{
  currentMillis = milli;
}
