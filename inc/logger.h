#pragma once
#include <stdarg.h>

enum LogLevel {
  LOG_NONE = 0,
  LOG_ERROR,
  LOG_INFO,
  LOG_DEBUG
};

class Logger {
public:
  static void begin(LogLevel level);
  static void error(const char* fmt, ...);
  static void info(const char* fmt, ...);
  static void debug(const char* fmt, ...);

private:
  static LogLevel currentLevel;
  static void log(LogLevel level, const char* tag, const char* fmt, va_list args);
};
