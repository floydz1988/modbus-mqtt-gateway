#pragma once
#include <stdint.h>

class ModbusDevice {
public:
  virtual ~ModbusDevice() {}
  virtual const char* type() const = 0;
  virtual bool read() = 0;
};
