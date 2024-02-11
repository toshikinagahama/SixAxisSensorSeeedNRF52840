#ifndef UI_H
#define UI_H
// インクルード
#include <Arduino.h>
#include <nrf52840.h>
#include "global.h"

class UI
{

public:
  // メンバ
  ulong time_s = 0;
  ulong time_e = 0;
  bool isRedOn = false;
  bool isGreenOn = false;
  bool isBlueOn = false;
  //  関数
  UI();
  ~UI();
  void initialize();
  void redBlink(uint16_t);
  void greenBlink(uint16_t);
  void blueBlink(uint16_t);
  void setLEDRGB(bool, bool, bool);
};
#endif