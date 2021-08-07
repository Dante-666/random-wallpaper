#include "interface.h"

// TODO: make this readable from a config file and keep default values here
const char *Windows::appDataLoc = getenv("appdata");
const char *Linux::tmpWorkDir = "~/.local/share/rwall";

// GCC
#if defined __linux__
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

int UtilImpl::systemCall(const string &command) {
  return std::system(command.c_str());
}

const char *access_denied::what() const noexcept {
  return "couldn't create/open file";
}

Linux::~Linux(){};

vector<string> Linux::fetchFiles(const path &dir) try {
  // TODO: get away with this in the constructor?
  const auto &absPath = OSUtils::replaceHome(tmpWorkDir);
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
    Logger::LogInfo("Opening file to read lsout...");
    input.open((absPath / ".linux_lsout").string());
    if (not input.is_open())
      throw access_denied{};
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

const path Linux::getConfFile() {
  return OSUtils::replaceHome("~/.config/rwall.conf");
}

Windows::~Windows(){};

vector<string> Windows::fetchFiles(const path &dir) try {
  path absPath(appDataLoc);
  absPath /= "rwall";
  if (not exists(absPath)) {
    Logger::LogInfo("Creating shared directory " + absPath.string());
    std::filesystem::create_directories(absPath);
  }
  auto absolutify = [](const path &location, const string &extension) {
    return "\"" + absolute(location / extension).string() + "\" ";
  };
  stringstream command;
  command << "dir " << absolutify(dir, "*.png") << absolutify(dir, "*.jpg")
          << absolutify(dir, "*.jpeg") << "/b /s > "
          << (absPath / "dirout.txt");
  Logger::LogDebug(canonical(absolute(dir)).string());
  Logger::LogDebug(command.str());
  if (systemCall(command.str()) == 0) {
    ifstream input;
    {
      Logger::LogInfo("Opening file to read lsout...");
      input.open((absPath / "dirout.txt").string());
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
} catch (std::exception &e) {
  Logger::LogError(e.what());
  return {};
}

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

// TODO: fix a location for this
const path Windows::getConfFile() {
  return OSUtils::replaceHome("~/rwall.conf");
}
