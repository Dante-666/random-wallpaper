#pragma once

#include "parser.h"
#include "tinyxml.h"

class XMLParser : public Parser {
  regex _re;

public:
  XMLParser();
  virtual ~XMLParser() override;
  virtual vector<string> parseStream(const stringstream &stream) override;
};
