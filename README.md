# random-wallpaper

[![Action Status](https://github.com/Dante-666/random-wallpaper/workflows/linux/badge.svg)](https://github.com/Dante-666/random-wallpaper/actions)
[![Action Status](https://github.com/Dante-666/random-wallpaper/workflows/windows/badge.svg)](https://github.com/Dante-666/random-wallpaper/actions)
[![Action Status](https://github.com/Dante-666/random-wallpaper/workflows/mac/badge.svg)](https://github.com/Dante-666/random-wallpaper/actions)

Cycle through wallpapers from configurable source(s)

TODO: Add gifs for linux and windows

## Build Instructions

Right now, there are only three operation systems(x64) which are supported. If you get stuck with any steps, then please have a look at the .github/workflows/{os}.yml for explicit build commands.

Clone the repository and submodules first as a common step.

      git clone --recurse-submodules https://github.com/Dante-666/random-wallpaper

Then run premake for the required platform. Make sure you are using the latest version of premake5 since c++20 is needed for this. For example, on linux run

      premake5 gmake2

### Linux

#### Dependencies

You need these external dependencies to make your life easier: **feh** & **curl** and **google-test**. Depending on the OS's package management, you may have to look for exact names. An example for Arch Linux is given below,

      sudo pacman -Syy feh curl gtest

Curl is included as a submodule for Windows and if you so choose, you can build it locally and link it with the executable.

With premake configured for **gmake2**, run the following command.

      make -j config=release

### Mac

The build steps are exactly the same as linux. If homebrew is setup, we need to install the above dependencies. Using make to build this is more sensible since premake5 doesn't support newer versions of Xcode too well.

TODO: background setter workflow 

### Windows

For windows, first you'll have to build cURL and the best way to do it is via cmake and you can find the build commands in actions workflow for windows.

You need Visual Studio BuildTools to build this and it is shipped with Visual Studio Editor or can be downloaded separately.

Run premake5 with the flag **vs2019** and the build directory contains **random-wallpaper.vcxproj**.

      premake5 vs2019
      MSbuild -p:Configuration=Release build/random-wallpaper.sln

For now, 64 bit is the only supported architecture. I hope everyone is out of the caves.
## Config file
### TODO: add this file
