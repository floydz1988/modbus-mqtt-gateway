#include "../inc/sdm120m.h"
#include "../inc/modbus_bus.h"
#include "../inc/logger.h"

SDM120M::SDM120M(uint8_t id, ModbusBus& b)
: slave(id), bus(b), voltageValue(0.0f) {}

const char* SDM120M::type() const {
  return "SDM120M";
}

void SDM120M::setRetries(uint8_t retries, uint16_t delayMs){
    maxRetries = retries;
    retryDelayMs = delayMs;
}

bool SDM120M::read() {
  for (uint8_t attempt = 1; attempt <= maxRetries; attempt++) {
    if (readOnce()) {
      return true;
    }

    Logger::debug(
      "SDM120M[%d] retry %d/%d",
      slave,
      attempt,
      maxRetries
    );

    delay(retryDelayMs);
  }

  Logger::error("SDM120M[%d] read failed after %d retries", slave, maxRetries);
  return false;
}

bool SDM120M::readOnce() {
  if (!bus.readInput(slave, 0x0000, 2)) {
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
