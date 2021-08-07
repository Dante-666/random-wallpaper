#pragma once

#include "schemaparser.h"
#include "tinyxml.h"

class XMLParser : public SchemaParser {
  regex _re;

public:
  XMLParser();
  virtual ~XMLParser() override;
  virtual vector<string> parseStream(const stringstream &stream) override;
};
