#include "ui.h"
#include "global.h"

UI::UI()
{
}

UI::~UI()
{
}

/**
 * @brief 初期化関数
 *
 */
void UI::initialize()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

/**
 * @brief 赤色点滅関数
 *
 * @param interval_on
 * @param interval_off
 */
void UI::redBlink(uint16_t interval_on, uint16_t interval_off)
{
  this->time_e = millis();
  if (this->isRedOn && this->time_e - this->time_s > interval_on)
  {
    // 点灯している場合
    // 点灯間隔分を超えたらオフ
    digitalWrite(LED_RED, HIGH);
    isRedOn = !isRedOn;
    this->time_s = this->time_e;
  }
  else if (!this->isRedOn && this->time_e - this->time_s > interval_off)
  {
    // 消灯している場合
    // 消灯間隔分を超えたらオン
    digitalWrite(LED_RED, LOW);
    isRedOn = !isRedOn;
    this->time_s = this->time_e;
  }
  else
  {
  }
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

/**
 * @brief 緑色点滅関数
 *
 * @param interval_on
 * @param interval_off
 */
void UI::greenBlink(uint16_t interval_on, uint16_t interval_off)
{
  this->time_e = millis();
  if (this->isGreenOn && this->time_e - this->time_s > interval_on)
  {
    // 点灯している場合
    // 点灯間隔分を超えたらオフ
    digitalWrite(LED_GREEN, HIGH);
    isGreenOn = !isGreenOn;
    this->time_s = this->time_e;
  }
  else if (!this->isGreenOn && this->time_e - this->time_s > interval_off)
  {
    // 消灯している場合
    // 消灯間隔分を超えたらオン
    digitalWrite(LED_GREEN, LOW);
    isGreenOn = !isGreenOn;
    this->time_s = this->time_e;
  }
  else
  {
  }
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

/**
 * @brief 青色点滅関数
 *
 * @param interval_on
 * @param interval_off
 */
void UI::blueBlink(uint16_t interval_on, uint16_t interval_off)
{
  this->time_e = millis();
  if (this->isGreenOn && this->time_e - this->time_s > interval_on)
  {
    // 点灯している場合
    // 点灯間隔分を超えたらオフ
    digitalWrite(LED_GREEN, HIGH);
    isGreenOn = !isGreenOn;
    this->time_s = this->time_e;
  }
  else if (!this->isGreenOn && this->time_e - this->time_s > interval_off)
  {
    // 消灯している場合
    // 消灯間隔分を超えたらオン
    digitalWrite(LED_GREEN, LOW);
    isGreenOn = !isGreenOn;
    this->time_s = this->time_e;
  }
  else
  {
  }
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
}

/**
 * @brief LEDのRGBの色を変える関数
 *
 * @param red
 * @param green
 * @param blue
 */
void UI::setLEDRGB(bool red, bool green, bool blue)
{
  if (!red)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);
  if (!green)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);
  if (!blue)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);
}