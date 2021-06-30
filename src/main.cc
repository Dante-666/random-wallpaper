#include "fetcher/cfetcher.h"
#include "parsers/xmlparser.h"

int main() {
  string uri = "https://www.reddit.com/r/Animewallpaper/"
               "search.rss?q=flair_name%3A%22Desktop%22&restrict_sr=1";
  auto out = CurlFetcher::fetchResource(uri);

	std::unique_ptr<Parser> parser;
	parser.reset(new XMLParser());

	parser->parseStream(out);

  return 0;
}
