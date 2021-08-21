#pragma once

#if defined __linux__ || __APPLE__
#include <gtest/gtest.h>
#elif defined _WIN64 || defined _WIN32
#include "gtest/gtest.h"
#endif
