# random-wallpaper

![Build](https://github.com/github/docs/actions/workflows/main.yml/badge.svg)

Cycle through wallpapers from configurable source(s)

TODO: Add gifs for linux and windows

## Build Instructions

Right now, there are only three operation systems(x64) which are supported. If you get stuck with any steps, then please have a look at the .github/workflows/main.yml for explicit build commands.

Clone the repository and submodules first as a common step.

      git clone --recurse-submodules https://github.com/Dante-666/random-wallpaper

Then run premake for the required platform. For example, on linux run

      premake5 gmake2

### Linux

#### Dependencies

You need these external dependencies to make your life easier: **feh** & **curl**. Depending on the OS's package management, you may have to look for exact names. An example for Arch Linux is given below,

      sudo pacman -Syy feh curl

Curl is included as a submodule for Windows and if you so choose, you can build it locally and link it with the executable.

With premake configured for **gmake2**, run the following command.

      make -j config=release

### Mac

The build steps are exactly the same as linux.

TODO: background setter workflow 

### Windows

For windows, first you'll have to build cURL and the simplest way to do that is by following the guide at **external/curl/winbuild/README.md**.

You need Visual Studio BuildTools to build this and it is shipped with Visual Studio Editor or can be downloaded separately.

Assuming that premake was run with the flag **vs2019** and the build directory contains **random-wallpaper.vcxproj**.

      MSbuild -m build/random-wallpaper.vcxproj -property:Configuration=Release

## Config file
### TODO: add this file