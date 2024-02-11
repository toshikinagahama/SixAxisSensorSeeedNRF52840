#ifndef MYSENSOR_H
#define MYSENSOR_H
// インクルード
#include <Arduino.h>
#include <nrf52840.h>
#include "LSM6DS3.h"
#include "Wire.h"
#include "global.h"

class MySensor
{

public:
  // メンバ
  LSM6DS3 *IMU;
  uint16_t acc_x;
  uint16_t acc_y;
  uint16_t acc_z;
  uint16_t gyr_x;
  uint16_t gyr_y;
  uint16_t gyr_z;

  //  関数
  MySensor();
  ~MySensor();
  void initialize();
  void getValue();
};
#endif