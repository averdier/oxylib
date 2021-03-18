/*****************************************************************************/
/*INCLUDES                                                                   */
/*****************************************************************************/
#include "Arduino.h"

#include <ArduinoBLE.h>

#include "Nano33BLEPressure.h"
#include "Nano33BLETemperature.h"

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/
/* 
 * We use strings to transmit the data via BLE, and this defines the buffer
 * size used to transmit these strings. Only 20 bytes of data can be 
 * transmitted in one packet with BLE.
 */
#define BLE_BUFFER_SIZES              20
/* Device name which can be scene in BLE scanning software. */
#define BLE_DEVICE_NAME               "Oxylib Nano 33 - Dev"
/* Local name which should pop up when scanning for BLE devices. */
#define BLE_LOCAL_NAME                "oxylib-01012345"
/* Characteristics update rate */
#define BLE_UPDATE_RATE               1000

/*
 * We use led to indicate oxygen level
 */
#define RED                           22
#define GREEN                         23
#define BLUE                          24

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/
/* 
 * Objects which we will store data in each time we read
 * the each sensor. 
 */
Nano33BLEPressureData pressureData;
Nano33BLETemperatureData temperatureData;
int axisRotation;

/* 
 * Declares the BLEService and characteristics we will need for the BLE 
 * transfer. The UUID was randomly generated using one of the many online 
 * tools that exist.
 */
BLEService BLEOxylibService("1bfc18c5-a691-47c1-aa0b-f10992bf5a1a");
BLECharacteristic pressureBLE("3dd37aa3-a1b8-45e9-b555-349f8f421c42", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic tempertaureBLE("ea668c2a-fd7e-45d0-85fa-a5c8907754b8", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic humidityBLE("cb599054-bcb0-4fdd-9a1b-034c3c8d7816", BLERead | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);
BLECharacteristic oxygenBLE("deb5a19f-759a-4ec3-9a7f-a554bf6722c6", BLERead | BLEWrite | BLENotify | BLEBroadcast, BLE_BUFFER_SIZES);

/* Common global buffer will be used to write to the BLE characteristics. */
char bleBuffer[BLE_BUFFER_SIZES];

/**
 * Common global variable will be used to write the last notify time
 */
unsigned long lastUpdateTime;

/*****************************************************************************/
/*FUNCTIONS                                                                  */
/*****************************************************************************/

void setupLed () {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void ledOff () {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
}

void ledGreen () {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
}

void ledBlue () {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, LOW);
}

void ledRed () {
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
}

/*
 * Update BLE characteristics from device data to listeners
 */
void updateBLECharacteristics () {
  if (millis() - lastUpdateTime > BLE_UPDATE_RATE) {
    int writeLength;
  
    if (Temperature.pop(temperatureData)) {
      // Temperature
      writeLength = sprintf(bleBuffer, "%f", temperatureData.temperatureCelsius);
      tempertaureBLE.writeValue(bleBuffer, &writeLength);
  
      // Humidity
      writeLength = sprintf(bleBuffer, "%f", temperatureData.humidity);
      humidityBLE.writeValue(bleBuffer, &writeLength);
    }
  
    if (Pressure.pop(pressureData)) {
      // Pressure
      writeLength = sprintf(bleBuffer, "%f", pressureData.barometricPressure);
      pressureBLE.writeValue(bleBuffer, &writeLength);
    }
  
    // TODO : update oxygen property from rotation sensor
    byte oxygen[1];
    oxygen[0] = axisRotation;

    // Update led from oxygen percent
    if (oxygen[0] <= 33) {
      ledRed();
    }
    else {
      if (oxygen[0] <= 66) {
        ledBlue();
      }
      else {
        ledGreen();
      }
    }
    oxygenBLE.writeValue(oxygen, sizeof(oxygen));
  
    lastUpdateTime = millis();

  }
}

/*
 * On oxygen characteristic written
 */
void oxygenCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  /* Get target value */
  Serial.print("Characteristic event, read: ");
  byte targetPercent = 0;
  oxygenBLE.readValue(targetPercent);
  Serial.println(targetPercent);

  /* TODO: Compute angle for motor */
  int targetAngle = (int) targetPercent;
  rotateMotor(targetAngle);
}

void turnOnMotor () {
  /* TODO: Set power pin to 1 */
}

void turnOffMotor () {
  /* TODO: Set power pin to 0 */
}

void rotateMotor (int angle) {
  /* TODO: Set PWL for motor */
  axisRotation = angle;
}

void readOxygen () {
  /* TODO: Read value from sensor */
}

/*****************************************************************************/
/*SETUP                                                                      */
/*****************************************************************************/
void setup() {
  Serial.begin(115200);

  /* 
   * Initialises sensors, and starts the 
   * periodic reading of the sensor using a Mbed OS thread. 
   * The data is placed in a circular buffer and can be read whenever.
   */
  Pressure.begin();
  Temperature.begin();

  /* BLE Setup. For information, search for the many ArduinoBLE examples.*/
  if (!BLE.begin())
  {
    while (1);    
  }
  else
  {
    BLE.setDeviceName(BLE_DEVICE_NAME);
    BLE.setLocalName(BLE_LOCAL_NAME);
    
    BLE.setAdvertisedService(BLEOxylibService);
    BLEOxylibService.addCharacteristic(pressureBLE);
    BLEOxylibService.addCharacteristic(tempertaureBLE);
    BLEOxylibService.addCharacteristic(humidityBLE);
    BLEOxylibService.addCharacteristic(oxygenBLE);
    oxygenBLE.setEventHandler(BLEWritten, oxygenCharacteristicWritten);
    
    BLE.addService(BLEOxylibService);
    BLE.advertise();
  }

  lastUpdateTime = 0;
  delay(500);
}

/*****************************************************************************/
/*MAIN LOOP                                                                  */
/*****************************************************************************/
void loop() {
  BLEDevice central = BLE.central();
  if(central)
  {
    turnOnMotor();
    
    while(central.connected())
    {
      updateBLECharacteristics();
    }
    
    turnOffMotor();
  }
}