/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// BLE Scanner setup
int scanDuration = 3; //In seconds
BLEScan* pBLEScan;

bool deviceFound = false;

BLEAddress targetAddress("fc:f5:c4:0f:6c:36");

//class targetDevice { } should implement in future...

// what to do upon discovering a new device during a scan interval
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      BLEAddress address = advertisedDevice.getAddress();
      std::string deviceName = advertisedDevice.getName();
      int rssi= advertisedDevice.getRSSI();
      esp_ble_addr_type_t addressType = advertisedDevice.getAddressType();

      if (address.equals(targetAddress)){
          deviceFound = true;
          if (advertisedDevice.haveName()){
              Serial.print("Device Name: ");
              Serial.println(deviceName.c_str());
          }
          Serial.print("Address: ");
          Serial.print(address.toString().c_str());

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
    }
};

// BLE peripheral device should rotate through each advertising channel
// and send advertisement packets in sequence. 
// 
void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  // the interval to scan (milliseconds)
  
  // only one advertising channel is being looked at during each scanInterval
  // after 100ms, scanner will switch to next channel
  // frequency of which scanner hops between advertising channels
  pBLEScan->setInterval(100);

  // time spent scanning within a channel sniffing interval (prior to a hop)
  pBLEScan->setWindow(99);  // less or equal setInterval valu

}

void loop() {
  // put your main code here, to run repeatedly:
  // 'scanTime' paramater refers to duration in seconds of each scan
  // (how long should we hop through advertising channels and scan for?)
  BLEScanResults foundDevices = pBLEScan->start(scanDuration, false);
  //Serial.print("Devices found: ");
  //Serial.println(foundDevices.getCount());
  Serial.print("Scan done. Target device found: ");
  if (deviceFound){
    Serial.println("True");
  }
  else{
    Serial.println("False");
  }
 
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory

  // reset our success indicator before next scan
  deviceFound = false;

  // How long to delay until we start of the next scan.
  delay(2000);
}
