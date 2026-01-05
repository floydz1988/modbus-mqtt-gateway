# Phase 1 Scanner for Binghe ESP32-WROOM-32 1.96" TFT (170x320 ST7789)
# Arduino IDE: 
# 1. Board: ESP32 Dev Module
# 2. Libraries: ModbusRTU (Emelianov), TFT_eSPI (Bodmer)
# 3. TFT_eSPI User_Setup.h: ST7789_DRIVER, TFT_WIDTH=170, TFT_HEIGHT=320

#include <ModbusRTU.h>
#include <HardwareSerial.h>
#include <TFT_eSPI.h>

#define RE_DE_PIN 4
#define SLAVE_ID 1
#define REG_START 0
#define REG_COUNT 10

TFT_eSPI tft = TFT_eSPI();
ModbusRTU mb;
bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) { return true; }

void setup() {
  Serial.begin(115200);
  
  // TFT Init (1.96" 170x320)
  tft.init();
  tft.setRotation(1);  // Landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Modbus Scanner", 10, 10);
  tft.setTextSize(1);
  tft.drawString("Connect RS485 A/B/GND", 10, 40);
  
  // RS485 UART2
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  mb.begin(&Serial2, RE_DE_PIN);
  mb.slaveId
