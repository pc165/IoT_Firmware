#include "BLESerial.h"
#include "pic.h"
#include "pic2.h"
#include "pic3.h"
#include <BLEPeripheral.h>
#include <SPI.h>
#define PIN_LED1 2
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
            case 0x01:
                bleSerial.write(PICTURE1, PICTURE1_LEN);
                break;
            case 0x02:
                bleSerial.write(PICTURE2, PICTURE2_LEN);
                break;
            case 0x03:
                bleSerial.write(PICTURE3, PICTURE3_LEN);
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