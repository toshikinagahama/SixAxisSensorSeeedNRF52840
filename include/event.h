#ifndef EVENT_H

#ifdef EVENT_H
#define EXTERN extern
#else
#define EXTERN
#endif
#define EVENT_H

#include <Arduino.h>

enum MyState
{
  STATE_WAIT,
  STATE_ADVERTISE,
  STATE_MEAS,
  STATE_MAX,
};

enum MyEvent
{
  EVT_NOP,
  EVT_BLE_CONNECTED,
  EVT_BLE_DISCONNECTED,
  EVT_MEAS_START,
  EVT_MEAS_STOP,
  EVT_GET_DEVICE_INFO,
};

// 外部宣言
extern MyEvent Detect_Event();
extern void Release_Event(MyEvent);
#endif // EVENT_H