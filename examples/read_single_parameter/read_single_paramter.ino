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

void loop(){
  if (meter.read(CURRENT)){
    Serial.print("Current: ");
    Serial.println(meter.get(CURRENT));
  } else{
    Serial.println("Read failed");
  }
  delay(1000);
}