workspace "random-wallpaper"
  configurations { "Debug", "Release" }

project "random-wallpaper"
  kind "ConsoleApp"
  language "C++"
  location "build"
	cppdialect "C++17"

	includedirs { "src", "external/tinyxml" }

	-- tinyxml
	files { "external/tinyxml/*.h", "external/tinyxml/*.cpp"}
  files { "src/**.h", "src/**.cc" }
	removefiles { "external/tinyxml/echo.cpp", "external/tinyxml/xmltest.cpp" }
    
  links { "curl" }
	buildoptions { "-DTIXML_USE_STL" }

  filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
	filter {}

  filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

