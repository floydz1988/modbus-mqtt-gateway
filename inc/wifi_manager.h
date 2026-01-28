#pragma once
#include <WiFi.h>

class WiFiManager {
public:
  WiFiManager(const char* ssid, const char* password);

  void begin();
  void loop();

  bool connected() const;
  const char* ip() const;

private:
  const char* ssid;
  const char* password;

  unsigned long lastAttemptMs = 0;
  static constexpr unsigned long RECONNECT_INTERVAL_MS = 5000;
};

