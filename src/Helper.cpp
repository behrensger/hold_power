#include "helper.h"
#include <math.h>
#include <stdlib.h>
//#include <Arduino.h>

int Helper::constraint(const int min, const int val, const int max)
{
  if (val <= min)
    return min;
  if (val >= max)
    return max;
  return val;
}



//double Helper::round(double d)
//{
//    return floor(d + 0.5);
//}



int Helper::roundUp(int numToRound, int multiple)
{
  if (multiple == 0)
    return numToRound;

  int remainder = abs(numToRound) % multiple;
  if (remainder == 0)
    return numToRound;

  if (numToRound < 0)
    return -(abs(numToRound) - remainder);
  else
    return numToRound + multiple - remainder;
}

/**
	for example: potentiometer can create values 0..1023 but application wants percent 0..100
	int mapped_value = map(value, 0, 1023, 0, 100);
*/
int Helper::maps(const int value, const int fromLow, const int fromHigh, const int toLow, const int toHigh)
{
  long input_range = fromHigh - fromLow;
  long output_range = toHigh - toLow;
  long output = (value - fromLow) * output_range / input_range + toLow;
  return output;
}

int Helper::maps(const int value, const int fromLow, const int fromHigh, const int toLow, const int toHigh, const int steps)
{
  int m = maps(value, fromLow, fromHigh, toLow, toHigh);
  int n = roundUp(m, steps);
  return constraint(toLow, n, toHigh);
}
