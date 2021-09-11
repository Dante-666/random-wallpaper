#include "interface.h"
// TODO: add type_traits and disable preprocessor

// TODO: make this readable from a config file and keep default values here
const char *Windows::appDataLoc = getenv("appdata");
const char *Linux::tmpWorkDir = "~/.local/share/rwall";
const char *Mac::tmpWorkDir = "~/.local/share/rwall";

// GCC
#if defined __linux__
UtilImpl &OSUtils::_impl = *(new Linux());

Linux::~Linux(){};

void Linux::updateWallpaper(const string &uri) try {
  stringstream command;
  command << "feh --bg-fill " << uri;
  std::system(command.str().c_str());
} catch (const std::exception &e) {
  Logger::LogError(e.what());
}

const path Linux::getConfFile() {
  return OSUtils::replaceHome("~/.config/rwall.conf");
}
#endif

// MSVC
#if defined _WIN64 || defined _WIN32
#include "WinUser.h"
UtilImpl &OSUtils::_impl = *(new Windows());

Windows::Windows() {
  path tempFilePath(appDataLoc);
  tempFilePath = tempFilePath / "rwall";
  if (not exists(tempFilePath)) {
    create_directory(tempFilePath);
  }
}

Windows::~Windows(){};

void Windows::updateWallpaper(const string &uri) try {
  regex re("(.(jpg|png|jpeg)$)");
  smatch ext;

  auto updateHelper = [&](const path &wallpaper) {
    BOOL retVal;
    retVal = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,
#ifdef UNICODE
                                  (PVOID)wallpaper.wstring().c_str(),
#else
                                  (PVOID)wallpaper.string().c_str(),
#endif
                                  SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
  };

  if (exists(uri)) {
    /* File is present on disk */
    updateHelper(uri);
  } else if (regex_search(uri, ext, re)) {
    /* download the file first and let this script transcode the image */
    Logger::LogInfo("Initializing download...");
    path tempFilePath(appDataLoc);

    tempFilePath = tempFilePath / "rwall" / ("tmpDownload" + ext[0].str());
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

// TODO: fix a location for this
const path Windows::getConfFile() {
  return OSUtils::replaceHome("~/rwall.conf");
}
#endif

// Clang
#if defined __APPLE__
#include "mac_impl.h"
UtilImpl &OSUtils::_impl = *(new Mac());

Mac::Mac() {
  path tmpWorkDirPath(tmpWorkDir);
  tmpWorkDirPath = OSUtils::replaceHome(tmpWorkDirPath);
  if (not exists(tmpWorkDirPath)) {
    create_directories(tmpWorkDirPath);
  }
}

Mac::~Mac(){};

void Mac::updateWallpaper(const string &uri) try {
  regex re("(.(jpg|png|jpeg)$)");
  smatch ext;
  if (exists(uri)) {
    /* File is present on disk */
    setDesktopBackground(uri);
  } else if (regex_search(uri, ext, re)) {
    /* WebURL */
    Logger::LogInfo("Initializing download...");
    //TODO: avoid this all the time and do once in the constructor
    path tempFilePath = OSUtils::replaceHome(tmpWorkDir);
    tempFilePath = tempFilePath / ("tmpDownload" + ext[0].str());
    Logger::LogDebug(tempFilePath.string() + ", " + uri);

    // TODO: handle and then claim file written
    CurlFetcher::writeImageToDisk(uri, tempFilePath.string());
    Logger::LogInfo("File written...");
    setDesktopBackground(tempFilePath);
  }
} catch (const std::exception &e) {
  Logger::LogError(e.what());
}

const path Mac::getConfFile() {
	return OSUtils::replaceHome("~/rwall.conf");
}

#endif

const path OSUtils::replaceHome(const path sPath) try {
  regex re("~");
  auto homePrefix = regex_replace(sPath.string(), re,
#if defined _WIN64 || defined _WIN32
                                  getenv("USERPROFILE"),
#elif defined __linux__ || defined __APPLE__
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
  const auto &fullDir = replaceHome(dir);
  vector<string> output;

  for (auto &fullPath : recursive_directory_iterator(
           fullDir, directory_options::skip_permission_denied)) {
    const auto &ext = fullPath.path().extension();
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

