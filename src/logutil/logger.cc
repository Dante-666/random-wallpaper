#include "logger.h"

unique_ptr<LoggerImpl> Logger::_impl = make_unique<ConsoleLogger>();
#ifdef NDEBUG
LogLevel Logger::_level = LogLevel::WARN
#else
LogLevel Logger::_level = LogLevel::ALL;
#endif

ostream &operator<<(ostream &stream, const LogLevel &level)
{
  switch (level)
  {
  case LogLevel::ALL:
    stream << " [DEBUG] ";
    break;
  case LogLevel::INFO:
    stream << " [INFO] ";
    break;
  case LogLevel::WARN:
    stream << " [WARN] ";
    break;
  case LogLevel::FATAL:
    stream << " [FATAL] ";
    break;
  }
  return stream;
}

void Logger::log(const LogLevel &level, const string &message) {
  if (level >= _level)
  {
    stringstream _message;
    _message << zoned_time{current_zone(), floor<seconds>(system_clock::now())} << level << ": "<< message;
    _impl->log(_message.str());
  }
}

void Logger::LogDebug(const string& message) {
  Logger::log(LogLevel::ALL, message);
}
void Logger::LogInfo(const string& message) {
  Logger::log(LogLevel::INFO, message);
}
void Logger::LogWarn(const string& message) {
  Logger::log(LogLevel::WARN, message);
}
void Logger::LogError(const string& message) {
  Logger::log(LogLevel::FATAL, message);
}

void LoggerImpl::log(const string &message) {
  this->_log(message);
  if (this->_next) {
    _next->log(message);
  }
}

void ConsoleLogger::_log(const string &message)
{
  cout << message << endl;
}