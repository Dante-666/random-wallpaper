#include "fetcher/cfetcher.h"
// TODO: #5
#include <iostream>

int main() {
  string uri = "https://www.reddit.com/r/Animewallpaper/"
               "search.rss?q=flair_name%3A%22Desktop%22&restrict_sr=1";
  auto out = CurlFetcher::fetchResource(uri);
  std::cout << out << std::endl;
  return 0;
}
