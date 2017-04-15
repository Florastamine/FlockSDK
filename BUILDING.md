# Building 

## On Linux 
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

## On Windows 
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
