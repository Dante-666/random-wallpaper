#include <gtest/gtest.h>
//#include "gtest/gtest.h"

#include "container/container.h"

TEST(testContainer, emptyContainer) {
  Container tContainer;
  EXPECT_THROW(tContainer.popURI(), std::out_of_range);
}

TEST(testContainer, emptyRandomize) {
  Container tContainer;
  EXPECT_NO_THROW(tContainer.randomize());
  {
    vector<string> list1{"a", "aa", "aaa"};
    vector<string> list2{"b", "bb", "bbb"};
    vector<string> list3{"c", "cc", "ccc"};
    EXPECT_NO_THROW(tContainer.append(list1));
    EXPECT_NO_THROW(tContainer.append(list2));
    EXPECT_NO_THROW(tContainer.append(list3));
    EXPECT_TRUE(std::all_of(list1.begin(), list1.end(),
                            [](const string &str) { return str == ""; }));
  }

  EXPECT_NO_THROW(tContainer.randomize());

  std::vector<string> output;
  bool run = true;
  while (run) {
    try {
      output.emplace_back(tContainer.popURI());
    } catch (const std::out_of_range &) {
      run = false;
    }
  }

  vector<string> list{"a", "aa", "aaa", "b", "bb", "bbb", "c", "cc", "ccc"};
  EXPECT_EQ(output.size(), list.size());
  EXPECT_TRUE(output != list);
}

TEST(testContainer, duplicacy) {
  Container tContainer;
  EXPECT_NO_THROW(tContainer.randomize());
  {
    vector<string> list1{"a", "aa", "aaa"};
    vector<string> list2{"a", "aa", "aaa"};
    EXPECT_NO_THROW(tContainer.append(list1));
    EXPECT_NO_THROW(tContainer.append(list2));
  }

  EXPECT_NO_THROW(tContainer.randomize());

  std::set<string> output;
  bool run = true;
  while (run) {
    try {
      auto it = output.insert(tContainer.popURI());
			EXPECT_TRUE(it.second);
    } catch (const std::out_of_range &) {
      run = false;
    }
  }
}
