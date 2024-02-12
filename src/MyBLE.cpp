#include "MyBLE.h"
#include "global.h"

MyBLE::MyBLE()
{
  // コンストラクタ
  this->SENSOR_Service = new BLEService(BLE_SENSOR_SERVICE_UUID);
  this->Battery_Service = new BLEService(BLE_BATTERY_SERVICE_UUID);
  this->SENSOR_TX_Chara = new BLECharacteristic(BLE_SENSOR_TX_CHARA_UUID, BLENotify, 22);
  this->SENSOR_RX_Chara = new BLECharacteristic(BLE_SENSOR_RX_CHARA_UUID, BLEWrite, 4);
  this->Battery_chara = new BLEIntCharacteristic(BLE_BATTERY_CHARA_UUID, BLERead | BLENotify);
  this->SENSOR_Descriptor = new BLEDescriptor(BLE_DESCRIPTOR_UUID, "SENSOR");
  this->Battery_Descriptor = new BLEDescriptor(BLE_DESCRIPTOR_UUID, "Battery Level: 0 - 100");
}

MyBLE::~MyBLE()
{
  // デストラクタ
  delete this->SENSOR_Service;
  delete this->Battery_Service;
  delete this->SENSOR_TX_Chara;
  delete this->SENSOR_RX_Chara;
  delete this->Battery_chara;
  delete this->Battery_Descriptor;
  delete this->SENSOR_Descriptor;
}

void MyBLE::SensorCharaWrittenHandler(BLEDevice central, BLECharacteristic chara)
{
  // 受け取ったメッセージごとに処理
  // Serial.println(chara.value());
  uint8_t data[2];
  chara.readValue(data, 2);

  // debug
  Serial.print(data[0]);
  Serial.print(",");
  Serial.println(data[1]);
  switch (data[0])
  {
  case 0x00:
    // 機器情報関連
    switch (data[1])
    {
    case 0x01:
      // 機器情報取得
      INT.BIT.CMD_GET_DEVICE_INFO = 1;
      break;
    default:
      break;
    }
    break;
  case 0x01:
    // 測定関係
    switch (data[1])
    {
    case 0x00:
      // 測定開始
      INT.BIT.CMD_MEAS_START = 1;
      break;
    case 0x01:
      // 測定終了
      INT.BIT.CMD_MEAS_STOP = 1;
      break;
    default:
      break;
    }
    break;
  default:
    Serial.println("Invalid cmd");
    break;
  }
}

void MyBLE::SensorCharaReadHandler(BLEDevice central, BLECharacteristic chara)
{
}

void MyBLE::blePeripheralConnectHandler(BLEDevice central)
{

  Serial.print("Connected evenr, central: ");
  Serial.println(central.address());
  INT.BIT.BLE_CONNECTED = 1;
}

void MyBLE::blePeripheralDisconnectHandler(BLEDevice central)
{
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  INT.BIT.BLE_DISCONNECTED = 1;
}

void MyBLE::initialize()
{
  BLE.end();
  if (!BLE.begin())
  {
    Serial.println("starting BLE failed!");
    while (1)
      ;
  }
  BLE.setEventHandler(BLEConnected, this->blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, this->blePeripheralDisconnectHandler);
  BLE.setDeviceName(BLE_LOCAL_NAME);
  BLE.setLocalName(BLE_LOCAL_NAME);
  BLE.setAdvertisedService(*this->SENSOR_Service);                 // add the service UUID
  BLE.setAdvertisedService(*this->Battery_Service);                // add the service UUID
  this->Battery_chara->addDescriptor(*this->Battery_Descriptor);   // add descriptor
  this->SENSOR_Service->addCharacteristic(*this->SENSOR_TX_Chara); // add characteristic
  this->SENSOR_Service->addCharacteristic(*this->SENSOR_RX_Chara); // add characteristic
  this->Battery_Service->addCharacteristic(*this->Battery_chara);  // add characteristic
  this->SENSOR_TX_Chara->addDescriptor(*this->SENSOR_Descriptor);  // add descriptor
  this->SENSOR_RX_Chara->addDescriptor(*this->SENSOR_Descriptor);  // add descriptor
  this->SENSOR_TX_Chara->setEventHandler(BLERead, this->SensorCharaReadHandler);
  this->SENSOR_RX_Chara->setEventHandler(BLEWritten, this->SensorCharaWrittenHandler);
  BLE.addService(*this->Battery_Service);
  BLE.addService(*this->SENSOR_Service);
}

void MyBLE::poll()
{
  BLE.poll();
}

void MyBLE::advertiseStart()
{
  BLE.advertise();
}

void MyBLE::advertiseStop()
{
  BLE.stopAdvertise();
}
