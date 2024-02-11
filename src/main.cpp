#include <Arduino.h>
#include <nrf52840.h>
#include "global.h"
#include "MyBLE.h"
#include "MySensor.h"
#include "ui.h"
#include "event.h"

ulong time_s0 = 0;
ulong time_s = 0;
ulong time_e = 0;
ulong cnt = 0;

MyState state;
MyBLE *ble = new MyBLE();
MySensor *sensor = new MySensor();
UI *ui = new UI();
BLEDevice central;

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
  Serial.println(time_e - tmp_time_s);
  // Serial.println(time_e - tmp_time_s);
  cnt++;
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
    ui->greenBlink(1000);
    switch (EVT)
    {
    case EVT_BLE_CONNECTED:
      state = STATE_WAIT;
      break;
    default:
      break;
    }
    break;
  case STATE_WAIT:
    ui->setLEDRGB(true, false, false);
    central = BLE.central();
    if (central)
    {
      delay(100);
      Serial.print("\nConnected to central: ");
      Serial.println(central.address());

      time_s0 = micros();
      cnt = 0;
      Serial.println(central.connected());
      while (central.connected())
      {
        sampling();
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
        ble->SENSOR_chara->writeValue(val, 22);
      }
      // when the central disconnects
      Serial.print("Disconnected from central: ");
      Serial.println(central.address());
    }
    switch (EVT)
    {
    case EVT_BLE_DISCONNECTED:
      state = STATE_ADVERTISE;
      break;
    case EVT_MEAS_START:
      state = STATE_MEAS;
      break;
    case EVT_GET_DEVICE_INFO:
      break;
    default:
      break;
    }
    break;
  case STATE_MEAS:
    switch (EVT)
    {
    case EVT_MEAS_STOP:
      state = STATE_WAIT;
      break;
    case EVT_BLE_DISCONNECTED:
      state = STATE_ADVERTISE;
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  ui->initialize();
  ble->initialize();
  ble->advertiseStart();
  sensor->initialize();

  time_s = micros();
  state = STATE_ADVERTISE;
}

void loop()
{
  MyEvent EVT = Detect_Event(); // イベント検知
  doAction(EVT);                // イベントに応じたアクション実行
  Release_Event(EVT);           // イベント開放
  delayMicroseconds(1);
}