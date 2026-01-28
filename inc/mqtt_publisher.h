#pragma once
#include <PubSubClient.h>

class MqttPublisher {
public:
  MqttPublisher(PubSubClient& client,
                const char* gatewayId);

  void publishMetric(const char* deviceType,
                     uint8_t deviceId,
                     const char* metric,
                     float value,
                     const char* unit,
                     const char* health);

private:
  PubSubClient& mqtt;
  const char* gateway;
};

