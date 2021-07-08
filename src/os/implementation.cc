#include "interface.h"

// TODO: make this readable from a config file and keep default values here
const char *Windows::tmpLocation = "D:/test";
// TODO: implement full path
const char *Linux::tmpWorkDir = "~/.local/share/rwall";

// GCC
#ifdef __linux__
UtilImpl &OSUtils::_impl = *(new Linux());
#endif

// MSVC
#if defined _WIN64 || defined _WIN32
UtilImpl &OSUtils::_impl = *(new Windows());
#endif

int UtilImpl::systemCall(const string &command) {
  return std::system(command.c_str());
}

const char *access_denied::what() const noexcept {
  return "couldn't create/open file";
}

Linux::~Linux(){};

vector<string> Linux::fetchFiles(const path &dir) try {
  const auto &absPath = shellToAbs(tmpWorkDir);
  if (not exists(absPath)) {
    Logger::LogInfo("Creating shared directory " + absPath.string());
    std::filesystem::create_directories(absPath);
  }
  stringstream command;
  command << "find " << absolute(dir)
          << " -regex  \".*.\\(png\\|jpg\\|jpeg\\)$\" 1> " << tmpWorkDir
          << "/.linux_lsout";
  if (systemCall(command.str()) == 0) {
    ifstream input;
    {
      Logger::LogInfo("Opening file to read lsout...");
      input.open((absPath / ".linux_lsout").string());
      if (not input.is_open())
        throw access_denied{};
    }
    vector<string> output;
    for (string line; getline(input, line);) {
			Logger::LogDebug(line);
      output.emplace_back(std::move(line));
    }
    input.close();
    return output;
  } else {
    throw access_denied{};
  }
} catch (const std::exception &e) {
  Logger::LogError(e.what());
  return {};
}

void Linux::updateWallpaper(const string &uri) {
  stringstream command;
  command << "feh --bg-fill " << uri;
  systemCall(command.str().c_str());
}

const path Linux::shellToAbs(const char *sPath) {
  string strPath(sPath);
  regex re("~");
  stringstream homePrefix;
  homePrefix << getenv("HOME")
             << regex_replace(strPath, re, "", format_first_only);
	Logger::LogDebug(homePrefix.str());
  return homePrefix.str();
}

Windows::~Windows(){};

vector<string> Windows::fetchFiles(const path &dir) { return {}; }

void Windows::updateWallpaper(const string &uri) {
  /* download the file first and let this script transcode the image */

  using std::regex;
  using std::regex_search;
  using std::smatch;

  regex re("(.(jpg|png|jpeg)$)");
  smatch ext;

  if (regex_search(uri, ext, re)) {
    Logger::LogInfo("Initializing download...");
    const string fullName = tmpLocation + ext[0].str();
		Logger::LogDebug(fullName + ", " + uri);

    // TODO: handle and then claim file written
    CurlFetcher::writeImageToDisk(uri, fullName);
    Logger::LogInfo("File written...");
    stringstream itemProp;
    itemProp << "powershell.exe Set-ItemProperty -path 'HKCU:\\Control "
                "Panel\\Desktop\\' -name Wallpaper -value \""
             << fullName << "\"";
    systemCall(itemProp.str());
    systemCall("timeout 1 /NOBREAK > NUL");
    systemCall("powershell.exe rundll32.exe user32.dll, "
               "UpdatePerUserSystemParameters");
  } else {
    // TODO: error handling
    Logger::LogError("Invalid file extension...");
  }
}
