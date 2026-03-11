#include <ModbusMaster.h>
#include "em2m.h"

// RS485 Pins Configuration
#define RXD2 16
#define TXD2 17
#define TX_ENABLE_PIN 4

// Modbus Master and EM2M Meter
ModbusMaster node;
EM2M meter(node, TX_ENABLE_PIN);

void setup() {
    Serial.begin(115200);
    
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    node.begin(1, Serial2);

    Serial.println("Driver setup complete...");
    Serial.print("Device: ");
    Serial.println(meter.deviceName());
}

void loop() {
    Serial.println("Updating meter values...");

    if (meter.update()) {
        // Collecting meter values
        float voltage = meter.get(VOLTAGE);
        float current = meter.get(CURRENT);
        float powerFactor = meter.get(POWER_FACTOR);
        float frequency = meter.get(FREQUENCY);
        float activePower = meter.get(ACTIVE_POWER);

        // Displaying the readings
        Serial.print("Voltage: ");
        Serial.print(voltage);
        Serial.println(" V");

        Serial.print("Current: ");
        Serial.print(current);
        Serial.println(" A");

        Serial.print("Power Factor: ");
        Serial.println(powerFactor);

        Serial.print("Frequency: ");
        Serial.print(frequency);
        Serial.println(" Hz");

        Serial.print("Active Power: ");
        Serial.print(activePower);
        Serial.println(" W");
    } else {
        Serial.println("Meter read failed (retry logic executed)");
    }

    Serial.println("---------------------------");
    delay(2000);  // Delay between readings
}
