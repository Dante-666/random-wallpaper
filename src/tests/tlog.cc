#include "tcommon.h"

#include <logutil/logger.h>

#ifdef __linux__
TEST(testLogger, consoleLogging) {
  string matchDebug =
      "[DEBUG] : testLogger_consoleLogging_Test::TestBody -> consoleDebug";
  string matchInfo =
      "[INFO] : testLogger_consoleLogging_Test::TestBody -> consoleInfo";
  string matchWarn =
      "[WARN] : testLogger_consoleLogging_Test::TestBody -> consoleWarn";
  string matchError =
      "[ERROR] : testLogger_consoleLogging_Test::TestBody -> consoleError";

  {
    auto *backup = std::cout.rdbuf();
    stringstream buffer;
    std::cout.rdbuf(buffer.rdbuf());

    Logger::SetLogLevel(LogLevel::DEBUG);
    Logger::LogDebug("consoleDebug");
    Logger::LogInfo("consoleInfo");
    Logger::LogWarn("consoleWarn");
    Logger::LogError("consoleError");

    std::cout.rdbuf(backup);
#ifdef __GNUC__
    const auto &output = buffer.str();
    EXPECT_TRUE(output.find(matchDebug) != string::npos);
    EXPECT_TRUE(output.find(matchInfo) != string::npos);
    EXPECT_TRUE(output.find(matchWarn) != string::npos);
    EXPECT_TRUE(output.find(matchError) != string::npos);
#endif
  }

  {
    auto *backup = std::cout.rdbuf();
    stringstream buffer;
    std::cout.rdbuf(buffer.rdbuf());
    Logger::SetLogLevel(LogLevel::INFO);
    Logger::LogDebug("consoleDebug");
    Logger::LogInfo("consoleInfo");
    Logger::LogWarn("consoleWarn");
    Logger::LogError("consoleError");

    std::cout.rdbuf(backup);
#ifdef __GNUC__
    const auto &output = buffer.str();
    EXPECT_FALSE(output.find(matchDebug) != string::npos);
    EXPECT_TRUE(output.find(matchInfo) != string::npos);
    EXPECT_TRUE(output.find(matchWarn) != string::npos);
    EXPECT_TRUE(output.find(matchError) != string::npos);
#endif
  }

  {
    auto *backup = std::cout.rdbuf();
    stringstream buffer;
    std::cout.rdbuf(buffer.rdbuf());
    Logger::SetLogLevel(LogLevel::WARN);
    Logger::LogDebug("consoleDebug");
    Logger::LogInfo("consoleInfo");
    Logger::LogWarn("consoleWarn");
    Logger::LogError("consoleError");

    std::cout.rdbuf(backup);
#ifdef __GNUC__
    const auto &output = buffer.str();
    EXPECT_FALSE(output.find(matchDebug) != string::npos);
    EXPECT_FALSE(output.find(matchInfo) != string::npos);
    EXPECT_TRUE(output.find(matchWarn) != string::npos);
    EXPECT_TRUE(output.find(matchError) != string::npos);
#endif
  }

  {
    auto *backup = std::cout.rdbuf();
    stringstream buffer;
    std::cout.rdbuf(buffer.rdbuf());
    Logger::SetLogLevel(LogLevel::ERROR);
    Logger::LogDebug("consoleDebug");
    Logger::LogInfo("consoleInfo");
    Logger::LogWarn("consoleWarn");
    Logger::LogError("consoleError");

    std::cout.rdbuf(backup);
#ifdef __GNUC__
    const auto &output = buffer.str();
    EXPECT_FALSE(output.find(matchDebug) != string::npos);
    EXPECT_FALSE(output.find(matchInfo) != string::npos);
    EXPECT_FALSE(output.find(matchWarn) != string::npos);
    EXPECT_TRUE(output.find(matchError) != string::npos);
#endif
  }
#ifdef NDEBUG
  Logger::SetLogLevel(LogLevel::WARN);
#else
  Logger::SetLogLevel(LogLevel::DEBUG);
#endif
}
#endif
