#include "configparser.h"

ConfigParser::ConfigParser(const path &confFile) try {
	ifstream input;
	Logger::LogInfo("Opening config file...");
	input.open(confFile.string());
	if(not input.is_open())
		throw access_denied{};

	stringstream sstream;
	sstream << input.rdbuf();
	input.close();

	Logger::LogDebug(sstream.str());
	std::string output;
	auto data = sstream.str();

	Iter start = data.begin();
	Iter end = data.end();

	qi::phrase_parse(start, end, Config, skipgg, output);
	//qi::phrase_parse(start, end, c, output);

	Logger::LogDebug(output);

} catch (const std::exception &e) {
  Logger::LogError(e.what());
}

ConfigParser::~ConfigParser() {}

const char *access_denied::what() const noexcept {
  return "unable to open file";
}
