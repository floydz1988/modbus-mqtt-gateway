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
  // Optional tuning
  void setRetries(uint8_t retries, uint16_t delayMs);

private:
  bool readOnce();

  uint8_t slave;
  ModbusBus& bus;
  float voltageValue;

  uint8_t maxRetries = 3;
  uint16_t retryDelayMs = 100;
};
