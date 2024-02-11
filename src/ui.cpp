#include "ui.h"
#include "global.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::initialize()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
}

void UI::redBlink(uint16_t interval)
{
  this->time_e = millis();
  if (this->time_e - this->time_s > interval)
  {
    if (this->isRedOn)
    {
      digitalWrite(LED_RED, HIGH);
    }
    else
    {
      digitalWrite(LED_RED, LOW);
    }
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    isRedOn = !isRedOn;
    this->time_s = this->time_e;
  }
}

void UI::greenBlink(uint16_t interval)
{
  this->time_e = millis();
  if (this->time_e - this->time_s > interval)
  {
    if (this->isGreenOn)
    {
      digitalWrite(LED_GREEN, HIGH);
    }
    else
    {
      digitalWrite(LED_GREEN, LOW);
    }
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    isGreenOn = !isGreenOn;
    this->time_s = this->time_e;
  }
}

void UI::blueBlink(uint16_t interval)
{
  this->time_e = millis();
  if (this->time_e - this->time_s > interval)
  {
    if (this->isBlueOn)
    {
      digitalWrite(LED_BLUE, HIGH);
    }
    else
    {
      digitalWrite(LED_BLUE, LOW);
    }
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    isBlueOn = !isBlueOn;
    this->time_s = this->time_e;
  }
}

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