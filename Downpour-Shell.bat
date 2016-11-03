@echo off 
title Downpour MinGW + GNU make + CMake Shell 

set "MinGWRoot=%1"
set "GNUMakeRoot=%2"
set "CMakeRoot=%3"

set "String_TestGNUC=[ ] Testing gcc/g++..."
set "String_TestGNUC_Pass=[X] Testing gcc/g++..."
set "String_TestGNUC_Fail=gcc/g++ couldn't be found!"
set "Bool_TestGNUC=0"

set "String_TestGNUMake=[ ] Testing GNU make..."
set "String_TestGNUMake_Pass=[X] Testing GNU make..."
set "String_TestGNUMake_Fail=GNU make couldn't be found!"
set "Bool_MakeFound=0"
set "String_GNUMake="

set "String_TestCMake=[ ] Testing CMake..."
set "String_TestCMake_Pass=[X] Testing CMake..."
set "String_TestCMake_Fail=CMake couldn't be found!"
set "Bool_CMakeFound=0"

set "PATH=%PATH%;%MinGWRoot%;%MinGWRoot%\bin;%GNUMakeRoot%;%CMakeRoot%;%CMakeRoot%\bin"

echo %String_TestGNUC%
if exist "%MinGWRoot%\bin\gcc.exe" (
	if exist "%MinGWRoot%\bin\g++.exe" (
		echo %String_TestGNUC_Pass%
		set "Bool_TestGNUC=1"
	)
) 

echo %String_TestGNUMake% 
if exist "%GNUMakeRoot%\make.exe" (
	set "String_GNUMake=%GNUMakeRoot%\make.exe"
	set "Bool_MakeFound=1"
	echo %String_TestGNUMake_Pass% found at %String_GNUMake%
) else (
	if exist "%GNUMakeRoot%\mingw32-make.exe" (
		set "String_GNUMake=%GNUMakeRoot%\mingw32-make.exe"
		set "Bool_MakeFound=1"
		echo %String_TestGNUMake_Pass% found at %String_GNUMake% 
	)
)

echo %String_TestCMake%
if exist "%CMakeRoot%\bin\cmake.exe" (
	set "Bool_CMakeFound=1"
	echo %String_TestCMake_Pass% found at %CMakeRoot%\bin\cmake.exe
)

if %Bool_TestGNUC% == 0 (
	echo %String_TestGNUC_Fail%
) 

if %Bool_CMakeFound% == 0 (
	echo %String_TestCMake_Fail% 
)

if %Bool_MakeFound% == 0 (
	echo %String_TestGNUMake_Fail% 
)
