#include "tcommon.h"

#include "container/container.h"

TEST(testContainer, emptyContainer) {
  Container tContainer;
  EXPECT_THROW(tContainer.getURI(), std::out_of_range);
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

  vector<string> list{"a", "aa", "aaa", "b", "bb", "bbb", "c", "cc", "ccc"};
  vector<string> output;
  bool run = true;
  for_each(list.begin(), list.end(),
           [&](const string &) { output.push_back(tContainer.getURI()); });

  EXPECT_EQ(output.size(), list.size());
  EXPECT_TRUE(output != list);
}

TEST(testContainer, repetition) {
  Container tContainer;
  EXPECT_NO_THROW(tContainer.randomize());
  {
    vector<string> list1{"a", "aa", "aaa"};
    vector<string> list2{"a", "aa", "aaa"};
    EXPECT_NO_THROW(tContainer.append(list1));
    EXPECT_NO_THROW(tContainer.append(list2));
  }

  EXPECT_NO_THROW(tContainer.randomize());

  vector<string> outPassOne(3);
  for_each(outPassOne.begin(), outPassOne.end(),
           [&](const string &) { outPassOne.push_back(tContainer.getURI()); });
  vector<string> outPassTwo(3);
  for_each(outPassTwo.begin(), outPassTwo.end(),
           [&](const string &) { outPassTwo.push_back(tContainer.getURI()); });

  EXPECT_TRUE(outPassOne == outPassTwo);
}
