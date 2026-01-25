#include "../inc/logger.h"
#include "../inc/health.h"
#include <Arduino.h>

static const uint8_t MAX_CONSECUTIVE_FAILURES = 5;
static const uint32_t FAIL_TIMEOUT_MS = 10000;

HealthTracker::HealthTracker()
: consecutiveFailures(0),
  lastSuccessMillis(0) {}

void HealthTracker::onSuccess() {
  consecutiveFailures = 0;
  lastSuccessMillis = millis();
}

void HealthTracker::onFailure() {
  consecutiveFailures++;
}

HealthState HealthTracker::state() const {
  uint32_t now = millis();

  if (consecutiveFailures >= MAX_CONSECUTIVE_FAILURES ||
      (now - lastSuccessMillis) > FAIL_TIMEOUT_MS) {
    return HEALTH_FAILED;
  }

  if (consecutiveFailures > 0) {
    return HEALTH_DEGRADED;
  }

  return HEALTH_OK;
}

const char* HealthTracker::toString() const {
  switch (state()) {
    case HEALTH_OK:       return "OK";
    case HEALTH_DEGRADED: return "DEGRADED";
    case HEALTH_FAILED:   return "FAILED";
    default:              return "UNKNOWN";
  }
}
