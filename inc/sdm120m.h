#pragma once
#include <stdint.h>
#include "modbus_device.h"

class ModbusBus;   // forward declaration

class SDM120M : public ModbusDevice {
public:
  SDM120M(uint8_t slaveId, ModbusBus& bus);

  const char* type() const override;
  bool read() override;
  float voltage() const;

private:
  uint8_t slave;
  ModbusBus& bus;
  float voltageValue;
};
