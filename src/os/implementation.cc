#include "interface.h"

// GCC
#ifdef __linux__
UtilImpl &OSUtils::_impl = *(new Linux());
#endif

// MSVC
#if defined _WIN64 || defined _WIN32
#include "curl/curl.h"
const char* Windows::tmpDownload = "D:/test.jpg";
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
	/* take test.ps1 as a guide and work it up */
	/* download the file first and let this script transcode the image */
  CURL *curl_handle;
  CURLcode retVal;
  curl_handle = curl_easy_init();

  if (curl_handle) {
    LogOutput("Handle Initialized...");
		unique_ptr<ofstream> outfile = make_unique<ofstream>(tmpDownload, std::ios_base::binary);
    curl_easy_setopt(curl_handle, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,
                     &Windows::write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, outfile.get());

    retVal = curl_easy_perform(curl_handle);

    curl_easy_cleanup(curl_handle);

    if (retVal == CURLcode::CURLE_OK) {
			LogOutput("File written...");
			outfile->close();
    } else {
      LogError(curl_easy_strerror(retVal));
    }
  } else {
		LogError("Couldn't obtain handle...");
	}

	stringstream itemProp;
	itemProp << "powershell.exe Set-ItemProperty -path 'HKCU:\\Control Panel\\Desktop\\' -name Wallpaper -value \"" << tmpDownload << "\"";
	systemCall(itemProp.str().c_str());
	systemCall("powershell.exe rundll32.exe user32.dll, UpdatePerUserSystemParameters");
}

size_t Windows::write_callback(char* buffer, size_t size, size_t nitems, ofstream* outstream) {
	//TODO: error handling
	outstream->write(buffer, nitems);
	return nitems;
}