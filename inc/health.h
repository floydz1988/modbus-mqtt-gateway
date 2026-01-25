#pragma once
#include <stdint.h>

enum HealthState {
  HEALTH_OK,
  HEALTH_DEGRADED,
  HEALTH_FAILED
};

class HealthTracker {
public:
  HealthTracker();

  void onSuccess();
  void onFailure();

  HealthState state() const;
  const char* toString() const;

private:
  uint8_t consecutiveFailures;
  uint32_t lastSuccessMillis;
};
