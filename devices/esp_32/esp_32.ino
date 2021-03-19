//https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/examples/BLE_notify/BLE_notify.ino

/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
#define BLE_BUFFER_SIZES       20
#define BLE_UPDATE_RATE        1000
#define SERVICE_UUID          "1bfc18c5-a691-47c1-aa0b-f10992bf5a1a"
#define OXYGEN_TX             "deb5a19f-759a-4ec3-9a7f-a554bf6722c6"
#define OXYGEN_RX             "bafbee31-4442-4e1a-be74-1ae65a8caf88"

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

// BLE Definition
BLEServer* bleServer = NULL;
BLECharacteristic* bleOxygenTx = NULL;
BLECharacteristic* bleOxygenRx = NULL;
bool bleConnected = false;
bool bleOldConnected = false;

// Device state
uint32_t oxygenPercent = 0;

unsigned long lastUpdateTime;

/*****************************************************************************/
/*FUNCTIONS                                                                  */
/*****************************************************************************/

// Bluetooth server callback
class ServerCallback: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    bleConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    bleConnected = false;
  }
};

// Oxygen rx callback
class OxygenCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      Serial.print("New target: ");
      Serial.println((uint32_t) value[0]);
      oxygenPercent = (uint32_t) value[0];
    }
  }
};

// Notify characteristics
void updateBLECharacteristics () {
  if (millis() - lastUpdateTime > BLE_UPDATE_RATE) {
    bleOxygenTx->setValue((uint8_t*)&oxygenPercent, 4);
    lastUpdateTime = millis();
  }
}

// Initialize ble
void initBle () {
  
}

/*****************************************************************************/
/*SETUP                                                                      */
/*****************************************************************************/
void setup() {
  Serial.begin(115200);

  BLEDevice::init("oxylib-02012345");
  bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new ServerCallback);
  BLEService *oxylibService = bleServer->createService(SERVICE_UUID);

  bleOxygenTx = oxylibService->createCharacteristic(
    OXYGEN_TX,
    BLECharacteristic::PROPERTY_READ   |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE
  );
  bleOxygenTx->addDescriptor(new BLE2902());
  bleOxygenTx->setValue((uint8_t*)&oxygenPercent, 4);

  bleOxygenRx = oxylibService->createCharacteristic(
    OXYGEN_RX,
    BLECharacteristic::PROPERTY_READ   |
    BLECharacteristic::PROPERTY_WRITE
  );
  bleOxygenRx->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  bleOxygenRx->setValue((uint8_t*)&oxygenPercent, 4);
  bleOxygenRx->setCallbacks(new OxygenCallback());

  oxylibService->start();
  BLEAdvertising *bleAdvertising = bleServer->getAdvertising();
  bleAdvertising->start();

  BLESecurity *bleSecurity = new BLESecurity();
  bleSecurity->setStaticPIN(123456);

  Serial.println("Setup complete");
}

/*****************************************************************************/
/*MAIN LOOP                                                                  */
/*****************************************************************************/
void loop() {

  if (bleConnected) {
    updateBLECharacteristics();
  }

  // Disconnecting
  if (!bleConnected && bleOldConnected) {
    delay(500);
    bleServer->startAdvertising();
    bleOldConnected = bleConnected;
  }

  // Connecting
  if (bleConnected && !bleOldConnected) {
    bleOldConnected = bleConnected;
  }
}