#include "xmlparser.h"
#include <memory>

XMLParser::~XMLParser() {}

vector<string> XMLParser::parseStream(const stringstream &stream) {
  TiXmlDocument doc;
  doc.Parse(stream.str().c_str());
  auto root = doc.RootElement();
  TiXmlHandle rootHandle(root);
  auto entry = rootHandle.FirstChild("entry").ToElement();
  while (entry) {
    TiXmlHandle entryHandle(entry);
    auto content = entryHandle.FirstChild("content").ToElement();
    LogOutput(content->GetText());
    entry = entry->NextSiblingElement("entry");
  }
  return {};
}
