#pragma once

#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;

class UtilImpl {
public:
	virtual ~UtilImpl() {};
  virtual vector<string> fetchFiles(const char *dir) = 0;
  virtual int systemCall(const char *command) = 0;
	virtual void updateWallpaper(const string& uri) = 0;
};

class OSUtils {
  static UtilImpl &_impl;
public:
	~OSUtils() {
		delete &_impl;
	}
  static vector<string> fetchFiles(const char *dir) {
    return _impl.fetchFiles(dir);
  }
  static void updateWallpaper(const string& uri) {
    _impl.updateWallpaper(uri);
  }
};

class Linux : public UtilImpl {
	virtual ~Linux() override;
  virtual vector<string> fetchFiles(const char *dir) override;
  virtual int systemCall(const char *command) override;
	virtual void updateWallpaper(const string& uri) override;
};

class Windows : public UtilImpl {
	virtual ~Windows() override;
  virtual vector<string> fetchFiles(const char *dir) override;
  virtual int systemCall(const char *command) override;
	virtual void updateWallpaper(const string& uri) override;
};