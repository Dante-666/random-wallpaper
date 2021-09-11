workspace "random-wallpaper"
  configurations { "Debug", "Release" }
  language "C++"
  location "build"
	architecture "x64"

	includedirs { "src", 
								"external/tinyxml", 
								"external/spirit/include", 
								"external/config/include",
								"external/mpl/include",
								"external/preprocessor/include",
								"external/fusion/include",
								"external/type_traits/include",
								"external/core/include",
								"external/static_assert/include",
								"external/utility/include",
								"external/phoenix/include",
								"external/predef/include",
								"external/proto/include",
								"external/bind/include",
								"external/typeof/include",
								"external/variant/include",
								"external/type_index/include",
								"external/container_hash/include",
								"external/throw_exception/include",
								"external/assert/include",
								"external/detail/include",
								"external/move/include",
								"external/integer/include",
								"external/foreach/include",
								"external/range/include",
								"external/iterator/include",
								"external/regex/include",
								"external/optional/include",
								"external/concept_check/include",
								"external/endian/include",
								"external/function/include",
								"external/function_types/include",
								"external/array/include",
								"external/smart_ptr/include",
							}

	filter "configurations:Debug"
	  defines { "DEBUG" }
	  symbols "On"
	  libdirs { "build/bin/Debug" }
	filter {}

	filter "configurations:Release"
	  defines { "NDEBUG" }
	  optimize "On"
	  libdirs { "build/bin/Release" }
	filter {}
	-- curl is not present by default for win, for a x64 OS, do this
    -- mkdir builds && cd builds
    -- cmake -A x64 -DBUILD_SHARED_LIBS=OFF -DCURL_STATIC_CRT=ON -DENABLE_UNICODE=ON ..
    -- cmake --build . --config [Release|Debug]
	filter "system:windows"
	  cppdialect "C++20"
	  includedirs { "external/curl/include", "D:/gtest/include" }
	  libdirs { "external/curl/builds/lib/Release" }
	  links { "libcurl.lib", "wldap32.lib", "Crypt32.lib",
              "ws2_32.lib", "winmm.lib", "Normaliz.lib" }
	  staticruntime "on"
	  buildoptions { "-DCURL_STATICLIB"}
	filter {}
	-- link dynamically from system package manager
	filter "system:linux or system:macosx"
	  --TODO: On Arch this is failing, zoned_time is not defined
	  cppdialect "C++17"
	  links { "curl" }
	filter {}

	filter { "system:windows", "configurations:Debug" }
	  ignoredefaultlibraries { "MSVCRT", "LIBCMT" }
	filter {}

project "random-wallpaper"
  kind "StaticLib"
	-- tinyxml
	files { "external/tinyxml/*.h", "external/tinyxml/*.cpp"}
    -- rwall
    files { "src/**.h", "src/**.cc" }
    filter "system:macosx"
      files { "src/**.mm" }
    filter{}
	removefiles { "src/main.cc" }
	removefiles { "src/tests/*" }
	removefiles { "external/tinyxml/echo.cpp", 
				  "external/tinyxml/xmltest.cpp" }

	buildoptions { "-DTIXML_USE_STL" }

project "rwall"
  kind "ConsoleApp"

	files { "src/main.cc" }
	
	filter "system:windows"
	  links { "random-wallpaper.lib" }
	filter {}
	filter "system:linux or system:macosx"
	  links { "random-wallpaper" }
	filter {}
    filter "system:macosx"
      linkoptions { "-framework Appkit" }
    filter {}

project "tests"
	kind "ConsoleApp"

	files { "src/tests/*.cc" }

	filter { "system:windows" }
	  includedirs {"D:/gtest/include"}
	  libdirs { "D:/gtest/lib" }
	  links { "random-wallpaper" }
	filter {}

	filter { "system:windows", "configurations:Release" }
	  links { "gtest" }
	filter {}
	filter { "system:windows", "configurations:Debug" }
	  links { "gtestd" }
	filter {}

	filter "system:linux or system:macosx"
      links { "gtest", "random-wallpaper" }
	filter {}
    filter "system:linux"
	  links { "pthread" }
	filter {}
    filter "system:macosx"
      linkoptions { "-framework Appkit" }
    filter {}
