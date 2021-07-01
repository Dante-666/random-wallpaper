#include "container/container.h"
#include "fetcher/cfetcher.h"
#include "parsers/xmlparser.h"

int main() {
	//TODO: Add these and other URI sources in *.conf
  string uri = "https://www.reddit.com/r/Animewallpaper/"
               "search.rss?q=flair_name%3A%22Desktop%22&restrict_sr=1";
  auto out = CurlFetcher::fetchResource(uri);

	std::unique_ptr<Parser> parser;
	parser.reset(new XMLParser());

	auto list = parser->parseStream(out);

	Container container;
	container.append(list);
	container.randomize();

	//TODO: encapsulate into OS specific commands
	stringstream command;
	auto str = container.popURI();
	command << "feh --bg-fill " << str;

	std::system(command.str().c_str());

	LogOutput("Exiting...");

  return 0;
}
