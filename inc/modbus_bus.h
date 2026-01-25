#pragma once
#include <ModbusMaster.h>
#include <Arduino.h>

class ModbusBus {
public:
  void begin(HardwareSerial& serial, uint32_t baud, uint8_t derePin);
  bool readInput(uint8_t slave, uint16_t reg, uint16_t count);
  ModbusMaster& node();

private:
  ModbusMaster mb;
  uint8_t derePin;

  static void preTransmission();
  static void postTransmission();
  static ModbusBus* instance;
};
