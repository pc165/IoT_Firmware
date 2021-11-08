// Import libraries (BLEPeripheral depends on SPI)
#include "BLESerial.h"
#include "pic.h"
#include <BLEPeripheral.h>
#include <SPI.h>
// #define PIN_LED1 2
//custom boards may override default pin definitions with BLESerial(PIN_REQ, PIN_RDY, PIN_RST)
BLESerial bleSerial;

void blePeripheralConnectHandler(BLECentral &central) {
    // central connected event handler
    Serial.print(F("Connected event, central: "));
    Serial.println(central.address());
    digitalWrite(PIN_LED1, HIGH);
}

void blePeripheralDisconnectHandler(BLECentral &central) {
    // central disconnected event handler
    Serial.print(F("Disconnected event, central: "));
    Serial.println(central.address());
    digitalWrite(PIN_LED1, LOW);
}

void setup() {
    // custom services and characteristics can be added as well
    Serial.begin(9600);
    bleSerial.setLocalName("Camera");
    bleSerial.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    bleSerial.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
    bleSerial.begin();
    pinMode(PIN_LED1, OUTPUT);
    digitalWrite(PIN_LED1, LOW);
}

void handleMessage() {
    if (bleSerial) {
        int byte;
        while ((byte = bleSerial.read()) > 0) {
            switch (byte) {
            case '1':
                bleSerial.write(pic, PIC_LEN);
                break;

            default:
                break;
            }
        }
    }
}
void loop() {
    bleSerial.poll();
    handleMessage();
}