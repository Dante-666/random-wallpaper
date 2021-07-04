#include "interface.h"

// GCC
#ifdef __linux__
UtilImpl &OSUtils::_impl = *(new Linux());
#endif

// MSVC
#if defined _WIN64 || defined _WIN32
const char* Windows::tmpLocation = "D:/test";
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

void Windows::updateWallpaper(const string &uri)
{
	/* download the file first and let this script transcode the image */

	using std::regex;
	using std::regex_search;
	using std::smatch;

	regex re("(.(jpg|png|jpeg)$)");
	smatch ext;

	if (regex_search(uri, ext, re))
	{
		LogOutput("Initializing download...");
		const string fullName = tmpLocation + ext[0].str();

		//TODO: handle and then claim file written
		CurlFetcher::writeImageToDisk(uri, fullName);
		LogOutput("File written...");
		stringstream itemProp;
		itemProp << "powershell.exe Set-ItemProperty -path 'HKCU:\\Control Panel\\Desktop\\' -name Wallpaper -value \"" << fullName << "\"";
		systemCall(itemProp.str().c_str());
		systemCall("timeout 1 /NOBREAK > NUL");
		systemCall("powershell.exe rundll32.exe user32.dll, UpdatePerUserSystemParameters");
	}
	else
	{
		//TODO: error handling
		LogError("Invalid file extension...");
	}
}

