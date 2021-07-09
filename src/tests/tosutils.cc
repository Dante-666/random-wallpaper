#include "tcommon.h"

#include "os/interface.h"

#include <unordered_set>

using std::string_view;
using std::unordered_set;

TEST(testFetchFile, os_independent) {
  auto list = OSUtils::fetchFiles("test_resource");

  unordered_set<string_view> slist;
  for_each(list.begin(), list.end(),
           [&](const string &str) { slist.insert(str); });

  vector<string> outputs{"test_resource/tos/z.jpeg",
                         "test_resource/tos/1/x.png",
                         "test_resource/tos/1/with space/y.jpg"};

  for_each(outputs.begin(), outputs.end(), [&](const string &str) {
    EXPECT_TRUE(slist.find(absolute(str).string()) != slist.end());
  });

	// Test for filters to only work on image files
	EXPECT_TRUE(slist.find("test_resource/tos/a.txt") == slist.end());
}