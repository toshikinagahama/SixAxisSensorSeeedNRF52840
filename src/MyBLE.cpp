#include "MyBLE.h"
#include "global.h"

MyBLE::MyBLE()
{
  // コンストラクタ
  this->SENSOR_Service = new BLEService(BLE_SENSOR_SERVICE_UUID);
  this->Battery_Service = new BLEService(BLE_BATTERY_SERVICE_UUID);
  this->SENSOR_chara = new BLECharacteristic(BLE_SENSOR_CHARA_UUID, BLEWrite | BLERead | BLENotify, 20);
  this->Battery_chara = new BLEIntCharacteristic(BLE_BATTERY_CHARA_UUID, BLERead | BLENotify);
  this->SENSOR_Descriptor = new BLEDescriptor(BLE_DESCRIPTOR_UUID, "SENSOR");
  this->Battery_Descriptor = new BLEDescriptor(BLE_DESCRIPTOR_UUID, "Battery Level: 0 - 100");
}

MyBLE::~MyBLE()
{
  // デストラクタ
  delete this->SENSOR_Service;
  delete this->Battery_Service;
  delete this->SENSOR_chara;
  delete this->Battery_chara;
  delete this->Battery_Descriptor;
  delete this->SENSOR_Descriptor;
}

void MyBLE::SensorCharaWriteHandler(BLEDevice central, BLECharacteristic chara)
{
}

void MyBLE::SensorCharaReadHandler(BLEDevice central, BLECharacteristic chara)
{
  uint8_t val[20];
  chara.readValue(val, 20);
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
  BLE.setAdvertisedService(*this->SENSOR_Service);                // add the service UUID
  BLE.setAdvertisedService(*this->Battery_Service);               // add the service UUID
  this->Battery_chara->addDescriptor(*this->Battery_Descriptor);  // add descriptor
  this->SENSOR_Service->addCharacteristic(*this->SENSOR_chara);   // add characteristic
  this->Battery_Service->addCharacteristic(*this->Battery_chara); // add characteristic
  this->SENSOR_chara->addDescriptor(*this->SENSOR_Descriptor);    // add descriptor
  this->SENSOR_chara->setEventHandler(BLERead, this->SensorCharaReadHandler);
  this->SENSOR_chara->setEventHandler(BLEWrite, this->SensorCharaWriteHandler);
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
