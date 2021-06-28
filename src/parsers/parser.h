#pragma once
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

class Parser {
public:
  virtual ~Parser() = 0;
  virtual vector<string> parseStream(const ostream &stream) = 0;
};

