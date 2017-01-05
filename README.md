# Downpour/IWBHT_SDK 

## Introduction 

This is a fork of the [Urho3D game engine](https://github.com/urho3d/Urho3D) which is being used for the development of my upcoming video game project. It was forked when [Urho3D](https://github.com/urho3d/Urho3D) is `322 commits` ahead of its `1.6` release, and since then the fork has been received patches, modifications, cuts and updates introduced by both the [Urho3D](https://github.com/urho3d/Urho3D) developers and myself. 

A list of modifications, cuts, and updates since the fork was born: 
* Rest in pieces `ARM`, `PowerPC`, `Raspberry Pi`, `Android`, `iOS`, `OS X`, `tvOS`, `Xcode`, `Haiku` and `Web` targets. This fork puts heavy emphasises on `Windows` and `Linux` targets. 
* Modifications to the both the game engine and the `AngelScript`-based editor to use a different folder naming and structure. 
* A collection of build scripts for compiling the SDK with custom settings related to the game project. 
* Reworked `Urho3DPlayer`. 
* Integration with [Herbal](https://github.com/Florastamine/Herbal), a Lua- and GUI-based build target configuration tool.  
* Forbids generation of documentation files using [Doxygen](http://www.stack.nl/~dimitri/doxygen/).  

The fork follows the same license as [Urho3D](https://github.com/urho3d/Urho3D), and there is no intention of keeping it compatible with the original code. 

## Building 
### On Windows 
You'll need a working `MinGW`/`MinGW-W64` and `CMake` (>=`3.2.3`) installation. First off, make sure both of them are correctly installed. Grab the source code and go to the `Build` directory:
```bash 
git clone https://github.com/Florastamine/IWBHT_SDK
cd .\Build 
```

Set up the build environment by making `gcc/g++` and `cmake` available in your `PATH`: 
```bash 
.\build-shell.bat <path_to_MinGW_root> <path_to_make> <path_to_CMake_root>
``` 

...or you can just simply do it by hand: 
```bash 
set "PATH=%PATH%;C:\MinGW32\bin;C:\CMake\bin"
```

For example:
```bash 
.\build-shell.bat C:\MinGW32 C:\MinGW32\bin C:\CMake 
``` 

You should see something like this: 
```bash 
[ ] Testing gcc/g++...
[X] Testing gcc/g++... found at [C:\MinGW32]
[ ] Testing GNU make... 
[X] Testing GNU make... found at [C:\MinGW32\bin]
[ ] Testing CMake...
[X] Testing CMake... found at [C:\CMake]
All checks passed! 
```

If not, take a look at the folders you've just specified to make sure `gcc/g++` and `cmake` executables are there. 
Invoke the script of your choice to initiate the build process: 

```bash 
.\build-<platform>-<target>-<renderer>.bat
```

Where: 
* platform = windows, linux 
* target = 32, 64
* renderer = d3d9, d3d11, gl 

For example: `build-windows-64-d3d11.bat`.
