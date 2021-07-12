#include "logger.h"

unique_ptr<LoggerImpl> Logger::_impl = make_unique<ConsoleLogger>();
#ifdef NDEBUG
LogLevel Logger::_level = LogLevel::WARN;
#else
LogLevel Logger::_level = LogLevel::DEBUG;
#endif

ostream &operator<<(ostream &stream, const LogLevel &level) {
  switch (level) {
  case LogLevel::DEBUG:
    stream << " [DEBUG] ";
    break;
  case LogLevel::INFO:
    stream << " [INFO] ";
    break;
  case LogLevel::WARN:
    stream << " [WARN] ";
    break;
  case LogLevel::ERROR:
    stream << " [ERROR] ";
    break;
  }
  return stream;
}

void Logger::log(const LogLevel &level, const string &message) {
  if (level >= _level) {
    stringstream _message;
		//TODO: Disable C++20 for linux now since zoned_time isn't available
#ifdef __linux__
		auto time = system_clock::to_time_t(system_clock::now());
		string str_time(std::ctime(&time));
    _message << str_time.substr(0, str_time.length() - 1)
#else
    _message << zoned_time{current_zone(), floor<seconds>(system_clock::now())}
#endif
             << level << ": " << message;
    _impl->log(_message.str());
  }
}

void Logger::LogDebug(const string &message) {
  s=__PRETTY_FUNCTION__;
  Logger::log(LogLevel::DEBUG, message);
}
void Logger::LogInfo(const string &message) {
  s=__PRETTY_FUNCTION__;
  Logger::log(LogLevel::INFO, message);
}
void Logger::LogWarn(const string &message) {
  s=__PRETTY_FUNCTION__;
  Logger::log(LogLevel::WARN, message);
}
void Logger::LogError(const string &message) {
  s=__PRETTY_FUNCTION__;
  Logger::log(LogLevel::ERROR, message);
}

void LoggerImpl::log(const string &message) {
  this->_log(message);
  if (this->_next) {
    _next->log(message);
  }
}

void ConsoleLogger::_log(const string &message) { cout << message << endl; }
