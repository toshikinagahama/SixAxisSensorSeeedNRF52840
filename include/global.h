#ifndef GLOBAL_H
#define GLOBAL_H

#define ADDRESS_MAJOR_VERSION 0x00
#define ADDRESS_MINOR_VERSION 0x01
#define ADDRESS_REVISION_VERSION 0x02

#include <Arduino.h>

/**
 * @brief 割り込みビット
 *
 */
struct INT_BIT_T
{
  uint8_t BLE_CONNECTED = 0;          // BLE端末と接続
  uint8_t BLE_DISCONNECTED = 0;       // BLE端末と切断
  uint8_t CMD_MEAS_START = 0;         // 測定開始コマンド
  uint8_t CMD_MEAS_STOP = 0;          // 測定終了コマンド
  uint8_t CMD_GET_DEVICE_INFO = 0;    // 機器情報取得コマンド
  uint8_t BUTTON_A_SHORT_PRESSED = 0; // ボタンA単押し
  uint8_t BUTTON_A_LONG_PRESSED = 0;  // ボタンA長押し
};

/**
 * @brief 割り込み構造体
 *
 */
struct INT_T
{
  struct INT_BIT_T BIT; // ビットフィールド
};

/**
 * @brief システム構造体
 *
 */
struct SYS_T
{
  String DEVICE_SERIAL_NUM = "";
  String DEVICE_VERSION = "";
  String BLE_ARG = ""; // BLEコマンドの引数
};

// 外部宣言
extern INT_T INT;
extern SYS_T SYS;

#endif // GLOBAL_H