#include "logger.h"

unique_ptr<LoggerImpl> BaseLogger::_impl = make_unique<ConsoleLogger>();
#ifdef NDEBUG
LogLevel BaseLogger::_level = LogLevel::WARN;
#else
LogLevel BaseLogger::_level = LogLevel::DEBUG;
#endif

regex BaseLogger::re("\\s&?((\\w+::)?(\\w+))\\(.*\\)$");

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

void BaseLogger::log(const LogLevel &level, const string &message,
                     const string &function) {
  if (level >= _level) {
    stringstream _message;
    // TODO: Disable C++20 for linux now since build machines 
    // doesn't have same compile flag
#if defined __linux__ || __APPLE__
    auto time = system_clock::to_time_t(system_clock::now());
    string str_time(std::ctime(&time));
    _message << str_time.substr(0, str_time.length() - 1)
#elif defined _WIN64 || _WIN32
    _message << zoned_time {
      current_zone(), floor<seconds>(system_clock::now())
    }
#endif
             << level << ": ";
    smatch ma;
    if (regex_search(function, ma, re)) {
      assert(ma.size() > 1);
      _message << ma[1] << " -> ";
    }
    _message << message;
    _impl->log(_message.str());
  }
}

void BaseLogger::setLogLevel(const LogLevel &level) { _level = level; }
void BaseLogger::logDebug(const string &message, const string &function) {
  BaseLogger::log(LogLevel::DEBUG, message, function);
}
void BaseLogger::logInfo(const string &message, const string &function) {
  BaseLogger::log(LogLevel::INFO, message, function);
}
void BaseLogger::logWarn(const string &message, const string &function) {
  BaseLogger::log(LogLevel::WARN, message, function);
}
void BaseLogger::logError(const string &message, const string &function) {
  BaseLogger::log(LogLevel::ERROR, message, function);
}

void LoggerImpl::log(const string &message) {
  this->_log(message);
  if (this->_next) {
    _next->log(message);
  }
}

void ConsoleLogger::_log(const string &message) { cout << message << endl; }
