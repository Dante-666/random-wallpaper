#pragma once

#include <curl/curl.h>
#include <stdlib.h>

#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>

#include "logutil/logger.h"

using std::make_unique;
using std::ofstream;
using std::stringstream;
using std::unique_ptr;

// TODO #6 unit tests
class MemChunk {
  char *memory;
  size_t size;

public:
  MemChunk();
  ~MemChunk();
  static size_t write_callback(char *buffer, size_t size, size_t nitems,
                               void *outstream);
  stringstream get_string();
};

struct CurlFetcher {
  static stringstream fetchResource(const string &uri);
  static void writeImageToDisk(const string &uri, const string &file);
};
