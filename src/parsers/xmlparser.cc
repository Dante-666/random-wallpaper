#include "xmlparser.h"
#include <memory>

XMLParser::XMLParser() : 
_re("<span>\\s?<a\\s+href=\"(.*.(jpg|png|jpeg))\"", std::regex::ECMAScript) {};

XMLParser::~XMLParser() {}

vector<string> XMLParser::parseStream(const stringstream &stream) {
	LogOutput("Parsing XML rss stream...");
	TiXmlDocument doc;
  doc.Parse(stream.str().c_str());
  auto root = doc.RootElement();
  TiXmlHandle rootHandle(root);
  auto entry = rootHandle.FirstChild("entry").ToElement();
  vector<string> list;
  while (entry) {
    TiXmlHandle entryHandle(entry);
    auto content = entryHandle.FirstChild("content").ToElement();
		cmatch uri;
		if(regex_search(content->GetText(), uri, _re)) {
			list.emplace_back(std::move(uri[1].str()));
		}
    entry = entry->NextSiblingElement("entry");
  }
  return list;
}
