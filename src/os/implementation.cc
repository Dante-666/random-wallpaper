#include "interface.h"

// GCC
#ifdef __linux__
UtilImpl &OSUtils::_impl = *(new Linux());
#endif

// MSVC
#ifdef _WIN64 || _WIN32
UtilImpl &OSUtils::_impl = *(new Windows());
#endif

Linux::~Linux() {};

vector<string> Linux::fetchFiles(const char *dir) {
	return {};
}

int Linux::systemCall(const char *command) {
	return std::system(command);
}

void Linux::updateWallpaper(const string& uri) {
	stringstream command;
	command << "feh --bg-fill " << uri;
	systemCall(command.str().c_str());
}

Windows::~Windows() {};

vector<string> Windows::fetchFiles(const char *dir) {
	return {};
}

int Windows::systemCall(const char *command) {
	return std::system(command);
}

void Windows::updateWallpaper(const string& uri) {
	stringstream command;
	command << "feh --bg-fill " << uri;
	systemCall(command.str().c_str());
}