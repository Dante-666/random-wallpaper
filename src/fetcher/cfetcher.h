#pragma once

#include <curl/curl.h>
#include <stdlib.h>

#include <cstring>
#include <memory>
#include <string>

#include "logutil/logger.h"

using std::string;

// TODO #6 unit tests
class MemChunk {
  char *memory;
  size_t size;

public:
  MemChunk();
  ~MemChunk();
  static size_t write_callback(char *buffer, size_t size, size_t nitems,
                               void *outstream);
  string get_string();
};

struct CurlFetcher {
  static string fetchResource(const string &uri);
};
