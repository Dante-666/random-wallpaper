#include "tcommon.h"

#include "os/interface.h"

//TODO: incorporate methods from this later
#include <filesystem>

//TODO: this will fail on windows, fix #41
#ifdef __linux__
TEST(testFetchFile, linux) {
	auto path = std::filesystem::absolute("test_resource");
	auto list = OSUtils::fetchFiles(path.string().c_str());
	EXPECT_EQ(list[0], "/home/dante/git/random-wallpaper/test_resource/tos/z.jpeg");
	EXPECT_EQ(list[1], "/home/dante/git/random-wallpaper/test_resource/tos/1/x.png");
	EXPECT_EQ(list[2], "/home/dante/git/random-wallpaper/test_resource/tos/1/2/y.jpg");
}
#endif
