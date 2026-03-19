#include "em2m.h"
#include <string.h>

/* ----------------------------------------------------
  Static member initialization
---------------------------------------------------- */

uint8_t EM2M::_txEnablePin = 0;

/*  ----------------------------------------------------
  Register Table Definition
---------------------------------------------------- */
EM2MRegister em2mRegisters[EM2M_PARAM_COUNT] = {
  {"total_active_energy",    0x00, 0},
  {"import_active_energy",   0x02, 0},
  {"export_active_energy",   0x04, 0},

  {"total_reactive_energy",  0x06, 0},
  {"import_reactive_energy", 0x08, 0},
  {"export_reactive_energy", 0x0A, 0},

  {"apparent_energy",        0x0C, 0},

  {"active_power",           0x0E, 0},
  {"reactive_power",         0x10, 0},
  {"apparent_power",         0x12, 0},

  {"voltage",                0x14, 0},
  {"current",                0x16, 0},
  {"power_factor",           0x18, 0},
  {"frequency",              0x1A, 0},
  
  {"max_demand_active",      0x1C, 0},
  {"max_demand_reactive",    0X1E, 0},
  {"max_demand_apparent",    0X20, 0},
};

/* ----------------------------------------------------
  Constructor
----------------------------------------------------*/
EM2M::EM2M(ModbusMaster& node, uint8_t txEnablePin)
: _node(node){
  _txPin = txEnablePin;
  _txEnablePin = txEnablePin;

  pinMode(_txPin, OUTPUT);
  digitalWrite(_txPin, LOW);

  _node.preTransmission(preTransmission);
  _node.postTransmission(postTransmission);
}

/* ----------------------------------------------------
  RS485 Direction Control
---------------------------------------------------- */

void EM2M::preTransmission(){
  digitalWrite(_txEnablePin, HIGH);
}
void EM2M::postTransmission(){
  digitalWrite(_txEnablePin, LOW);
}

/* ----------------------------------------------------
  Modbus Float conversion
---------------------------------------------------- */

float EM2M::modbusToFloat(uint16_t low, uint16_t high){
  uint32_t combined = ((uint32_t) high << 16) | low;

  float value;
  memcpy(&value, &combined, sizeof(value));
  return value;
}

/* ----------------------------------------------------
  Read One Param
---------------------------------------------------- */
bool EM2M::read(EM2MParam param){
  uint8_t result;
  uint16_t addr = em2mRegisters[param].address;
  for(int attempt = 0; attempt < EM2M_MAX_RETRIES; attempt++){
    result = _node.readInputRegisters(addr, 2);
    if(result == _node.ku8MBSuccess){
      uint16_t low  = _node.getResponseBuffer(0);
      uint16_t high = _node.getResponseBuffer(1);
      em2mRegisters[param].value = modbusToFloat(low, high);
      return true;
      }
    delay(EM2M_RETRY_DELAY);
  }
  return false;
}

/* ----------------------------------------------------
  Read All Param
---------------------------------------------------- */
bool EM2M::update() {
    uint8_t result;

    for (int attempt = 0; attempt < EM2M_MAX_RETRIES; attempt++) {
        result = _node.readInputRegisters(0x00, 34);

        if (result == _node.ku8MBSuccess) { // decode registers
            for (int i = 0; i < EM2M_PARAM_COUNT; i++) {
                uint16_t low  = _node.getResponseBuffer(i * 2);
                uint16_t high = _node.getResponseBuffer(i * 2 + 1);
                em2mRegisters[i].value = modbusToFloat(low, high);
            }
            return true;
        }

        delay(EM2M_RETRY_DELAY);
    }

    return false;
}


/* ----------------------------------------------------
  Access Functions
---------------------------------------------------- */
float EM2M::get(uint8_t index){
  if(index >= EM2M_PARAM_COUNT)
    return 0;
  return em2mRegisters[index].value;
}

const char* EM2M::getName(uint8_t index) {
  if(index >= EM2M_PARAM_COUNT)
    return "";
  return em2mRegisters[index].name;
}

/* ----------------------------------------------------
  Debug Print
---------------------------------------------------- */
void EM2M::printAll(Stream &serial){
  for(uint8_t i = 0; i < EM2M_PARAM_COUNT; i++){
    serial.print(em2mRegisters[i].name);
    serial.print(" : ");
    serial.println(em2mRegisters[i].value);
  }
}

const char* EM2M::deviceName()
{
    return "EM2M Energy Meter";
}

/* ----------------------------------------------------
  Clean get Functions (user - friendly api)
---------------------------------------------------- */

float EM2M::voltage(){
  return em2mRegisters[VOLTAGE].value;
}

float EM2M::current(){
  return em2mRegisters[CURRENT].value;
}

float EM2M::frequency(){
  return em2mRegisters[FREQUENCY].value;
}

float EM2M::activePower(){
  return em2mRegisters[ACTIVE_POWER].value;
}

float EM2M::apparentPower(){
  return em2mRegisters[APPARENT_POWER].value;
}

float EM2M::reactivePower(){
  return em2mRegisters[REACTIVE_POWER].value;
}

float EM2M::powerFactor(){
  return em2mRegisters[POWER_FACTOR].value;
}

float EM2M::apparentEnergy(){
  return em2mRegisters[APPARENT_ENERGY].value;
}

float EM2M::totalActiveEnergy(){
  return em2mRegisters[TOTAL_ACTIVE_ENERGY].value;
}

float EM2M::importActiveEnergy(){
  return em2mRegisters[IMPORT_ACTIVE_ENERGY].value;
}

float EM2M::exportActiveEnergy(){
  return em2mRegisters[EXPORT_ACTIVE_ENERGY].value;
}

float EM2M::maxDemandActive(){
  return em2mRegisters[MAX_DEMAND_ACTIVE].value;
}

float EM2M::maxDemandReactive(){
  return em2mRegisters[MAX_DEMAND_REACTIVE].value;
}

float EM2M::maxDemandApparent(){
  return em2mRegisters[MAX_DEMAND_APPARENT].value;
}
