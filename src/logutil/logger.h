#pragma once

#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ratio>
#include <regex>
#include <sstream>
#include <type_traits>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::stringstream;

#if defined _WIN64 || defined _WIN32
using std::chrono::current_zone;
using std::chrono::zoned_time;
#endif
using std::chrono::seconds;
using std::chrono::system_clock;

using std::make_unique;
using std::unique_ptr;

using std::regex;
using std::regex_search;
using std::smatch;

#undef DEBUG
#undef ERROR

enum class LogLevel { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3 };

class LoggerImpl {
  virtual void _log(const string &message) = 0;
  unique_ptr<LoggerImpl> _next;

public:
  virtual ~LoggerImpl() = default;
  void log(const string &message);
};

class ConsoleLogger : public LoggerImpl {
  virtual void _log(const string &message) override;

public:
  virtual ~ConsoleLogger() override = default;
};

// TODO: FileLogger later

class BaseLogger {
  static LogLevel _level;
  static void log(const LogLevel &level, const string &message,
                  const string &function);
  static const unique_ptr<LoggerImpl> &getInstance();

public:
  static void setLogLevel(const LogLevel &level);
  static void logDebug(const string &message, const string &function);
  static void logInfo(const string &message, const string &function);
  static void logWarn(const string &message, const string &function);
  static void logError(const string &message, const string &function);
};

namespace Logger {
using BaseLogger = BaseLogger;
#define SetLogLevel(LVL) BaseLogger::setLogLevel(LVL)
#if defined __GNUC__ || __APPLE_CC__
#define LogDebug(MSG) BaseLogger::logDebug(MSG, __PRETTY_FUNCTION__)
#define LogInfo(MSG) BaseLogger::logInfo(MSG, __PRETTY_FUNCTION__)
#define LogWarn(MSG) BaseLogger::logWarn(MSG, __PRETTY_FUNCTION__)
#define LogError(MSG) BaseLogger::logError(MSG, __PRETTY_FUNCTION__)
#elif defined _WIN64 || _WIN32
#define LogDebug(MSG) BaseLogger::logDebug(MSG, __FUNCSIG__)
#define LogInfo(MSG) BaseLogger::logInfo(MSG, __FUNCSIG__)
#define LogWarn(MSG) BaseLogger::logWarn(MSG, __FUNCSIG__)
#define LogError(MSG) BaseLogger::logError(MSG, __FUNCSIG__)
#endif
} // namespace Logger
