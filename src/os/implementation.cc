#include "interface.h"

#ifdef __linux__
UtilImpl &OSUtils::_impl = *(new Linux());
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
