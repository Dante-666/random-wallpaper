#include "container.h"

Container::~Container() { }

void Container::append(vector<string>& list) {
	LogOutput("Appending to random container");
	std::move(list.begin(), list.end(), std::back_inserter(_tempStore));
}

void Container::randomize() {
	std::random_device rd;
	std::shuffle(_tempStore.begin(), _tempStore.end(), std::mt19937(rd()));
	std::move(_tempStore.begin(), _tempStore.end(), std::back_inserter(_uriStore));
	_tempStore.clear();
}

const string Container::popURI() {
	const auto top = _uriStore.back();
	_uriStore.pop_back();
	return top;
}
