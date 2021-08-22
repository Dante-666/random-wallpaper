#pragma once

#include <string>

using std::string;

void setDesktopBackground(const string &url);

class mac_error : public std::exception {
  string _errMsg;
  virtual const char *what() const noexcept override;

public:
  mac_error(const string &&errMsg) : _errMsg(errMsg){};
};
