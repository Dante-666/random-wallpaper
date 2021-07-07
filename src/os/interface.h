#pragma once

#include "fetcher/cfetcher.h"
#include "logutil/logger.h"

#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

class UtilImpl {
public:
  virtual ~UtilImpl(){};
  virtual vector<string> fetchFiles(const char *dir) = 0;
  virtual int systemCall(const char *command) = 0;
  virtual void updateWallpaper(const string &uri) = 0;
};

class OSUtils {
  static UtilImpl &_impl;

public:
  ~OSUtils() { delete &_impl; }
  static vector<string> fetchFiles(const char *dir) {
    return _impl.fetchFiles(dir);
  }
  static void updateWallpaper(const string &uri) { _impl.updateWallpaper(uri); }
};

class Linux : public UtilImpl {
  virtual ~Linux() override;
  virtual vector<string> fetchFiles(const char *dir) override;
  virtual int systemCall(const char *command) override;
  virtual void updateWallpaper(const string &uri) override;
};

class Windows : public UtilImpl {
  static const char *tmpLocation;
  virtual ~Windows() override;
  virtual vector<string> fetchFiles(const char *dir) override;
  virtual int systemCall(const char *command) override;
  virtual void updateWallpaper(const string &uri) override;
  static size_t write_callback(char *buffer, size_t size, size_t nitems,
                               ofstream *outstream);
};
