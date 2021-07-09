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
UtilImpl &OSUtils::_impl = *(new Windows());
#endif

int UtilImpl::systemCall(const string &command)
{
  return std::system(command.c_str());
}

const char *access_denied::what() const noexcept
{
  return "couldn't create/open file";
}

Linux::~Linux(){};

vector<string> Linux::fetchFiles(const path &dir)
try
{
  //TODO: get away with this in the constructor?
  const auto &absPath = replaceHome(tmpWorkDir);
  if (not exists(absPath))
  {
    Logger::LogInfo("Creating shared directory " + absPath.string());
    std::filesystem::create_directories(absPath);
  }
  stringstream command;
  command << "find " << absolute(dir)
          << " -regex  \".*.\\(png\\|jpg\\|jpeg\\)$\" 1> " << tmpWorkDir
          << "/.linux_lsout";
  if (systemCall(command.str()) == 0)
  {
    ifstream input;
    {
      Logger::LogInfo("Opening file to read lsout...");
      input.open((absPath / ".linux_lsout").string());
      if (not input.is_open())
        throw access_denied{};
    }
    vector<string> output;
    for (string line; getline(input, line);)
    {
      Logger::LogDebug(line);
      output.emplace_back(std::move(line));
    }
    input.close();
    return output;
  }
  else
  {
    throw access_denied{};
  }
}
catch (const std::exception &e)
{
  Logger::LogError(e.what());
  return {};
}

void Linux::updateWallpaper(const string &uri)
{
  stringstream command;
  command << "feh --bg-fill " << uri;
  systemCall(command.str().c_str());
}

const path Linux::replaceHome(const char *sPath)
try
{
  string strPath(sPath);
  regex re("~");
  auto homePrefix = regex_replace(strPath, re, getenv("HOME"), format_first_only);
  Logger::LogDebug(homePrefix);
  exists(homePrefix);
  return homePrefix;
}
catch (const std::exception &e)
{
  Logger::LogError(e.what());
  return {};
}

Windows::~Windows(){};

vector<string> Windows::fetchFiles(const path &dir)
try
{
  path absPath(appDataLoc);
  absPath /= "rwall";
  if (not exists(absPath))
  {
    Logger::LogInfo("Creating shared directory " + absPath.string());
    std::filesystem::create_directories(absPath);
  }
  auto absolutify = [](const path &location, const string &extension)
  {
    return "\"" + absolute(location / extension).string() + "\" ";
  };
  stringstream command;
  command << "dir " << absolutify(dir, "*.png")
          << absolutify(dir, "*.jpg") << absolutify(dir, "*.jpeg")
          << "/b /s > " << (absPath / "dirout.txt");
  Logger::LogDebug(canonical(absolute(dir)).string());
  Logger::LogDebug(command.str());
  if (systemCall(command.str()) == 0)
  {
    ifstream input;
    {
      Logger::LogInfo("Opening file to read lsout...");
      input.open((absPath / "dirout.txt").string());
      if (not input.is_open())
        throw access_denied{};
    }
    vector<string> output;
    for (string line; getline(input, line);)
    {
      Logger::LogDebug(line);
      output.emplace_back(std::move(line));
    }
    input.close();
    return output;
  }
  else
  {
    throw access_denied{};
  }
}
catch (std::exception &e)
{
  Logger::LogError(e.what());
  return {};
}

void Windows::updateWallpaper(const string &uri) try {
  using std::regex;
  using std::regex_search;
  using std::smatch;

  regex re("(.(jpg|png|jpeg)$)");
  smatch ext;

  //TODO: Fix #45
  auto updateHelper = [&] (const path& wallpaper) {
    stringstream itemProp;
    itemProp << "powershell.exe Set-ItemProperty -path 'HKCU:\\Control "
                "Panel\\Desktop\\' -name Wallpaper -value \""
             << wallpaper.string() << "\"";
    Logger::LogDebug(itemProp.str());
    systemCall(itemProp.str());
    systemCall("timeout 2 /NOBREAK > NUL");
    systemCall("powershell.exe rundll32.exe user32.dll, "
               "UpdatePerUserSystemParameters");
  };

  if (exists(uri)) {
    /* File is present on dist */
    updateHelper(uri);
  }
  else if (regex_search(uri, ext, re)) {
    /* download the file first and let this script transcode the image */
    Logger::LogInfo("Initializing download...");
    path tempFilePath(appDataLoc);
    //TODO: Directory creation at instantiation
    tempFilePath = tempFilePath / "rwall";
    if (not exists(tempFilePath))
    {
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

const path Windows::replaceHome(const char *sPath)
try
{
  string strPath(sPath);
  regex re("~");
  auto homePrefix = regex_replace(strPath, re, getenv("USERPROFILE"), format_first_only);
  Logger::LogDebug(homePrefix);
  exists(homePrefix);
  return homePrefix;
}
catch (const std::exception &e)
{
  Logger::LogError(e.what());
  return {};
}