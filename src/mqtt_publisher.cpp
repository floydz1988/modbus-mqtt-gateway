#include "../inc/mqtt_publisher.h"
#include <Arduino.h>
#include "../inc/logger.h"

MqttPublisher::MqttPublisher(PubSubClient& client,
                             const char* gatewayId)
  : mqtt(client), gateway(gatewayId) {}

void MqttPublisher::publishMetric(
  const char* deviceType,
  uint8_t deviceId,
  const char* metric,
  float value,
  const char* unit,
  const char* health
) {
  char topic[128];
  char payload[128];

  snprintf(topic, sizeof(topic),
    "gateway/%s/modbus/%s/%d/%s",
    gateway, deviceType, deviceId, metric);

  snprintf(payload, sizeof(payload),
    "{\"value\":%.2f,\"unit\":\"%s\",\"health\":\"%s\"}",
    value, unit, health);

  //mqtt.publish(topic, payload, true);
  bool ok = mqtt.publish(topic, payload, true);

  if (!ok) {
    Logger::error("MQTT publish failed: %s", topic);
  } 
  else {
    Logger::debug("MQTT published: %s", topic);
  }
}

