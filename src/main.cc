#include "container/container.h"
#include "fetcher/cfetcher.h"
#include "os/interface.h"
#include "parsers/xmlparser.h"

#include "parsers/configparser.h"

int main() {
  Logger::LogInfo("Initializing...");

  // TODO: Add these and other URI sources in *.conf
  string uri = "https://www.reddit.com/r/Animewallpaper/"
               "search.rss?q=flair_name%3A%22Desktop%22&restrict_sr=1";
  auto out = CurlFetcher::fetchResource(uri);

  std::unique_ptr<SchemaParser> parser;
  parser.reset(new XMLParser());

  Container container;
  {
    auto list_rss = parser->parseStream(out);
		//TODO: make path configurable but keep this as default
    auto list_local = OSUtils::fetchFiles("~/pictures/wallpaper");

    container.append(list_rss);
    container.append(list_local);
    container.randomize();
  }

  OSUtils::updateWallpaper(container.getURI());

  Logger::LogInfo("Exiting...");

  return 0;
}
