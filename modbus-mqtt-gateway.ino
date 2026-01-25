#include <ModbusMaster.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "soc/rtc_cntl_reg.h"
#include "inc/logger.h"
#include "inc/health.h"
#include "inc/sdm120m.h"


/* =========================
   Pin configuration
   ========================= */
#define MAX485_DE_RE 4
#define MODBUS_RX    16
#define MODBUS_TX    17

/* =========================
   Modbus
   ========================= */
ModbusMaster node;

/* =========================
   Retry & health configuration
   ========================= */
const uint8_t MAX_RETRIES = 3;
const uint8_t MAX_CONSECUTIVE_FAILURES = 5;
const uint32_t HEALTH_FAIL_TIMEOUT_MS = 10000;

/* =========================
   Counters & state
   ========================= */
uint32_t successCount = 0;
uint32_t crcErrorCount = 0;
uint32_t timeoutCount = 0;

uint8_t consecutiveFailures = 0;
uint32_t lastSuccessMillis = 0;

uint8_t recentErrors = 0;
uint8_t recentSuccesses = 0;

const uint8_t HEALTH_WINDOW = 5;

/* =========================
   Health state
   ========================= */
enum HealthState {
  HEALTH_OK,
  HEALTH_DEGRADED,
  HEALTH_FAILED
};

HealthState health = HEALTH_FAILED;

/* Wifi module SSID and pwd config */
const char* WIFI_SSID = "WILHELM.TEL-UWEV51SDIW";
const char* WIFI_PASS = "85725677939890880558";

/*MQTT broker IP and port setting */
const char* MQTT_BROKER = "192.168.178.10";   // or test.mosquitto.org
const int   MQTT_PORT   = 1883;

WiFiClient espClient;
PubSubClient mqttClient(espClient);


/* =========================
   RS485 direction control
   ========================= */
void preTransmission() {
  digitalWrite(MAX485_DE_RE, HIGH);
}

void postTransmission() {
  digitalWrite(MAX485_DE_RE, LOW);
}

/* =========================
   Health evaluation
   ========================= */
void updateHealth() {
  uint32_t now = millis();

  if (consecutiveFailures >= MAX_CONSECUTIVE_FAILURES ||
      (now - lastSuccessMillis) > HEALTH_FAIL_TIMEOUT_MS) {
    health = HEALTH_FAILED;
  }
  else if (recentErrors > 0) {
    health = HEALTH_DEGRADED;
  }
  else {
    health = HEALTH_OK;
  }
}


const char* healthToString() {
  switch (health) {
    case HEALTH_OK:       return "OK";
    case HEALTH_DEGRADED: return "DEGRADED";
    case HEALTH_FAILED:   return "FAILED";
    default:              return "UNKNOWN";
  }
}

void connectWiFi() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println();
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    mqttClient.connect("esp32-gateway-001");
    delay(1000);
  }
}

/*MQTT - publishing valid data */
void publishVoltage(float voltage) {
  char payload[128];

  snprintf(payload, sizeof(payload),
    "{\"voltage\":%.2f,\"health\":\"%s\",\"ts\":%lu}",
    voltage,
    healthToString(),
    time(nullptr)
  );

  mqttClient.publish(
    "gateway/esp32-001/devices/sdm120m/metrics",
    payload
  );
}



/* =========================
   Setup
   ========================= */
void setup() {
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
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

  node.begin(1, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  lastSuccessMillis = millis();
  
  /*Wifi connection setup*/
  
#if 0
  connectWiFi();
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  connectMQTT();
#endif

  Serial.println("Setup completed successfully.");
}

/* =========================
   Main loop
   ========================= */
void loop() {
  uint8_t result;
  float voltage;
  bool success = false;

  for (uint8_t attempt = 1; attempt <= MAX_RETRIES; attempt++) {

    result = node.readInputRegisters(0x0000, 2);

    if (result == node.ku8MBSuccess) {
      uint32_t raw =
        ((uint32_t)node.getResponseBuffer(0) << 16) |
         (uint32_t)node.getResponseBuffer(1);

      memcpy(&voltage, &raw, sizeof(voltage));

      successCount++;
      consecutiveFailures = 0;
      lastSuccessMillis = millis();
      success = true;
      recentSuccesses++;
      recentErrors = 0;   // forgive past errors

      /* publishing MQTT only data is valid*/
#if 0
      if (!mqttClient.connected()) {
        connectMQTT();
        }
        mqttClient.loop();
#endif
      break;
    }

    if (result == node.ku8MBInvalidCRC) {
      crcErrorCount++;
    }
    else if (result == node.ku8MBResponseTimedOut) {
      timeoutCount++;
    }

    delay(100);
  }

  if (!success) {
    consecutiveFailures++;
    recentErrors++;
    recentSuccesses = 0;
  }

  updateHealth();

  /* =========================
     Output
     ========================= */
  if (success) {
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.print(" V | Health: ");
    Serial.println(healthToString());
  } else {
    Serial.print("Read failed | Health: ");
    Serial.print(healthToString());
    Serial.print(" | CRC=");
    Serial.print(crcErrorCount);
    Serial.print(" TO=");
    Serial.println(timeoutCount);
  }

  delay(1500);
}
