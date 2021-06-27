workspace "random-wallpaper"
    configurations { "Debug", "Release" }

project "random-wallpaper"
    kind "ConsoleApp"
    language "C++"
    location "build"

    files { "src/**.h", "src/**.cc" }
    
    links { "curl" }

    filter "configurations:Debug"
	defines { "DEBUG" }
	symbols "On"

    filter "configurations:Release"
	defines { "NDEBUG" }
	optimize "On"


