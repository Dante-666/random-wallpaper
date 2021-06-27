#pragma once

#include <ctime>
#include <iostream>
#include <type_traits>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

#ifdef DEBUG
template <typename> struct write_output : std::true_type {};
#else
template <typename> struct write_output : std::false_type {};
#endif

template <typename T = void>
typename std::enable_if<write_output<T>::value, void>::type
LogOutput(const string &message) {
  time_t now;
  struct tm *timeinfo;
  time(&now);
  timeinfo = localtime(&now);
	string stime(asctime(timeinfo));

  cout << stime.substr(0, stime.length() - 1)  << " : " << message << endl;
}
template <typename T = void>
typename std::enable_if<!write_output<T>::value, void>::type
LogOutput(const string &message) {}

template <typename T = void> void LogError(const string &message) {
  time_t now;
  struct tm *timeinfo;
  time(&now);
  timeinfo = localtime(&now);
	string stime(asctime(timeinfo));

  cerr << stime.substr(0, stime.length() - 1)  << " : " << message << endl;
}
