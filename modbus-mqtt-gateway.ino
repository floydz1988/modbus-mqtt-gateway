#include "soc/rtc_cntl_reg.h"
#include "inc/logger.h"
#include "inc/health.h"
#include "inc/sdm120m.h"
#include "inc/modbus_bus.h"


/* =========================
   Pin configuration
   ========================= */
#define MAX485_DE_RE 4
#define MODBUS_RX    16
#define MODBUS_TX    17


ModbusBus bus;
SDM120M meter1(1, bus);
HealthTracker health;



/* =========================
   Setup
   ========================= */
void setup() {
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);
  Logger::begin(LOG_DEBUG);
  Logger::info("Gateway starting");
  delay(1000);

  Serial.println("SDM120M Modbus RTU test starting...");

  pinMode(MAX485_DE_RE, OUTPUT);
  digitalWrite(MAX485_DE_RE, LOW);

  Serial2.begin(
    2400,              // stable baud rate
    SERIAL_8N1,
    MODBUS_RX,
    MODBUS_TX
  );
  
  bus.begin(Serial2,2400,MAX485_DE_RE);
  Serial.println("Setup completed successfully.");
}

/* =========================
   Main loop
   ========================= */
void loop() {
  
  /*SDM120M meter - loop it for more meters/slaves */
  if (meter1.read()) {
    health.onSuccess();

    Logger::info(
      "Voltage: %.2f V | Health: %s",
      meter1.voltage(),
      health.toString()
    );

    // publishVoltage(meter1.voltage());
  } else {
    health.onFailure();
    Logger::error("Read failed | Health: %s", health.toString());
  }

  delay(1500);
}
