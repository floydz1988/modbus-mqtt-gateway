#include "soc/rtc_cntl_reg.h"
#include "inc/modbus_bus.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "inc/config.h"
#include "inc/logger.h"
#include "inc/health.h"
#include "inc/wifi_manager.h"
#include "inc/mqtt_publisher.h"
#include "inc/sdm120m.h"




/* =========================
   Pin configuration
   ========================= */
#define MAX485_DE_RE 4
#define MODBUS_RX    16
#define MODBUS_TX    17




WiFiClient espClient;
PubSubClient mqttClient(espClient);
WiFiManager wifi(WIFI_SSID, WIFI_PASS);
MqttPublisher publisher(mqttClient, DEVICE_ID);
ModbusBus bus;
SDM120M meter1(1, bus);
HealthTracker health;





/* =========================
   Setup
   ========================= */
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);
  Logger::begin(LOG_DEBUG);
  Logger::info("Gateway starting");
  delay(1000);

  wifi.begin();
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);

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
  
  /*Wifi loop */
  wifi.loop();
  if (wifi.connected()) {
    if (!mqttClient.connected()) {
      Logger::info("Connecting to MQTT...");
      if (mqttClient.connect(DEVICE_ID)) {
        Logger::info("MQTT connected");
      } 
      else {
        Logger::error("MQTT connect failed, rc=%d", mqttClient.state());
      }
    }
    mqttClient.loop();
  }
  
  Logger::debug("WiFi=%s MQTT=%s",
  wifi.connected() ? "OK" : "DOWN",
  mqttClient.connected() ? "OK" : "DOWN");


  /*SDM120M meter - loop it for more meters/slaves */
  if (meter1.read()) {
    health.onSuccess();

    Logger::info(
      "Voltage: %.2f V | Health: %s",
      meter1.voltage(),
      health.toString() );

      /* Publish MQTT onSuccess */
      if (wifi.connected() && mqttClient.connected()) {
        publisher.publishMetric("sdm120m",
                              meter1.id(),
                              "voltage",
                              meter1.voltage(),
                              "V",
                              health.toString());
        }


    // publishVoltage(meter1.voltage());
  } 
  else {
    health.onFailure();
    Logger::error("Read failed | Health: %s", health.toString());
  }

  delay(1500);
}
