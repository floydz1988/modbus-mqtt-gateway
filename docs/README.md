text
# Modbus-MQTT Gateway (Binghe ESP32 Dual TFT)

## Phase 1 ✅ Scanner
src/phase1_scanner.ino ─ Master polls SDM120CT Reg0-9
hardware/BOM.csv ─ €44 parts list

text

**Wiring**:
Binghe Master GPIO16→MAX485 DI, 17→RO, 4→RE/DE
MAX485 A→SDM120 A, B→B (+5V/GND)

text

**Arduino IDE**:
Board: ESP32 Dev Module
Lib: ModbusRTU + TFT_eSPI (ST7789 170x320)
Upload → TFT shows "SUCCESS Power:123W"

text

## Phase 2 ⏳ Slave + MQTT
src/phase2_slave.ino ─ SDM120 → MQTT broker
hardware/shield.sch ─ KiCad dual-Binghe PCB
docs/api.md ─ /power, /volt JSON endpoints

text

## Features
- Dual 1.96" TFT dashboards (master/slave)
- RS485 Modbus RTU ↔ MQTT Home Assistant
- OTA updates, WiFi AP config
- €44 BOM, 10cm protoboard

**Repo Live**: https://github.com/floydz1988/modbus-mqtt-gateway
**Discord**: DM floydz1988 for beta access

---
Binghe ESP32-WROOM-32 | Made Norderstedt 2026
