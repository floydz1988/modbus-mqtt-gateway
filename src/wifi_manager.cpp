#include "../inc/wifi_manager.h"
#include "../inc/logger.h"
#include "../inc/config.h"

WiFiManager::WiFiManager(const char* s, const char* p)
: ssid(s), password(p) {}

void WiFiManager::begin() {
  Serial.println("[WIFI] Starting WiFi");
  Serial.print("[WIFI] SSID: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] Connected");
    Serial.print("[WIFI] IP: ");
    Serial.println(WiFi.localIP());
    Logger::info("WiFi connected, IP=%s", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n[WIFI] Connection timeout");
    Logger::error("WiFi connection failed");
  }
}

void WiFiManager::loop() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  unsigned long now = millis();
  if (now - lastAttemptMs < RECONNECT_INTERVAL_MS) {
    return;
  }

  lastAttemptMs = now;
  Logger::info("WiFi reconnecting...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);
}

bool WiFiManager::connected() const {
  return WiFi.status() == WL_CONNECTED;
}

const char* WiFiManager::ip() const {
  static String ipStr;
  ipStr = WiFi.localIP().toString();
  return ipStr.c_str();
}
