#include "BLESerial.h"
#include "crc32.h"
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

void sendPicture(const uint8_t *pic, const size_t len) {
    uint32_t code = 0;
    uint8_t buffer[1 + sizeof(len)];
    buffer[0] = 1;
    memcpy(&buffer[1], &len, sizeof(len));
    bleSerial.write(buffer, sizeof(buffer));
    bleSerial.imageWrite(pic, len);
}

void handleMessage() {
    if (bleSerial) {
        int byte;
        while ((byte = bleSerial.read()) > 0) {
            switch (byte) {
            case 0x01:
                sendPicture(PICTURE1, PICTURE1_LEN);
                break;
            case 'A':
                sendPicture(PICTURE1, PICTURE1_LEN);
                break;
            case 0x03:
                sendPicture(PICTURE1, PICTURE1_LEN);
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