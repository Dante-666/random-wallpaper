#pragma once

#include "parser.h"
#include "tinyxml.h"

class XMLParser : public Parser {
public:
  virtual ~XMLParser() override;
  virtual vector<string> parseStream(const stringstream &stream) override;
};
