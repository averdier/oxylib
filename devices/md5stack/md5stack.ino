/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <StateMachine.h>
#include <Preferences.h>
#include <M5Stack.h>

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/

#define BLE_SERVICE_UUID          "1bfc18c5-a691-47c1-aa0b-f10992bf5a1a"
#define BLE_OXYGEN_TX             "deb5a19f-759a-4ec3-9a7f-a554bf6722c6"
#define BLE_OXYGEN_RX             "bafbee31-4442-4e1a-be74-1ae65a8caf88"
#define BLE_WIFI_ENABLED_RXTX     "6a8e7657-1b99-447a-aee0-38128a948c48"
#define BLE_WIFI_STATUS_TX        "599ebf9c-f2b3-411e-861c-fe018f837309"
#define BLE_WIFI_CONFIG_RX        "6c1ff9cd-ae32-45ba-9d40-bb3d258fbee4"

#define BLE_DEFAULT_DEVICE_ID     "oxylib-02012345"

/*****************************************************************************/
/*GLOBAL DATA                                                                */
/*****************************************************************************/

// BLE Definition
BLEServer* bleServer = NULL;
BLECharacteristic* bleOxygenTx = NULL;
BLECharacteristic* bleOxygenRx = NULL;
BLECharacteristic* bleWifiEnabledRxTx = NULL;
BLECharacteristic* bleWifiStatusTx = NULL;
BLECharacteristic* bleWifiConfigRx = NULL;

// Hardware state
bool bleConnected = false;
bool bleOldConnected = false;
uint32_t oxygenLevel = 0;
uint32_t oxygenTarget = 0;

// Saved preferences
Preferences preferences;

bool testMode = false;

// Display buffer
TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);
extern const unsigned short icon_ble[0x4000];
extern const unsigned short icon_ble_disconnect[0x4000];

/*****************************************************************************/
/*FUNCTIONS                                                                  */
/*****************************************************************************/

uint32_t readOxygenLevel () {
  uint32_t result = oxygenLevel;
  // DEV, read sensor here
  if (oxygenLevel != oxygenTarget) {
    if (oxygenLevel < oxygenTarget) {
      if (oxygenLevel + 1 <= 100) {
        result += 1;
      }
    }
    else {
      if (oxygenLevel - 1 >= 0) {
        result -= 1;
      }
    }
    delay(10);
  }

  return result;
}

void updateDevice () {
  uint32_t newOxygenLevel = readOxygenLevel();
  if (newOxygenLevel != oxygenLevel) {
    oxygenLevel = newOxygenLevel;
    notifyOxygenLevel();
  }
  // BLE Disconnecting
  if (!bleConnected && bleOldConnected) {
    delay(500);
    bleServer->startAdvertising();
    bleOldConnected = bleConnected;
  }

  // BLE Connecting
  if (bleConnected && !bleOldConnected) {
    bleOldConnected = bleConnected;
  }
}

/*****************************************************************************/
/*DISPLAY                                                                    */
/*****************************************************************************/

void displayBuffer () {
  if (testMode) {
    Disbuff.setTextSize(1);
    Disbuff.setTextColor(RED);
    Disbuff.drawString("Test Mode", 0, 0, 1);
    Disbuff.setTextColor(WHITE);
  }
  Disbuff.pushSprite(0, 0);
}

void bootScreen () {
  uint16_t ellips[7] = { 70, 78, 86, 93, 100, 108, 116};

  for (int n = 0; n < 7; n++) {
    Disbuff.fillEllipse(160,120,ellips[n]/2, ellips[n]/2, 0x2b2b2b);
    displayBuffer();
    delay(20);
  }

  int color_bk = 43;
  for (int n = 116; n < 180; n=n+8) {
    Disbuff.fillEllipse(160,120,n/2, n/2, 0x2b2b2b);
    displayBuffer();
    delay(20);
    color_bk -= 5;
  }
}

void bleScreen () {
  while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
    Disbuff.fillRect(0, 0, 320, 240, BLACK);

    if (bleConnected) {
      Disbuff.pushImage(96, 16, 128, 128, (uint16_t *)icon_ble);
    }
    else {
      Disbuff.pushImage(96, 16, 128, 128, (uint16_t *)icon_ble_disconnect);
    }

    Disbuff.setTextSize(2);
    Disbuff.setCursor(12, 184);

    if (bleConnected) {
      Disbuff.setTextColor(GREEN);
      Disbuff.printf("BLE connected\n");
    }
    else {
      Disbuff.setTextColor(RED);
      Disbuff.printf("BLE disconnect\n");
    }

    Disbuff.setCursor(12, 204);
    Disbuff.setTextColor(Disbuff.color565(18,150,219));
    Disbuff.printf("BLE Name: ");
    Disbuff.printf(preferences.getString("bleId", BLE_DEFAULT_DEVICE_ID).c_str());
    Disbuff.printf("\n");

    displayBuffer();
        
    M5.update();
    updateDevice();
    delay(100);
  }
  while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
  {
    M5.update();
    updateDevice();
    delay(10);
  }
}

void oxygenScreen () {
  while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
    Disbuff.fillRect(0, 0, 320, 240, BLACK);

    Disbuff.setTextSize(3);
    Disbuff.setTextColor(WHITE);

    Disbuff.setCursor(12, 12);
    Disbuff.printf("Oxygen level:\n");

    Disbuff.setTextSize(7);
    Disbuff.setCursor(120, 100);
    Disbuff.printf(String(oxygenLevel).c_str());

    Disbuff.printf("%%\n");

    displayBuffer();
    
    M5.update();
    updateDevice();
    delay(100);
  }
  while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
  {
    M5.update();
    updateDevice();
    delay(10);
  }
}

/*****************************************************************************/
/*EVENTS                                                                     */
/*****************************************************************************/

// Bluetooth server callback
class ServerCallback: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    bleConnected = true;
    Serial.println("BLE connected");
  }

  void onDisconnect(BLEServer* pServer) {
    bleConnected = false;
    Serial.println("BLE disconnected");
  }
};

// Oxygen write callback
class OxygenRxCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string strValue = pCharacteristic->getValue();
    if (strValue.length() > 0) {
      uint32_t value = (uint32_t) strValue[0];
      Serial.print("New oxygen target: ");
      Serial.print(value);
      Serial.println("%");
      oxygenTarget = value;
    }
  }
};

// WiFi enabled write callback
class WifiEnabledRxCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string strValue = pCharacteristic->getValue();
    if (strValue.length() > 0) {
      bool value = strValue[0] ? true : false;
      Serial.print("New WiFi enabled target: ");
      Serial.println(value);
    }
  }
};

// WiFi configuration write callback
class WifiConfigRxCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string strValue = pCharacteristic->getValue();
    if (strValue.length() > 0) {
      Serial.print("New WiFi configuration target: ");
      Serial.println(strValue.c_str());
    }
  }
};

void notifyOxygenLevel () {
  if (bleConnected) {
    bleOxygenTx->setValue((uint8_t*)&oxygenLevel, 4);
    bleOxygenTx->notify();
  }
}

/*****************************************************************************/
/*SETUP                                                                      */
/*****************************************************************************/

bool setupBle () {
  String bleDeviceId = preferences.getString("bleId", BLE_DEFAULT_DEVICE_ID);
  BLEDevice::init(bleDeviceId.c_str());

  bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new ServerCallback);
  BLEService *oxylibService = bleServer->createService(BLE_SERVICE_UUID);

  // WiFi Enabled
  bleWifiEnabledRxTx = oxylibService->createCharacteristic(
    BLE_WIFI_ENABLED_RXTX,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );
  bleWifiEnabledRxTx->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  uint32_t devWifiEnabled = preferences.getBool("wifiEnabled") ? 1 : 0; // TODO: Do cleaner
  bleWifiEnabledRxTx->setValue((uint8_t*)&devWifiEnabled, 4);
  bleWifiEnabledRxTx->setCallbacks(new WifiEnabledRxCallback());

  // WiFi Config
  bleWifiConfigRx = oxylibService->createCharacteristic(
    BLE_WIFI_CONFIG_RX,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );
  bleWifiConfigRx->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  bleWifiConfigRx->setValue("{}");
  bleWifiConfigRx->setCallbacks(new WifiEnabledRxCallback());

  // Wifi Status
  bleWifiStatusTx = oxylibService->createCharacteristic(
    BLE_WIFI_STATUS_TX,
    BLECharacteristic::PROPERTY_READ    |
    BLECharacteristic::PROPERTY_WRITE   |
    BLECharacteristic::PROPERTY_NOTIFY  |
    BLECharacteristic::PROPERTY_INDICATE
  );
  bleWifiStatusTx->addDescriptor(new BLE2902());
  uint32_t devWifiStatus = 0; // TODO: Do cleaner
  bleWifiStatusTx->setValue((uint8_t*)&devWifiStatus, 4);

  // Oxygen TX
  bleOxygenTx = oxylibService->createCharacteristic(
    BLE_OXYGEN_TX,
    BLECharacteristic::PROPERTY_READ    |
    BLECharacteristic::PROPERTY_WRITE   |
    BLECharacteristic::PROPERTY_NOTIFY  |
    BLECharacteristic::PROPERTY_INDICATE
  );
  bleOxygenTx->addDescriptor(new BLE2902());
  bleOxygenTx->setValue((uint8_t*)&oxygenLevel, 4);

  // Oxygen RX
  bleOxygenRx = oxylibService->createCharacteristic(
    BLE_OXYGEN_RX,
    BLECharacteristic::PROPERTY_READ    |
    BLECharacteristic::PROPERTY_WRITE
  );
  bleOxygenRx->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  bleOxygenRx->setValue((uint8_t*)&oxygenLevel, 4);
  bleOxygenRx->setCallbacks(new OxygenRxCallback());

  oxylibService->start();
  BLEAdvertising *bleAdvertising = BLEDevice::getAdvertising();
  bleAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  bleAdvertising->setScanResponse(false);
  bleAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();

  BLESecurity *bleSecurity = new BLESecurity();
  bleSecurity->setStaticPIN(123456);

  return true;
}

void setup () {
  M5.begin();
  Serial.println("Start setup");
  M5.update();
  Serial.println("M5 initialized");

  preferences.begin("device_config", false);
  Disbuff.setColorDepth(8); 
  Disbuff.createSprite(320, 240);
  setupBle();
  bootScreen();
  Serial.println("Setup complete");
}

/*****************************************************************************/
/*LOOP                                                                       */
/*****************************************************************************/

void loop () {
  bleScreen();
  oxygenScreen();
}
