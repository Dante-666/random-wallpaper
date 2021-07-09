#pragma once

#include "fetcher/cfetcher.h"
#include "logutil/logger.h"

#include <filesystem>
#include <memory>
#include <regex>

#include <fstream>
#include <sstream>
#include <string>

#include <vector>

using std::ifstream;
using std::ostringstream;
using std::string;
using std::stringstream;
using std::vector;

using std::filesystem::absolute;
using std::filesystem::create_directories;
using std::filesystem::exists;
using std::filesystem::path;

using std::regex;
using std::regex_replace;
using std::regex_constants::format_first_only;

class access_denied : std::exception
{
  virtual const char *what() const noexcept override;
};

class UtilImpl
{
public:
  virtual ~UtilImpl(){};
  /* Provide a relative/absolute path here */
  virtual vector<string> fetchFiles(const path &dir) = 0;
  virtual int systemCall(const string &command);
  virtual void updateWallpaper(const string &uri) = 0;
  // TODO: replace this later
  virtual const path replaceHome(const char *sPath) = 0;
};

class OSUtils
{
  static UtilImpl &_impl;

public:
  ~OSUtils() { delete &_impl; }
  static vector<string> fetchFiles(const path &dir)
  {
    return _impl.fetchFiles(replaceHome(dir));
  }
  static void updateWallpaper(const string &uri) { _impl.updateWallpaper(uri); }
  /* converts shell ~ to absolute home path */
  static const path replaceHome(const path sPath) try {
    regex re("~");
    auto homePrefix = regex_replace(sPath.string(), re, getenv("USERPROFILE"), format_first_only);
    path homeP(homePrefix);
    Logger::LogDebug(homePrefix);
    exists(homeP);
    return homePrefix;
  } catch (const std::exception &e) {
    Logger::LogError(e.what());
    return {};
  }
};

class Linux : public UtilImpl
{
  static const char *tmpWorkDir;
  virtual ~Linux() override;
  virtual vector<string> fetchFiles(const path &dir) override;
  virtual void updateWallpaper(const string &uri) override;
  virtual const path replaceHome(const char *sPath) override;
};

class Windows : public UtilImpl
{
  static const char *appDataLoc;
  virtual ~Windows() override;
  virtual vector<string> fetchFiles(const path &dir) override;
  virtual void updateWallpaper(const string &uri) override;
  virtual const path replaceHome(const char *sPath) override;
};
