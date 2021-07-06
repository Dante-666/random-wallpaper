#include "container.h"

Container::~Container() {}

void Container::append(vector<string> &list) {
  //LogOutput("Appending to random container");
  std::move(list.begin(), list.end(), std::back_inserter(_tempStore));
}

void Container::randomize() {
  std::random_device rd;
  std::shuffle(_tempStore.begin(), _tempStore.end(), std::mt19937(rd()));
  set<string> uniqueSet;
  std::for_each(_tempStore.begin(), _tempStore.end(),
                [&](const string &it) { uniqueSet.insert(it); });
  _tempStore.clear();
  std::move(uniqueSet.begin(), uniqueSet.end(),
            std::back_inserter(_uriStore));
}

const string Container::popURI() {
  if (_uriStore.empty()) {
    throw std::out_of_range("container is empty!!!");
  }
  const auto top = _uriStore.back();
  _uriStore.pop_back();
  return top;
}
