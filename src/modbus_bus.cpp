#include "../inc/modbus_bus.h"

ModbusBus* ModbusBus::instance = nullptr;

void ModbusBus::begin(HardwareSerial& serial, uint32_t baud, uint8_t pin) {
  derePin = pin;
  pinMode(derePin, OUTPUT);
  digitalWrite(derePin, LOW);

  instance = this;

  serial.begin(baud);
}

void ModbusBus::preTransmission() {
  digitalWrite(instance->derePin, HIGH);
}

void ModbusBus::postTransmission() {
  digitalWrite(instance->derePin, LOW);
}

bool ModbusBus::readInput(uint8_t slave, uint16_t reg, uint16_t count) {
  mb.begin(slave, Serial2);
  mb.preTransmission(preTransmission);
  mb.postTransmission(postTransmission);

  return mb.readInputRegisters(reg, count) == mb.ku8MBSuccess;
}

ModbusMaster& ModbusBus::node() {
  return mb;
}
