#ifndef USAGEMODE_H
#define USAGEMODE_H


typedef enum
{
  TEST_HOLDING_FORCE = 1,
  TRAIN_HOLDING_FORCE = 2,
  TRAIN_HOLDING_STEADINESS = 3,
  TRAIN_TIMESERIES = 4,
  TRAIN_FIRST_SHOT = 5,
  CONFIGURE_SENSITIVITY = 6
} UsageMode_t;

#endif // USAGEMODE_H
