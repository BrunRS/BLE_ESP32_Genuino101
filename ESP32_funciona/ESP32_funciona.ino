/*

   This sketch emulates a Heart rate watch and is able to receive BLE signals of a Polar H7 Heart Rate Sensor.
   It shows the received values in Serial and is also able to switch notificaton on the sensor on and off (using BLE2902)

   Copyright <2017> <Andreas Spiess>

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

   Based on Neil Kolban's example file: https://github.com/nkolban/ESP32_BLE_Arduino
*/

#include "BLEDevice.h"
//#include "BLEScan.h"

const int ledPin1 = 19; //Pino Led 1
const int ledPin2 = 21; //Pino Led 2
String str1, str2, str3, str4, string1;

// The remote service we wish to connect to.
//static  BLEUUID serviceUUID("00001815-e8f2-537e-4f6c-d104768a1214");
static  BLEUUID serviceUUID("00001815-0000-1000-8000-00805f9b34fb");
// The characteristic of the remote service we are interested in.
static BLEUUID    ledUUID("00002a56-0000-1000-8000-00805f9b34fb"); //buton led
static BLEUUID    pressureUUID("00002a6d-0000-1000-8000-00805f9b34fb");
static BLEUUID    tempUUID("00002a1f-0000-1000-8000-00805f9b34fb");
static BLEUUID    humUUID("00002a6f-0000-1000-8000-00805f9b34fb");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLERemoteCharacteristic* pRemotetempCharacteristic;
static BLERemoteCharacteristic* pRemoteledCharacteristic; //buton led
static BLERemoteCharacteristic* pRemotehumCharacteristic; //buton led

uint32_t teste = 0;
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  //Serial.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
  str1 = String("00002a6d-0000-1000-8000-00805f9b34fb");//pressure
  str2 = String("00002a1f-0000-1000-8000-00805f9b34fb");//temp
  str3 = String("00002a56-0000-1000-8000-00805f9b34fb");//led
  str4 = String("00002a6f-0000-1000-8000-00805f9b34fb");//hum
  string1 = String(pBLERemoteCharacteristic->getUUID().toString().c_str());
  if (string1.equals(str1)) {
    teste = pData[3] << 24 | pData[2] << 16 | pData[1] << 8 | pData[0] ;
    //Serial.printf("Pressao: %u Pa \n", teste);

    
    Serial.println(teste);//pressao enviada

    
  } else if (string1.equals(str2)) {
    teste = pData[3] << 24 | pData[2] << 16 | pData[1] << 8 | pData[0] ;
    //Serial.printf("Temperatura: %u ºC\n", teste);

    
    Serial.println(teste);//temperatura enviada

    
  } else if (string1.equals(str3)) {
    teste = pData[3] << 24 | pData[2] << 16 | pData[1] << 8 | pData[0] ;
    /*Serial.printf("Led: %u\n", teste);
    if (teste == 1)
    {
      //Serial.println("LED 1 ligado");
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
    }
    if (teste == 0)
    {
      //Serial.println("LED 2 ligado");
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin1, LOW);
    }*/
  } else if (string1.equals(str4)) {
    teste = pData[3] << 24 | pData[2] << 16 | pData[1] << 8 | pData[0] ;
    //Serial.printf("Humidades: %u %\n", teste);

    
    Serial.println(teste);//humidade enviada

    
  }

  /*for (int i = 0; i < length; i++) {
    Serial.print(pData[i]);
    Serial.print(" ");
    }*/



  //Serial.print(" ");

  /*teste= (uint32_t) pData[0]  && 0xFF000000;
    Serial.println((uint32_t) pData[0]);
    teste = (uint32_t) pData[1]  && 0x00FF0000 << 8;
    Serial.println((uint32_t) pData[1] << 8);
    teste = (uint32_t) pData[2] << 16 && 0x0000FF00;
    Serial.println(teste);
    teste = (uint32_t) pData[3] << 24 && 0x000000FF;*/
  //uint32_t* pontas = &pData;



    }



    bool connectToServer(BLEAddress pAddress) {
    Serial.print("Forming a connection to ");
    Serial.println(pAddress.toString().c_str());

    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    // Connect to the remove BLE Server.
    pClient->connect(pAddress);
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
    }
    Serial.println(" - Found our service");

    //pressure
    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(pressureUUID);
    if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(pressureUUID.toString().c_str());
    return false;
    }
    Serial.println(" - Found our characteristic");
    // Read the value of the characteristic.
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(value.c_str());
    pRemoteCharacteristic->registerForNotify(notifyCallback);



    //led
    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteledCharacteristic = pRemoteService->getCharacteristic(ledUUID);
    if (pRemoteledCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(ledUUID.toString().c_str());
    return false;
    }
    Serial.println(" - Found our characteristic");
    // Read the value of the characteristic.
    std::string valueled = pRemoteledCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(valueled.c_str());
    pRemoteledCharacteristic->registerForNotify(notifyCallback);



    //temp
    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemotetempCharacteristic = pRemoteService->getCharacteristic(tempUUID);
    if (pRemotetempCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(tempUUID.toString().c_str());
    return false;
    }
    Serial.println(" - Found our characteristic");
    // Read the value of the characteristic.
    std::string valuetemp = pRemotetempCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(valuetemp.c_str());
    pRemotetempCharacteristic->registerForNotify(notifyCallback);



    //hum
    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemotehumCharacteristic = pRemoteService->getCharacteristic(humUUID);
    if (pRemotehumCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(humUUID.toString().c_str());
    return false;
    }
    Serial.println(" - Found our characteristic");
    // Read the value of the characteristic.
    std::string valuehum = pRemotehumCharacteristic->readValue();
    Serial.print("The characteristic value was: ");
    Serial.println(valuehum.c_str());
    pRemotehumCharacteristic->registerForNotify(notifyCallback);
    }
    /**
    Scan for BLE servers and find the first one that advertises the service we are looking for.
  */
  class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
      /**
          Called for each advertising BLE server.
      */
      void onResult(BLEAdvertisedDevice advertisedDevice) {
        Serial.print("BLE Advertised Device found: ");
        Serial.println(advertisedDevice.toString().c_str());

        // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {

          //
          Serial.print("Found our device!  address: ");
          advertisedDevice.getScan()->stop();

          pServerAddress = new BLEAddress(advertisedDevice.getAddress());
          doConnect = true;

        } // Found our server
      } // onResult
  }; // MyAdvertisedDeviceCallbacks


  void setup() {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    Serial.println("Starting Arduino BLE Client application...");
    BLEDevice::init("");

    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 30 seconds.
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30);
  } // End of setup.


  const uint8_t notificationOff[] = {0x0, 0x0};
  const uint8_t notificationOn[] = {0x1, 0x0};
  bool onoff = true;
  void loop() {

    // If the flag "doConnect" is true then we have scanned for and found the desired
    // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
    // connected we set the connected flag to be true.
    if (doConnect == true) {
      if (connectToServer(*pServerAddress)) {
        Serial.println("We are now connected to the BLE Server.");
        connected = true;
      } else {
        Serial.println("We have failed to connect to the server; there is nothin more we will do.");
      }
      doConnect = false;
    }

    // If we are connected to a peer BLE Server, update the characteristic each time we are reached
    // with the current time since boot.
    if (connected) {
      if (onoff) {
        //Serial.println("Notifications turned on");
        pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
        pRemoteledCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
        pRemotetempCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
        pRemotehumCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      }
     /* else     {
        //Serial.println("Notifications turned off");
        pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOff, 2, true);
        pRemoteledCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOff, 2, true);
        pRemotetempCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOff, 2, true);
        pRemotehumCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOff, 2, true);
      }

      onoff = onoff ? 0 : 1;*/
    }

    delay(10000); // Delay a second between loops.
  } // End of loop
