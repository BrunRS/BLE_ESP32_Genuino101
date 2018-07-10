//BLE
#include <CurieBLE.h>
//MPL3115A2
#include <Wire.h>
#include "SparkFunMPL3115A2.h"

//defines
const int ledPin = 13; // set ledPin to on-board LED
const int buttonPin = 4; // set buttonPin to digital pin 4
const int HIH3605_Pin = A0; //analog pin 0

MPL3115A2 myPressure;

//BLE
//BLEService ledService("00001815-E8F2-537E-4F6C-D104768A1214"); // create service
BLEService ledService("00001815-0000-1000-8000-00805f9b34fb"); // create service
// create switch characteristic and allow remote device to read and write
BLEUnsignedLongCharacteristic ledCharacteristic("00002a6d-0000-1000-8000-00805f9b34fb", BLERead | BLENotify);
// create button characteristic and allow remote device to get notifications
BLECharCharacteristic buttonCharacteristic("00002a56-0000-1000-8000-00805f9b34fb", BLERead | BLENotify); // allows remote device to get notifications
// create button characteristic and allow remote device to get notifications
BLEUnsignedLongCharacteristic tempCharacteristic("00002a1f-0000-1000-8000-00805f9b34fb", BLERead | BLENotify); // allows remote device to get notifications
// create button characteristic and allow remote device to get notifications
BLEUnsignedLongCharacteristic humCharacteristic("00002a6f-0000-1000-8000-00805f9b34fb", BLERead | BLENotify); // allows remote device to get notifications

void setup() {
  Serial.begin(9600);

  //defines
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output
  pinMode(buttonPin, INPUT); // use button pin 4 as an input

  //MPL3115A2
  Wire.begin();        // Join i2c bus
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 

  //BLE
  // begin initialization
  BLE.begin();
  // set the local name peripheral advertises
  BLE.setLocalName("Genuino");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);
  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);
  ledService.addCharacteristic(tempCharacteristic);
  ledService.addCharacteristic(humCharacteristic);
  // add the service
  BLE.addService(ledService);
  ledCharacteristic.setValue(0);
  buttonCharacteristic.setValue(0);
  tempCharacteristic.setValue(0);
  humCharacteristic.setValue(0);
  // start advertising
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();

  
 

  //MPL3115A2
  uint32_t pressure = (uint32_t)myPressure.readPressure();
  //float pressure=101325f;
  uint32_t temperature =(uint32_t)myPressure.readTemp();

//HIH3605
  //To properly caculate relative humidity, we need the temperature.
  uint32_t relativeHumidity = (uint32_t) getHumidity(temperature);
 
  // read the current button pin state
  char buttonValue = digitalRead(buttonPin);
  // has the value changed since the last read
  boolean buttonChanged = (buttonCharacteristic.value() != buttonValue);
 // if (buttonChanged) {
    Serial.println("");
    // button state changed, update characteristics
    ledCharacteristic.setValue(pressure);
    Serial.println((uint32_t) pressure);
    tempCharacteristic.setValue(temperature);
    Serial.println((uint32_t) temperature);
    buttonCharacteristic.setValue(buttonValue);
    Serial.println((short) buttonValue);
    humCharacteristic.setValue(relativeHumidity);
    Serial.println(relativeHumidity);
    delay(2000);
 /* }
  if (ledCharacteristic.written() || buttonChanged) {
    // update LED, either central has written to characteristic or button state has changed
    if (buttonCharacteristic.value()) {
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }
  }*/
}

//HIH3605
float getHumidity(float degreesCelsius) {
  //caculate relative humidity
  //float supplyVolt = 5.0;
  int HIH3605_Value = analogRead(HIH3605_Pin);// read the value from the sensor

  float trueRH =( HIH3605_Value>>5)+40;
 /* float voltage = HIH3605_Value / 1023. * supplyVolt; // convert to voltage value
  // convert the voltage to a relative humidity
  // - the equation is derived from the HIH-4030/31 datasheet
  // - it is not calibrated to your individual sensor
  // Table 2 of the sheet shows the may deviate from this line
  float sensorRH = 161.0 * voltage / supplyVolt - 25.8;
  float trueRH = sensorRH / (1.0546 - 0.00216 * degreesCelsius); //temperature adjustment
  //Vout=(Vsupply)(0.0062(sensorRH) + 0.16)*/
  return trueRH;
}
