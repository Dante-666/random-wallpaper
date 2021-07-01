#include "fetcher/cfetcher.h"
#include "parsers/xmlparser.h"


int main() {
  string uri = "https://www.reddit.com/r/Animewallpaper/"
               "search.rss?q=flair_name%3A%22Desktop%22&restrict_sr=1";
  auto out = CurlFetcher::fetchResource(uri);

	std::unique_ptr<Parser> parser;
	parser.reset(new XMLParser());

	auto list = parser->parseStream(out);

	//TODO: remove this and keep lists in common container with pop mechanism
	std::srand(time(nullptr));
	auto img = list[std::rand() % list.size()];

	stringstream command;
	command << "feh --bg-fill " << img;

	//TODO: encapsulate into OS specific commands
	std::system(command.str().c_str());

  return 0;
}
