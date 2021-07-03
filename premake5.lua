workspace "random-wallpaper"
  configurations { "Debug", "Release" }

project "random-wallpaper"
  kind "ConsoleApp"
  language "C++"
  location "build"
	cppdialect "C++17"
	-- TODO: add 32 bit support later
	architecture "x64"

	includedirs { "src", "external/tinyxml" }

	-- tinyxml
	files { "external/tinyxml/*.h", "external/tinyxml/*.cpp"}
  files { "src/**.h", "src/**.cc" }
	removefiles { "external/tinyxml/echo.cpp", "external/tinyxml/xmltest.cpp" }
	
	buildoptions { "-DTIXML_USE_STL" }

	-- curl is not present by default for win, for a x64 OS, do this
	-- build it manually first using a Developer CMD for VS; set target vcvarsall x64
	-- goto source root and run buildconf.bat
	-- goto winbuild and run nmake /f Makefile.vc mode=static MACHINE=x64 ENABLE_UNICODE=yes
	-- follow CURL documentation for more
	filter "system:windows"
		includedirs {"external/curl/builds/libcurl*/include"}
		libdirs { "external/curl/builds/libcurl*/lib" }
		links { "libcurl_a.lib" , "wldap32.lib", "Crypt32.lib" , "ws2_32.lib", "winmm.lib", "Normaliz.lib" }
		ignoredefaultlibraries { "MSVCRT" }
		buildoptions { "-DCURL_STATICLIB"}
	filter {}
	-- link dynamically from system package manager
	filter "system:linux or system:macosx"
  	links { "curl" }
	filter {}

  filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter {}

  filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
