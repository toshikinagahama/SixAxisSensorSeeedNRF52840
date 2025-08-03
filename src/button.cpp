#include "button.h"
#include <Arduino.h>
#include <nrf52840.h>
#include "global.h"

unsigned long buttonA_press_time = 0;
bool buttonA_pressed = false;
bool buttonA_long_press_detected = false;
bool buttonA_short_press_detected = false;

/**
 * @brief 初期化関数
 *
 */
void button_initialize()
{
  pinMode(PIN_BUTTON_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_A), buttonA_ISR, CHANGE);
}

/**
 * @brief ボタンA割り込み関数
 *
 */
void buttonA_ISR()
{
  if (digitalRead(PIN_BUTTON_A) == LOW)
  {
    // ボタンが押された瞬間 (LOW)
    buttonA_press_time = millis();
    buttonA_pressed = true;
  }
  else
  {
    // ボタンが離された瞬間 (HIGH)
    if (buttonA_pressed)
    {
      unsigned long duration = millis() - buttonA_press_time;
      if (duration >= LONG_PRESS_THRESHOLD_MS)
        // 閾値を超えていたら長押しと判断
        INT.BIT.BUTTON_A_LONG_PRESSED = 1;
      else
        INT.BIT.BUTTON_A_SHORT_PRESSED = 1;
      buttonA_pressed = false; // フラグをリセット
    }
  }
}