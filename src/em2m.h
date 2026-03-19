#ifndef EM2M_H
#define EM2M_H

#include <Arduino.h>
#include <ModbusMaster.h>
/*
*----------------------------------------------------
* EM2M Modbus Driver
* Device: SELEC EM2M-1P-C-100A ENERGY METER
* Protcol: Modbus RTU 
*
* This driver provides structured access 
* to all EM2M device parameters using a
* register map
*
* Author: dhruvkotian
*----------------------------------------------------
*/

/* ----------------------------------------------------
  Configuration Param
---------------------------------------------------- */ 
#define EM2M_MAX_RETRIES 3
#define EM2M_RETRY_DELAY 50   // milliseconds


/* ----------------------------------------------------
  Each entry corresponds to a modbus register pair
---------------------------------------------------- */
enum EM2MParam {
  TOTAL_ACTIVE_ENERGY,
  IMPORT_ACTIVE_ENERGY,
  EXPORT_ACTIVE_ENERGY,

  TOTAL_REACTIVE_ENERGY,
  IMPORT_REACTIVE_ENERGY,
  EXPORT_REACTIVE_ENERGY,

  APPARENT_ENERGY,

  ACTIVE_POWER,
  REACTIVE_POWER,
  APPARENT_POWER,

  VOLTAGE,
  CURRENT,
  POWER_FACTOR,
  FREQUENCY,

  MAX_DEMAND_ACTIVE,
  MAX_DEMAND_REACTIVE,
  MAX_DEMAND_APPARENT,

  EM2M_PARAM_COUNT
};

/* ----------------------------------------------------
  Register Map Structure
*---------------------------------------------------- */

struct EM2MRegister {
  const char* name;   // parameter name
  uint16_t address;  // modbus register address
  float value;      // decode float value
};

/* Global register table (defined in .cpp) */
extern EM2MRegister em2mRegisters[EM2M_PARAM_COUNT];

/* ----------------------------------------------------
  EM2M Driver Class
---------------------------------------------------- */

class EM2M {
  public:
  /*  Constructor  */
  EM2M(ModbusMaster& node, uint8_t txEnablePin);

  /* -----------------------------
    Reading Functions
  ----------------------------- */

  bool read(EM2MParam param); // read one parameter
  bool update();                  // read entire register table

  /* -----------------------------
    Data Access
  ----------------------------- */
  float get(uint8_t index);            // get value by index
  const char* getName(uint8_t index); // get parameter name

  /* -----------------------------
  Clean getter (API)
  ----------------------------- */
  float voltage();
  float current();
  float frequency();
  float powerFactor();

  float activePower();
  float reactivePower();
  float apparentPower();

  float apparentEnergy();
  float totalActiveEnergy();
  float importActiveEnergy();
  float exportActiveEnergy();

  float maxDemandActive();


  /* -----------------------------
    Debug Utilities
  ----------------------------- */ 
  void printAll(Stream &serial);
  
  const char* deviceName();

  private:
  /*  modbus instance reference  */
  ModbusMaster& _node;

  /*  RS485 direction control  */
  uint8_t _txPin;
  static uint8_t _txEnablePin;

  static void preTransmission();
  static void postTransmission();

  /*  utility conversion  */
  float  modbusToFloat(uint16_t low, uint16_t high);

};

#endif