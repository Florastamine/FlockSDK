@echo off
setlocal 

set "OutputFolder=%1"
set "MinGWRoot=%2"
set "GNUMakeRoot=%3"
set "CMakeRoot=%4"

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
) else (
	call .\cmake_mingw.bat "%OutputFolder%" -DWIN32=1 -DURHO3D_64BIT=0 -DURHO3D_ANGELSCRIPT=1 -DURHO3D_LUA=1 -DURHO3D_LUAJIT=1 -DURHO3D_SAFE_LUA=1 -DURHO3D_LUA_RAW_SCRIPT_LOADER=1 -DURHO3D_NETWORK=1 -DURHO3D_SAMPLES=0 -DURHO3D_TOOLS=1 -DURHO3D_EXTRAS=1 -DURHO3D_DATABASE_SQLITE=1 -DURHO3D_MINIDUMPS=0 -DURHO3D_PACKAGING=0 -DURHO3D_PROFILING=0 -DURHO3D_OPENGL=1 -DURHO3D_WIN32_CONSOLE=1 
)

if %Bool_MakeFound% == 0 (
	echo %String_TestGNUMake_Fail% 
) else (
	%String_GNUMake% --directory=%OutputFolder%
)

endlocal
