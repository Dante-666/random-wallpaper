#pragma once

#include "parser.h"

Parser::~Parser() {};

class XMLParser : public Parser {
public:
  virtual ~XMLParser() override;
  virtual vector<string> parseStream(const ostream &stream) override;
};
