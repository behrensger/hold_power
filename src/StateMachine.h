#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Holder.h"

class StateMachine
{
  private:
    const int MODE_START_WAITING_TIME;

  public:
    StateMachine(): MODE_START_WAITING_TIME(3) {};
    virtual ~StateMachine();
    void run(const Holder&) const;

  private:
    StateMachine(const StateMachine&) = delete;//no copy constructor

  protected:
    void TEST_HOLDING_FORCE_run(const Holder&) const; //hold as long as possible, large target
    void TRAIN_HOLDING_FORCE_run(const Holder&) const; //30" break 30" train, repeat as often as possible, small target
    void TRAIN_HOLDING_STEADINESS_run(const Holder&) const;//30" red, 5" yellow, 10" hold, 30 times repeat
    void TRAIN_TIMESERIES_run(const Holder&) const;
    void TRAIN_FIRST_SHOT_run(const Holder&) const;
    void CONFIGURE_SENSITIVITY_run(const Holder&) const;

    void TEST_HOLDING_FORCE_start(const Holder&) const;
    void TRAIN_HOLDING_FORCE_start(const Holder&) const;
    void TRAIN_HOLDING_STEADINESS_start(const Holder&) const;
    void TRAIN_TIMESERIES_start(const Holder&) const;
    void TRAIN_FIRST_SHOT_start(const Holder&) const;
    void CONFIGURE_SENSITIVITY_start(const Holder&) const;

  private:
    void initModus(const Holder&) const;
    bool isStillModeStartWaitingTime(const Holder&) const;
    bool isLaserOn(const Holder&) const;


};

#endif // STATEMACHINE_H
