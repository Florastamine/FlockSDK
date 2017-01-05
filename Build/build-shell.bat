@echo off 

rem Copyright (c) 2016, Florastamine
rem 
rem Permission is hereby granted, free of charge, to any person obtaining a copy of
rem this software and associated documentation files (the "Software"), to deal in
rem the Software without restriction, including without limitation the rights to
rem use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
rem of the Software, and to permit persons to whom the Software is furnished to do
rem so, subject to the following conditions:
rem 
rem The above copyright notice and this permission notice shall be included in all
rem copies or substantial portions of the Software.
rem 
rem THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
rem IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
rem FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
rem AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
rem WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
rem IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
rem 
rem 
rem 
rem This simple script sets up the environment for building the Downpour/IWBHT_SDK toolset. 
rem It's not really needed as you can just simply supply the paths to gcc/g++, make, cmake, strip, ar and 
rem some other GNU tools required for building IWBHT_SDK, it's just there to avoid the repetition of having to 
rem type in the paths into your current environment whenever you want to build the toolset. 
rem 
rem Usage: build-shell.bat <path_to_MinGW> <path_to_make> <path_to_CMake> 
rem 
rem After the build environment have been successfully set up, you can simply type 
rem     build-<platform>-<target>-<renderer>.bat 
rem to start the building process, where: 
rem platform = 
rem     windows 
rem     linux 
rem target = 
rem     32 
rem     64 
rem renderer = 
rem     d3d9
rem     d3d11 
rem     gl 
rem For example: build-linux-64-gl.bat 

title Downpour/IWBHT_SDK environment shell  

set "MinGWRoot=%1"
set "GNUMakeRoot=%2"
set "CMakeRoot=%3"

set "String_TestGNUC=[ ] Testing gcc/g++..."
set "String_TestGNUC_Pass=[X] Testing gcc/g++..."
set "String_TestGNUC_Fail=gcc/g++ could not be found!"
set "Bool_TestGNUC=0"

set "String_TestGNUMake=[ ] Testing GNU make..."
set "String_TestGNUMake_Pass=[X] Testing GNU make..."
set "String_TestGNUMake_Fail=GNU make could not be found!"
set "Bool_MakeFound=0"

set "String_TestCMake=[ ] Testing CMake..."
set "String_TestCMake_Pass=[X] Testing CMake..."
set "String_TestCMake_Fail=CMake could not be found!"
set "Bool_CMakeFound=0" 

set "String_AllChecksPassed=All checks passed!"

set "PATH=%PATH%;%MinGWRoot%;%MinGWRoot%\bin;%GNUMakeRoot%;%CMakeRoot%;%CMakeRoot%\bin"

echo %String_TestGNUC%
if exist "%MinGWRoot%\bin\gcc.exe" (
	if exist "%MinGWRoot%\bin\g++.exe" (
		set "Bool_TestGNUC=1"
		echo %String_TestGNUC_Pass% found at [%MinGWRoot%]
	)
) 

echo %String_TestGNUMake% 
if exist "%GNUMakeRoot%\make.exe" (
	set "Bool_MakeFound=1"
	echo %String_TestGNUMake_Pass% found at [%GNUMakeRoot%]
) else (
	if exist "%GNUMakeRoot%\mingw32-make.exe" (
		set "Bool_MakeFound=1"
		echo %String_TestGNUMake_Pass% found at [%GNUMakeRoot%]
	)
)

echo %String_TestCMake%
if exist "%CMakeRoot%\bin\cmake.exe" (
	set "Bool_CMakeFound=1"
	echo %String_TestCMake_Pass% found at [%CMakeRoot%]
)

if %Bool_TestGNUC% == 0 ( echo %String_TestGNUC_Fail% ) 
if %Bool_CMakeFound% == 0 ( echo %String_TestCMake_Fail% )
if %Bool_MakeFound% == 0 ( echo %String_TestGNUMake_Fail% )

if %Bool_TestGNUC% == 1 (
	if %Bool_MakeFound% == 1 (
		if %Bool_CMakeFound% == 1 (
			echo %String_AllChecksPassed% 
		)
	)
)
