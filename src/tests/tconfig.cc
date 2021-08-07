#include "tcommon.h"

#include "parsers/configparser.h"

TEST(testConfigParser, test1) {
	auto parser = ConfigParser("test_resource/tconfig/rwall.conf");
}
