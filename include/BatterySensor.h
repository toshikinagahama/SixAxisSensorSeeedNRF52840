#ifndef BATTERYSENSOR_H
#define BATTERYSENSOR_H
#include <Arduino.h>

class BatterySensor
{

private:
  const uint8_t PIN_WAKEUP = P0_14;
  const uint8_t PIN_READ = PIN_VBAT;

public:
  float raw_vol = 0.0;
  uint8_t level = 0;
  void initialize();
  void getValue();
};

#endif // BATTERYSENSOR_H