workspace "random-wallpaper"
  configurations { "Debug", "Release" }

project "random-wallpaper"
  kind "ConsoleApp"
  language "C++"
  location "build"
	cppdialect "C++17"

	includedirs { "src", "external/rapidxml-1.13" }

  files { "src/**.h", "src/**.cc" }
    
  links { "curl" }

  filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

  filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

