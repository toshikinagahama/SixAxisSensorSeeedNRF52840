#include "BatterySensor.h"
#include <nrf52840.h>
#include <nrfx_saadc.h>
#include <AnalogIn.h>
#include <pinDefinitions.h>

void BatterySensor::initialize()
{
  pinMode(this->PIN_WAKEUP, OUTPUT);
  pinMode(this->PIN_READ, INPUT);
}

class HackAnalogIn : public mbed::AnalogIn
{
  using mbed::AnalogIn::AnalogIn;

public:
  analogin_t getAnalogIn_t();
};

analogin_t HackAnalogIn::getAnalogIn_t()
{
  return this->_adc;
}

void BatterySensor::getValue()
{
  digitalWrite(this->PIN_WAKEUP, LOW);
  float adcCount = 0.0;
  float tmp = 0;
  uint8_t num = 10;
  Serial.println("vBat");
  for (uint8_t i = 0; i < num; i++)
  {
    PinName name = analogPinToPinName(this->PIN_READ);
    if (name == NC)
    {
      return;
    }
    HackAnalogIn *adc = static_cast<HackAnalogIn *>(analogPinToAdcObj(this->PIN_READ));
    if (adc == NULL)
    {
      adc = new HackAnalogIn(name);
      analogPinToAdcObj(this->PIN_READ) = static_cast<mbed::AnalogIn *>(adc);
#ifdef ANALOG_CONFIG
      if (isAdcConfigChanged)
      {
        adc->configure(adcCurrentConfig);
      }
#endif
    }
    nrf_saadc_value_t buffer = 0;
    nrfx_saadc_buffer_convert(&buffer, 1);
    nrfx_err_t ret = nrfx_saadc_sample();
    if (ret == NRFX_ERROR_BUSY)
    {
      // failed to start sampling
      return;
    }
    //   Serial.println(i);
    tmp += (float)buffer;
    delay(10);
  }
  // adcCount = buffer;
  adcCount = tmp / num;
  double adcVoltage = (adcCount * 3.3) / 1024;
  // double adcVoltage = (adcCount * 3.3) / 1024;
  double vBat = adcVoltage * 1510.0 / 510.0; // Voltage divider from Vbat to ADC
  Serial.println(vBat);
  digitalWrite(this->PIN_WAKEUP, HIGH);
}