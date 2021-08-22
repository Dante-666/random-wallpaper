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
using std::filesystem::create_directory;
using std::filesystem::directory_options;
using std::filesystem::exists;
using std::filesystem::path;
using std::filesystem::recursive_directory_iterator;

using std::regex;
using std::regex_replace;
using std::regex_constants::format_first_only;

class access_denied : std::exception {
  virtual const char *what() const noexcept override;
};

class UtilImpl {
public:
  virtual ~UtilImpl(){};
  virtual void updateWallpaper(const string &uri) = 0;
};

class OSUtils {
  static UtilImpl &_impl;

public:
  ~OSUtils() { delete &_impl; }
  static vector<string> fetchFiles(const path &dir);
  /* converts shell ~ to absolute home path.
   * Can be used with windows as well.
   */
  static const path replaceHome(const path sPath);
  static void updateWallpaper(const string &uri) { _impl.updateWallpaper(uri); }
};

class Linux : public UtilImpl {
  static const char *tmpWorkDir;
  virtual ~Linux() override;
  virtual void updateWallpaper(const string &uri) override;
};

class Windows : public UtilImpl {
  static const char *appDataLoc;
  virtual ~Windows() override;
  virtual void updateWallpaper(const string &uri) override;

public:
  Windows();
};

class Mac : public UtilImpl {
  static const char *tmpWorkDir;
  virtual ~Mac() override;
  virtual void updateWallpaper(const string &uri) override;

public:
  Mac();
};
