// Import libraries (BLEPeripheral depends on SPI)
#include "BLESerial.h"
#include <BLEPeripheral.h>
#include <SPI.h>
#define BLE_SERIAL_DEBUG 1

//custom boards may override default pin definitions with BLESerial(PIN_REQ, PIN_RDY, PIN_RST)
BLESerial bleSerial;

// forward received from Serial to BLESerial and vice versa
void forward() {
    if (bleSerial && Serial) {
        int byte;
        while ((byte = bleSerial.read()) > 0)
            Serial.write((char)byte);
        while ((byte = Serial.read()) > 0)
            bleSerial.write((char)byte);
    }
}

// echo all received data back
void loopback() {
    if (bleSerial) {
        int byte;
        while ((byte = bleSerial.read()) > 0) {
            Serial.write((char)byte);
            bleSerial.write(byte);
        }
    }
}

// periodically sent time stamps
void spam() {
    if (bleSerial) {
        bleSerial.print(millis());
        bleSerial.println(" tick-tacks!");
        delay(1000);
    }
}

void blePeripheralConnectHandler(BLECentral &central) {
    // central connected event handler
    Serial.print(F("Connected event, central: "));
    Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral &central) {
    // central disconnected event handler
    Serial.print(F("Disconnected event, central: "));
    Serial.println(central.address());
}

void setup() {
    // custom services and characteristics can be added as well
    Serial.begin(9600);
    bleSerial.setLocalName("Camera");
    bleSerial.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    bleSerial.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
    bleSerial.begin();
    Serial.println(F("Setup BLE serial"));
}

void loop() {
    bleSerial.poll();
    loopback();
}