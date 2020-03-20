/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 3; //In seconds
BLEScan* pBLEScan;


// what to do upon discovering a new device during a scan interval
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      std::string address = advertisedDevice.getAddress().toString();
      std::string deviceName = advertisedDevice.getName();
      int rssi= advertisedDevice.getRSSI();
      esp_ble_addr_type_t addressType = advertisedDevice.getAddressType();

      
      if (advertisedDevice.haveName()){
          Serial.print("Device Name: ");
          Serial.println(deviceName.c_str());
      }
      Serial.print(" Address: ");
      Serial.print(address.c_str());

      // BLE_ADDR_TYPE_PUBLIC = 0x00 (0)
      // BLE_ADDR_TYPE_RANDOM = 0x01 (1) - static random address
      // BLE_ADDR_TYPE_RPA_PUBLIC = 0x02 (2) - resolvable (uses secret ID key (IRK))
      // randomly generated address that changes after every connection
      // BLE_ADDR_TYPE_RPA_PRIVATE = 0x03 (3) - resolvable private random address
      
      Serial.print(" Address Type: ");
      Serial.print(addressType);
      Serial.print(" RSSI: ");
      Serial.println( rssi);    
      }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval valu
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
