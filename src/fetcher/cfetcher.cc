#include "cfetcher.h"

MemChunk::MemChunk() {
  this->memory = static_cast<char *>(malloc(1));
  this->size = 0;
}
MemChunk::~MemChunk() {
  if (memory)
    free(memory);
}

size_t MemChunk::write_callback(char *buffer, size_t size, size_t nitems,
                                void *outstream) {
  auto realsize = size * nitems;
  auto *memChunk = static_cast<MemChunk *>(outstream);

  char *ptr = static_cast<char *>(
      realloc(memChunk->memory, memChunk->size + realsize + 1));
  if (ptr) {
    memChunk->memory = ptr;
    memcpy(&(memChunk->memory[memChunk->size]), buffer, realsize);
    memChunk->size += realsize;
    memChunk->memory[memChunk->size] = 0;
    return realsize;
  } else {
    Logger::LogError("Not enough memory!");
    return 0;
  }
}

stringstream MemChunk::get_string() {
  if (this->size) {
    stringstream stream{this->memory};
    return stream;
  } else
    return {};
}

stringstream CurlFetcher::fetchResource(const string &uri) {
  CURL *curl_handle;
  CURLcode retVal;
  curl_handle = curl_easy_init();

  if (curl_handle) {
    Logger::LogDebug("Handle Initialized...");
    std::unique_ptr<MemChunk> chunk = std::make_unique<MemChunk>();
    curl_easy_setopt(curl_handle, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,
                     &MemChunk::write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, chunk.get());

    retVal = curl_easy_perform(curl_handle);

    curl_easy_cleanup(curl_handle);

    if (retVal == CURLcode::CURLE_OK) {
      return chunk->get_string();
    } else {
      // TODO: error handling
      Logger::LogError(curl_easy_strerror(retVal));
    }
  }
  // TODO: error handling
  Logger::LogError("Couldn't obtain handle...");
  return {};
}

void CurlFetcher::writeImageToDisk(const string &uri, const string &file) {
  CURL *curl_handle;
  CURLcode retVal;
  curl_handle = curl_easy_init();

  if (curl_handle) {
    Logger::LogDebug("Handle Initialized...");
    unique_ptr<ofstream> outfile =
        make_unique<ofstream>(file, std::ios_base::binary);
    curl_easy_setopt(curl_handle, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(
        curl_handle, CURLOPT_WRITEFUNCTION,
        +[](char *buffer, size_t size, size_t nitems,
            ofstream *outstream) -> size_t {
          // TODO: error handling
          outstream->write(buffer, nitems);
          return nitems;
        });
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, outfile.get());

    retVal = curl_easy_perform(curl_handle);

    curl_easy_cleanup(curl_handle);

    if (retVal == CURLcode::CURLE_OK) {
      outfile->close();
    } else {
      // TODO: error handling
      Logger::LogError(curl_easy_strerror(retVal));
    }
  } else {
    // TODO: error handling
    Logger::LogError("Couldn't obtain handle...");
  }
}
