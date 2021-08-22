#pragma once

#include <string>

using std::string;

bool setDesktopBackground(const string& url, const char* tmpDir, const bool& isWebURL = true);

class url_error : std::exception {
  virtual const char *what() const noexcept override;
};
