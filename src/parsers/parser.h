#pragma once
#include <string>
#include <sstream>
#include <vector>

#include "logutil/logger.h"

using std::stringstream;
using std::string;
using std::vector;

class Parser {
public:
  virtual ~Parser() {};
  virtual vector<string> parseStream(const stringstream &stream) = 0;
};

