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
		LogError("Not enough memory!");
    return 0;
  }
}

string MemChunk::get_string() {
	return string(this->memory);
}

string CurlFetcher::fetchResource(const string &uri) {
  CURL *curl_handle;
  CURLcode retVal;
  curl_handle = curl_easy_init();

  if (curl_handle) {
		LogOutput("Handle Initialized...");
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
			LogError(curl_easy_strerror(retVal));
    }
  }
	LogError("Couldn't obtain handle...");
	return string();
}
