/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <StateMachine.h>

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
#define BLE_BUFFER_SIZES       20
#define BLE_UPDATE_RATE        100
#define STATE_DELAY            1000
#define SERVICE_UUID          "1bfc18c5-a691-47c1-aa0b-f10992bf5a1a"
#define OXYGEN_TX             "deb5a19f-759a-4ec3-9a7f-a554bf6722c6"
#define OXYGEN_RX             "bafbee31-4442-4e1a-be74-1ae65a8caf88"
#define WIFI_ENABLED          "6a8e7657-1b99-447a-aee0-38128a948c48"

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

// BLE Definition
BLEServer* bleServer = NULL;
BLECharacteristic* bleOxygenTx = NULL;
BLECharacteristic* bleOxygenRx = NULL;
BLECharacteristic* bleWifiEnabled = NULL;
bool bleConnected = false;
bool bleOldConnected = false;

// Device state
uint32_t oxygenTarget = 0;
uint32_t oxygenValue = 0;
uint32_t oxygenOldValue = 0;
unsigned long lastUpdateTime;

// Hardware state
bool motorOn = false;
bool motorInverse = false; // horaire = false, antihoraire = true
bool wifiEnabled = false;

// State machine
StateMachine machine = StateMachine();
State* StateIdle     = machine.addState(&idleState);
State* StateRotate   = machine.addState(&rotateState);

/*****************************************************************************/
/*FUNCTIONS                                                                  */
/*****************************************************************************/

void turnOnMotor () {
  // TODO code here
  motorOn = true;
}

void turnOffMotor () {
  // TODO code here
  motorOn = false;
}

void readOxygenLevel () {
  oxygenOldValue = oxygenValue;
  // TODO code here

  // Dev
  if (oxygenValue != oxygenTarget) {
    if (motorInverse) {
      oxygenValue -= 1;
    }
    else {
      oxygenValue += 1;
    }
    delay(5);
  }
}

bool isManualOverride () {
  return (
    oxygenValue < oxygenOldValue && !motorInverse
  ) || (
    oxygenValue > oxygenOldValue && motorInverse
  );
}

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
      oxygenTarget = (uint32_t) value[0];

      if (oxygenValue != oxygenTarget) {
        Serial.print("New oxygen target: ");
        Serial.print(oxygenTarget);
        Serial.println("%");

        if (oxygenValue < oxygenTarget) {
          motorInverse = false;
        }
        else {
          motorInverse = true;
        }

        if (!motorOn) {
          turnOnMotor();
        }

        // TODO find and set motor pwm
      }
    }
  }
};

// Notify characteristics
void updateBLECharacteristics () {
  if (millis() - lastUpdateTime > BLE_UPDATE_RATE) {
    bleOxygenTx->setValue((uint8_t*)&oxygenTarget, 4);
    bleOxygenTx->notify();
    lastUpdateTime = millis();
  }
}

/*****************************************************************************/
/* State Machine                                                             */
/*****************************************************************************/

/****************************************/
/* States                               */
/****************************************/

void idleState () {
  readOxygenLevel();
}

void rotateState () {
  readOxygenLevel();

  if (bleConnected) {
    updateBLECharacteristics();
  }

  if (isManualOverride()) {
    turnOffMotor();
  }
}

/****************************************/
/* Transition                           */
/****************************************/

bool idleToItself () {
  if (oxygenValue != oxygenOldValue) {
    // If move return false for next transition
    return false;
  }
  return true;
}

bool idleToRotate () {
  if (oxygenValue != oxygenOldValue) { 
    return true;
  }
  return false;
}

bool rotateToItself () {
  if (oxygenValue != oxygenOldValue) { 
    return true;
  }
  return false;
}

bool rotateToIdle () {
  if (oxygenValue == oxygenOldValue) {
    if (motorOn) {
      turnOffMotor();
    }
    return true;
  }
  return false;
}



/*****************************************************************************/
/*SETUP                                                                      */
/*****************************************************************************/
void setup() {
  Serial.begin(115200);

  // BLE Setup
  BLEDevice::init("oxylib-02012345");
  bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new ServerCallback);
  BLEService *oxylibService = bleServer->createService(SERVICE_UUID);

  bleWifiEnabled = oxylibService->createCharacteristic(
    WIFI_ENABLED,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );
  bleWifiEnabled->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  bleOxygenRx = oxylibService->createCharacteristic(
    OXYGEN_RX,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );
  bleOxygenRx->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  bleOxygenRx->setValue((uint8_t*)&oxygenTarget, 4);
  bleOxygenRx->setCallbacks(new OxygenCallback());

  bleOxygenTx = oxylibService->createCharacteristic(
    OXYGEN_TX,
    BLECharacteristic::PROPERTY_READ    |
    BLECharacteristic::PROPERTY_WRITE   |
    BLECharacteristic::PROPERTY_NOTIFY  |
    BLECharacteristic::PROPERTY_INDICATE
  );
  bleOxygenTx->addDescriptor(new BLE2902());
  bleOxygenTx->setValue((uint8_t*)&oxygenTarget, 4);

  oxylibService->start();
  BLEAdvertising *bleAdvertising = BLEDevice::getAdvertising();
  bleAdvertising->addServiceUUID(SERVICE_UUID);
  bleAdvertising->setScanResponse(false);
  bleAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();

  BLESecurity *bleSecurity = new BLESecurity();
  bleSecurity->setStaticPIN(123456);

  // State machine setup
  StateIdle->addTransition(&idleToItself, StateIdle);
  StateIdle->addTransition(&idleToRotate, StateRotate);

  StateRotate->addTransition(&rotateToItself, StateRotate);
  StateRotate->addTransition(&rotateToIdle, StateIdle);
}

/*****************************************************************************/
/*MAIN LOOP                                                                  */
/*****************************************************************************/
void loop() {

  // Run state machine
  machine.run();

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

  // Lower speed when possible
  if (!bleConnected) {
    delay(STATE_DELAY);
  }
}