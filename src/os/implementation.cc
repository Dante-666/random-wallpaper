#include "interface.h"

// TODO: make this readable from a config file and keep default values here
const char *Windows::appDataLoc = getenv("appdata");
const char *Linux::tmpWorkDir = "~/.local/share/rwall";

// GCC
#ifdef __linux__
UtilImpl &OSUtils::_impl = *(new Linux());
#endif

// MSVC
#if defined _WIN64 || defined _WIN32
#include "WinUser.h"
UtilImpl &OSUtils::_impl = *(new Windows());
#endif

const path OSUtils::replaceHome(const path sPath) try {
  regex re("~");
  auto homePrefix = regex_replace(sPath.string(), re,
#if defined _WIN64 || defined _WIN32
                                  getenv("USERPROFILE"),
#elif defined __linux__
                                  getenv("HOME"),
#endif
                                  format_first_only);
  path homeP(homePrefix);
  Logger::LogDebug(homePrefix);
  (void)exists(homeP);
  return homePrefix;
} catch (const std::exception &e) {
  Logger::LogError(e.what());
  return {};
}

vector<string> OSUtils::fetchFiles(const path &dir) try {
  auto fullDir = replaceHome(dir);
  vector<string> output;

  for (auto &fullPath : recursive_directory_iterator(
           fullDir, directory_options::skip_permission_denied)) {
    auto ext = fullPath.path().extension();
    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
      Logger::LogDebug(fullPath.path().string());
      output.emplace_back(absolute(fullPath.path()).string());
    }
  }
  return output;
} catch (const std::exception &e) {
  Logger::LogError(e.what());
  return {};
}

int UtilImpl::systemCall(const string &command) {
  return std::system(command.c_str());
}

const char *access_denied::what() const noexcept {
  return "couldn't create/open file";
}

Linux::~Linux(){};

void Linux::updateWallpaper(const string &uri) {
  stringstream command;
  command << "feh --bg-fill " << uri;
  systemCall(command.str().c_str());
}

Windows::~Windows(){};

void Windows::updateWallpaper(const string &uri) try {
  using std::regex;
  using std::regex_search;
  using std::smatch;

  regex re("(.(jpg|png|jpeg)$)");
  smatch ext;

  auto updateHelper = [&](const path &wallpaper) {
#ifdef WINUSERAPI
    BOOL retVal;
    retVal = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,
#ifdef UNICODE
                                  (PVOID)wallpaper.wstring().c_str(),
#else
                                  (PVOID)wallpaper.string().c_str(),
#endif
                                  SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
#endif
  };

  if (exists(uri)) {
    /* File is present on dist */
    updateHelper(uri);
  } else if (regex_search(uri, ext, re)) {
    /* download the file first and let this script transcode the image */
    Logger::LogInfo("Initializing download...");
    path tempFilePath(appDataLoc);
    // TODO: Directory creation at instantiation
    tempFilePath = tempFilePath / "rwall";
    if (not exists(tempFilePath)) {
      create_directory(tempFilePath);
    }
    tempFilePath = tempFilePath / ("tmpDownload" + ext[0].str());
    Logger::LogDebug(tempFilePath.string() + ", " + uri);

    // TODO: handle and then claim file written
    CurlFetcher::writeImageToDisk(uri, tempFilePath.string());
    Logger::LogInfo("File written...");
    updateHelper(tempFilePath);
  } else {
    Logger::LogError("Invalid file extension...");
  }
} catch (const std::exception &e) {
  Logger::LogError(e.what());
}
