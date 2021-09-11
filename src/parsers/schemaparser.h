#pragma once
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "logutil/logger.h"

using std::cmatch;
using std::regex;
using std::regex_search;

using std::string;
using std::stringstream;

using std::vector;

class SchemaParser {
public:
  virtual ~SchemaParser(){};
  virtual vector<string> parseStream(const stringstream &stream) = 0;
};
