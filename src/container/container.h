#pragma once

#include "logutil/logger.h"

#include <algorithm>
#include <cassert>
#include <deque>
#include <mutex>
#include <random>
#include <unordered_set>
#include <string>
#include <vector>

using std::deque;
using std::unordered_set;
using std::string;
using std::vector;

class Container {
  // TODO: create a better class than string
  unordered_set<string> _uriStore;
  // For shuffle
  vector<string> _tempStore;
  std::once_flag _flag;
  unordered_set<string>::const_iterator _it;

public:
  ~Container();
  /* The lifecycle of list should determine the clearance of memory*/
  void append(vector<string> &list);
  void randomize();
  const string& getURI();
};
