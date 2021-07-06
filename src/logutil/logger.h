#pragma once

#include <chrono>
#include <ratio>
#include <iostream>
#include <iomanip>
#include <memory>
#include <type_traits>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::stringstream;

using std::chrono::current_zone;
using std::chrono::seconds;
using std::chrono::system_clock;
using std::chrono::zoned_time;

using std::unique_ptr;
using std::make_unique;

enum class LogLevel
{
  ALL = 0, INFO = 1, WARN = 2, FATAL = 3
};

class LoggerImpl
{
  virtual void _log(const string &message) = 0;
  unique_ptr<LoggerImpl> _next;
public:
  void log(const string &message);
};

class ConsoleLogger : public LoggerImpl
{
  virtual void _log(const string &message) override;
};

class Logger
{
  static unique_ptr<LoggerImpl> _impl;
  static LogLevel _level;
  static void log(const LogLevel &level, const string &message);
public:
  static void LogDebug(const string& message);
  static void LogInfo(const string& message);
  static void LogWarn(const string& message);
  static void LogError(const string& message);
};

//TODO: FileLogger later