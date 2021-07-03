#include "interface.h"

// GCC
#ifdef __linux__
UtilImpl &OSUtils::_impl = *(new Linux());
#endif

// MSVC
#if defined _WIN64 || defined _WIN32
#include "curl/curl.h"
#include <regex>
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
	CURL *curl_handle;
	CURLcode retVal;
	curl_handle = curl_easy_init();

	using std::regex;
	using std::regex_search;
	using std::smatch;

	regex re("(.(jpg|png|jpeg)$)");
	smatch ext;

	if (curl_handle && regex_search(uri, ext, re))
	{
		LogOutput("Handle Initialized...");

		const string fullName = tmpLocation + ext[0].str();
		unique_ptr<ofstream> outfile = make_unique<ofstream>(fullName, std::ios_base::binary);
		curl_easy_setopt(curl_handle, CURLOPT_URL, uri.c_str());
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,
										 &Windows::write_callback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, outfile.get());

		LogOutput(fullName);

		retVal = curl_easy_perform(curl_handle);

		curl_easy_cleanup(curl_handle);

		if (retVal == CURLcode::CURLE_OK)
		{
			LogOutput("File written...");
			outfile->close();
			stringstream itemProp;
			itemProp << "powershell.exe Set-ItemProperty -path 'HKCU:\\Control Panel\\Desktop\\' -name Wallpaper -value \"" << fullName << "\"";
			systemCall(itemProp.str().c_str());
			systemCall("timeout 1");
			systemCall("powershell.exe rundll32.exe user32.dll, UpdatePerUserSystemParameters");
		}
		else
		{
			//TODO: error handling
			LogError(curl_easy_strerror(retVal));
		}
	}
	else
	{
		//TODO: error handling
		LogError("Couldn't obtain handle...");
	}
}

size_t Windows::write_callback(char *buffer, size_t size, size_t nitems, ofstream *outstream)
{
	//TODO: error handling
	outstream->write(buffer, nitems);
	return nitems;
}