#include "container.h"

Container::~Container() {}

void Container::append(vector<string> &list) {
  Logger::LogInfo("Appending to random container");
  std::move(list.begin(), list.end(), std::back_inserter(_tempStore));
}

void Container::randomize() {
  std::random_device rd;
  std::shuffle(_tempStore.begin(), _tempStore.end(), std::mt19937(rd()));
  std::for_each(_tempStore.begin(), _tempStore.end(),
                [&](const string &it) { _uriStore.insert(it); });
}

const string &Container::getURI() {
  if (_uriStore.empty()) {
    throw std::out_of_range("container is empty!!!");
  }
  std::call_once(_flag, [&]() { _it = _uriStore.begin(); });
  if (_it == _uriStore.end()) {
    _it = _uriStore.begin();
  }
  Logger::LogDebug(*_it);
  return *(_it++);
}
