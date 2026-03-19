/*
  em2m-esp32-modbus
  Author: Dhruv Kotian
  GitHub: https://github.com/dhruvkotian/em2m-esp32-modbus
  License: MIT
  Description: EM2M Modbus driver example for ESP32.
*/

#include <em2m.h>

// RS485 Pins Configuration
#define RXD2 16
#define TXD2 17
#define MAX485 4

ModbusMaster node;
EM2M meter(node, MAX485);

void setup() {
    Serial.begin(115200);

    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    node.begin(1, Serial2);

    Serial.println("EM2M Driver Example");
    Serial.print("Device: ");
    Serial.println(meter.deviceName());
}

void loop(){

    if (meter.update()) {

        Serial.print("Voltage: ");
        Serial.print(meter.voltage());
        Serial.println("V");

        Serial.print("Current: ");
        Serial.print(meter.current());
        Serial.println("A");

        Serial.print("Frequency: ");
        Serial.print(meter.frequency());
        Serial.println("Hz");

        Serial.print("Active Power: ");
        Serial.print(meter.activePower());
        Serial.println("kW");

        Serial.print("Power Factor: ");
        Serial.println(meter.powerFactor());

        Serial.println("------------------------");

    } else {
        Serial.println("Meter communication failed");
    }

    delay(2000);
}