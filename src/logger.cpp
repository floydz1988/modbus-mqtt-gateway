#include "../inc/logger.h"


#if defined(ESP32)
  #include <Arduino.h>
#else
  #include <cstdio>
#endif

LogLevel Logger::currentLevel = LOG_INFO;

void Logger::begin(LogLevel level) {
  currentLevel = level;
}

void Logger::log(LogLevel level, const char* tag, const char* fmt, va_list args) {
  if (level > currentLevel) return;

#if defined(ESP32)
  Serial.printf("[%s] ", tag);
  Serial.vprintf(fmt, args);
  Serial.println();
#else
  printf("[%s] ", tag);
  vprintf(fmt, args);
  printf("\n");
#endif
}

void Logger::error(const char* fmt, ...) {
  va_list args; va_start(args, fmt);
  log(LOG_ERROR, "ERROR", fmt, args);
  va_end(args);
}

void Logger::info(const char* fmt, ...) {
  va_list args; va_start(args, fmt);
  log(LOG_INFO, "INFO", fmt, args);
  va_end(args);
}

void Logger::debug(const char* fmt, ...) {
  va_list args; va_start(args, fmt);
  log(LOG_DEBUG, "DEBUG", fmt, args);
  va_end(args);
}

