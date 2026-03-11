# EM2M ESP32 Modbus Driver

A lightweight ESP32 driver for communicating with the **Selec EM2M Energy Meter** using Modbus RTU over RS485.

This library provides a clean Hardware Abstraction Layer (HAL) style interface for reading electrical parameters such as voltage, current, power factor, and frequency.

## Features

- Modbus RTU communication
- RS485 direction control
- Bulk register reading
- Single parameter read
- Retry mechanism for communication reliability
- Clean API for embedded firmware

## Hardware Requirements

- ESP32
- RS485 transceiver module (MAX485 / equivalent)
- Selec EM2M Energy Meter

## Wiring Example

| ESP32 Pin | RS485 Module |
|-----------|--------------|
| TXD2 (17) | DI |
| RXD2 (16) | RO |
| GPIO4     | DE / RE |
| GND       | GND |

RS485 A/B lines connect to the EM2M meter RS485 terminals.

## Installation

1. Clone the repository:
git clone https://github.com/dhruvkotian/em2m-esp32-modbus.git

2. Copy the `src` folder into your Arduino project or install as a library.

## Example Usage

```cpp
#include <ModbusMaster.h>
#include "em2m.h"

#define RXD2 16
#define TXD2 17
#define TX_ENABLE_PIN 4

ModbusMaster node;
EM2M meter(node, TX_ENABLE_PIN);

void setup(){
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  node.begin(1, Serial2);

  Serial.println(meter.deviceName());
}

void loop(){
  if(meter.update()){
    Serial.print("Voltage: ");
    Serial.println(meter.get(VOLTAGE));

    Serial.print("Current: ");
    Serial.println(meter.get(CURRENT));

    Serial.print("Power Factor: ");
    Serial.println(meter.get(POWER_FACTOR));
  }

  delay(2000);
}

 ```
_Project inspired by [ModbusMaster](https://github.com/4-20ma/ModbusMaster)._

## Supported Parameters

- Voltage
- Current
- Active Power
- Reactive Power
- Apparent Power
- Power Factor
- Frequency
- Maximum Demand

## License

MIT License

## Author

Dhruv K
