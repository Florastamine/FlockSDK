# Downpour/IWBHT_SDK 

## Introduction 

`Downpour/IWBHT_SDK` is a fork of the [Urho3D game engine](https://github.com/urho3d/Urho3D) which is being used for the development of my upcoming video game project. It was forked when [Urho3D](https://github.com/urho3d/Urho3D) is `322 commits` ahead of its `1.6` release, and since then the fork has been received patches, modifications, cuts and updates introduced by both the [Urho3D](https://github.com/urho3d/Urho3D) developers and myself. 

The fork follows the same license as [Urho3D](https://github.com/urho3d/Urho3D), and there is no intention of keeping it compatible with the original code. 

## Prebuilt binaries ([release log](https://github.com/Florastamine/IWBHT_SDK/blob/master/CHANGELOG.md))
These prebuilt binaries are provided with the intention of providing a quick way of testing and playing around with the SDK, and thus they are not guaranteed to be stable enough for development purposes. These binaries include both executable files which can be run off straight away or static libraries which can be linked alongside your code. 

Latest **unstable** binaries were built and released on `03.31.2017`, and can be downloaded [here](https://drive.google.com/drive/folders/0Bx0q4f1kFRaieTBhZHJJYzRNRHc?usp=sharing). Older versions can still be downloaded [on the release notes page](https://github.com/Florastamine/IWBHT_SDK/blob/master/CHANGELOG.md). 

## Building 

### On Linux 
It's really easy to build the SDK with Linux. On Windows it takes an additional step to make sure the necessary tools are in `%PATH%` before building. 

You'll need a working installation of `gcc/g++` toolchain and `cmake` (>=`3.2.3`). Pull the source code off GitHub: 
```bash 
git clone https://github.com/Florastamine/IWBHT_SDK && cd ./IWBHT_SDK/Build  
``` 

Type 
```bash 
./build-linux-<target>.sh 
``` 
to initiate the build process, where: 
* target = 32, 64

For example: `build-linux-64.sh`. 

### On Windows 
You'll need a working `MinGW`/`MinGW-W64` and `CMake` (>=`3.2.3`) installation. First off, make sure both of them are correctly installed. Grab the source code and go to the `Build` directory:
```bash 
git clone https://github.com/Florastamine/IWBHT_SDK
cd .\IWBHT_SDK\Build 
```

If both `gcc/g++` and `cmake` are already present in your `%PATH%`, then great! Just invoke the script of your choice to initiate the build process: 

```bash 
.\build-windows-<target>.bat
```

Where: 
* target = 32, 64

For example: `build-windows-64.bat`.

Otherwise, you can use the `build-shell.bat` helper script to set up the build environment: 
```bash 
.\build-shell.bat <path_to_MinGW_root> <path_to_make> <path_to_CMake_root>
``` 

For example:
```bash 
.\build-shell.bat "C:\MinGW32" "C:\MinGW32\bin" "C:\CMake" 
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

Then, you can follow the rest of the build instructions as usual. 
