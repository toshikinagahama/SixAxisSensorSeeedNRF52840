#include <Arduino.h>
#include <nrf52840.h>
#include "global.h"
#include "MyBLE.h"
#include "MySensor.h"
#include "BatterySensor.h"
#include "led.h"
#include "button.h"
#include "event.h"

ulong time_s0 = 0;
ulong time_s = 0;
ulong time_e = 0;
ulong time_battery_s = 0; // バッテリー送付タイミング用（1sに1回送付）
ulong cnt = 0;

MyState state;
MyBLE *ble = new MyBLE();
MySensor *sensor = new MySensor();
BatterySensor *batterySensor = new BatterySensor();
LED *led = new LED();
BLEDevice central;

/**
 * @brief センサの値のサンプリング
 *
 */
void sampling()
{
  // サンプリング
  sensor->getValue();
  // ulong time_e_last = 0;
  time_e = micros();
  while (time_e - time_s0 < 10000 * cnt)
  {
    time_e = micros();
    // Serial.println(time_e - time_e_last);
    // time_e_last = time_e;
  }
  ulong tmp_time_s = time_s;
  time_s = time_e;
  // Serial.println(time_e - tmp_time_s);
  cnt++;
}

/**
 * @brief センサの値をNotify
 *
 */
void notify()
{
  uint8_t val[22];
  val[0] = 0x01; // 識別子1
  val[1] = 0x01; // 識別子2
  val[2] = uint8_t(cnt >> 0);
  val[3] = uint8_t(cnt >> 8);
  val[4] = uint8_t(cnt >> 16);
  val[5] = uint8_t(cnt >> 24);
  val[6] = uint8_t(uint8_t(sensor->IMU->settings.accelRange >> 0));
  val[7] = uint8_t(uint8_t(sensor->IMU->settings.accelRange >> 8));
  val[8] = uint8_t(uint8_t(sensor->IMU->settings.gyroRange >> 0));
  val[9] = uint8_t(uint8_t(sensor->IMU->settings.gyroRange >> 8));
  val[10] = uint8_t(uint8_t(sensor->acc_x >> 0));
  val[11] = uint8_t(uint8_t(sensor->acc_x >> 8));
  val[12] = uint8_t(uint8_t(sensor->acc_y >> 0));
  val[13] = uint8_t(uint8_t(sensor->acc_y >> 8));
  val[14] = uint8_t(uint8_t(sensor->acc_z >> 0));
  val[15] = uint8_t(uint8_t(sensor->acc_z >> 8));
  val[16] = uint8_t(uint8_t(sensor->gyr_x >> 0));
  val[17] = uint8_t(uint8_t(sensor->gyr_x >> 8));
  val[18] = uint8_t(uint8_t(sensor->gyr_y >> 0));
  val[19] = uint8_t(uint8_t(sensor->gyr_y >> 8));
  val[20] = uint8_t(uint8_t(sensor->gyr_z >> 0));
  val[21] = uint8_t(uint8_t(sensor->gyr_z >> 8));
  ble->SENSOR_TX_Chara->writeValue(val, 22);
}

/**
 * @brief アクション実行
 *
 * @param EVT
 */
void doAction(MyEvent EVT)
{
  switch (state)
  {
  case STATE_ADVERTISE:
    ble->poll();
    led->greenBlink(200, 1000);
    switch (EVT)
    {
    case EVT_BLE_CONNECTED:
      state = STATE_WAIT;
      break;
    case EVT_BUTTON_A_SHORT_PRESSED:
      Serial.println("SHORT PRESSED");
      batterySensor->getValue();
      led->setLEDRGB(false, false, true);
      delay(1000);
      break;
    case EVT_BUTTON_A_LONG_PRESSED:
      Serial.println("Going to System OFF");
      led->setLEDRGB(false, false, false);
      delay(1000); // delay seems important to apply settings, before going to System OFF
      // Ensure interrupt pin from IMU is set to wake up device
      nrf_gpio_cfg_sense_input(digitalPinToInterrupt(P1_13), NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
      delay(2000); // Trigger System OFF
      NRF_POWER->SYSTEMOFF = 1;
      break;
    default:
      break;
    }
    break;
  case STATE_WAIT:
    led->setLEDRGB(false, true, false);
    central = BLE.central();
    if (millis() - time_battery_s >= 1000)
    {
      // 1秒に1回
      ble->Battery_chara->writeValue((uint16_t)(batterySensor->getValue()));
      time_battery_s = millis();
    }
    switch (EVT)
    {
    case EVT_BLE_DISCONNECTED:
      state = STATE_ADVERTISE;
      break;
    case EVT_MEAS_START:
      time_s0 = micros();
      cnt = 0;
      state = STATE_MEAS;
      break;
    case EVT_GET_DEVICE_INFO:
      break;
    case EVT_BUTTON_A_SHORT_PRESSED:
      Serial.println("SHORT PRESSED");
      // バッテリーの状態で点滅の色？回数を変えたい。
      led->setLEDRGB(false, false, true);
      delay(1000);
      break;
    case EVT_BUTTON_A_LONG_PRESSED:
      Serial.println("Going to System OFF");
      led->setLEDRGB(false, false, false);
      delay(1000); // delay seems important to apply settings, before going to System OFF
      // Ensure interrupt pin from IMU is set to wake up device
      nrf_gpio_cfg_sense_input(digitalPinToInterrupt(P1_13), NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
      delay(2000); // Trigger System OFF
      NRF_POWER->SYSTEMOFF = 1;
      break;
    default:
      break;
    }
    break;
  case STATE_MEAS:
    led->setLEDRGB(true, false, false);
    if (central && central.connected())
    {
      sampling();
      notify();
    }
    else
    {
      // 接続が切れた場合
    }
    switch (EVT)
    {
    case EVT_MEAS_STOP:
      state = STATE_WAIT;
      break;
    case EVT_BLE_DISCONNECTED:
      state = STATE_ADVERTISE;
      break;
    case EVT_BUTTON_A_SHORT_PRESSED:
      break;
    case EVT_BUTTON_A_LONG_PRESSED:
      Serial.println("Going to System OFF");
      led->setLEDRGB(false, false, false);
      delay(1000); // delay seems important to apply settings, before going to System OFF
      // Ensure interrupt pin from IMU is set to wake up device
      nrf_gpio_cfg_sense_input(digitalPinToInterrupt(P1_13), NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
      delay(2000); // Trigger System OFF
      NRF_POWER->SYSTEMOFF = 1;
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

// 長押しの閾値 (ms)

void setup()
{
  Serial.begin(115200);
  batterySensor->initialize();
  led->initialize();
  button_initialize(); // ボタンはクラスにしたかったが、割り込み関数は静的じゃないといけないので、関数化してる
  ble->initialize();
  ble->advertiseStart();
  sensor->initialize();

  time_s = micros();
  state = STATE_ADVERTISE;

  // BUTTON DEBUG
  led->setLEDRGB(false, false, true);
  // attachInterrupt(digitalPinToInterrupt(P1_13), handleInterrupt, CHANGE);
}

void loop()
{
  MyEvent EVT = Detect_Event(); // イベント検知
  doAction(EVT);                // イベントに応じたアクション実行
  Release_Event(EVT);           // イベント開放
  delayMicroseconds(1);         // ディレイ
}