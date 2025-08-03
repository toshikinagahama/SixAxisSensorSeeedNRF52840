#include "BatterySensor.h"
#include <nrf52840.h>
#include <nrfx_saadc.h>

void BatterySensor::initialize()
{
  analogReference(AR_VDD);  // VREF = 2.4V
  analogReadResolution(10); // 10bit A/D
  pinMode(this->PIN_WAKEUP, OUTPUT);
  digitalWrite(this->PIN_WAKEUP, LOW);
  // pinMode(this->PIN_READ, INPUT);
}

uint16_t BatterySensor::getValue()
{

  const int max_voltage_mv = 4200; // 4.2V (満充電)
  const int min_voltage_mv = 3000; // 3.0V (放電終止)

  // バッテリー電圧の測定
  int vbat_raw = analogRead(PIN_VBAT);
  // int vbat_raw = 1;
  int vbat_mv = vbat_raw * 3300 / 1023; // VREF = 2.4V, 10bit A/D
  // vbat_mv = vbat_mv * 1510 / 510;       // 1M + 510k / 510k
  uint16_t volt = (uint16_t)(vbat_mv * 1.18);
  int battery_percent = map(vbat_mv, min_voltage_mv, max_voltage_mv, 0, 100);
  Serial.println(volt);
  return volt;
  // return battery_percent;
}