#include "../inc/sdm120m.h"
#include "../inc/modbus_bus.h"
#include "../inc/logger.h"

SDM120M::SDM120M(uint8_t id, ModbusBus& b)
: slave(id), bus(b), voltageValue(0) {}

const char* SDM120M::type() const {
  return "SDM120M";
}

bool SDM120M::read() {
  if (!bus.readInput(slave, 0x0000, 2)) {
    Logger::error("SDM120M[%d] read failed", slave);
    return false;
  }

  auto& n = bus.node();
  uint32_t raw =
    ((uint32_t)n.getResponseBuffer(0) << 16) |
     (uint32_t)n.getResponseBuffer(1);

  memcpy(&voltageValue, &raw, sizeof(voltageValue));
  return true;
}

float SDM120M::voltage() const {
  return voltageValue;
}
