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

set "SDKName=Downpour/IWBHT_SDK"
set "BuildTargetFolder=.\targets\%1"
set "ExtraCXXFlags=-O2 -fno-exceptions -Wall" 
set "NumSpawn=4" 
set "Debug=1"

set "Color_Red=0c"
set "Color_Default=0f"

set "Bool_GCCFound=0"
set "Bool_MakeFound=0"
set "String_Make=love" 
rem set "String_Strip=per" 
set "ResetERRORLEVEL=verify > nul"

set "String_GCCNotFound_1=gcc/g++ is required for building %SDKName%."
set "String_GCCNotFound_2=Have you tried setting up your build environment before attempting to build %SDKName%?"

set "String_MakeNotFound_1=make is required for generating Makefiles which will then be used for building %SDKName%." 
rem set "String_StripNotFound=strip cannot be found. I won't be stripping symbols from executables." 

call :Debug "BuildTargetFolder = %BuildTargetFolder%" 

where /q "gcc"
set "__gcc=%errorlevel%"
call :Debug "__gcc =  %__gcc%"
%ResetERRORLEVEL%

where /q "g++"  
set "__g++=%errorlevel%" 
call :Debug "__g++ = %__g++%"
%ResetERRORLEVEL%

if %__gcc% == 0 ( if %__g++% == 0 ( set "Bool_GCCFound=1" ) )

where /q "make" 
if %errorlevel% == 0 (
    call :Debug "Found 'make'."
    set "Bool_MakeFound=1"
    set "String_Make=make -j%NumSpawn% "
)
%ResetERRORLEVEL% 

if %Bool_MakeFound% == 0 (
    call :Debug "The 'make' identifier cannot be found. Will be searching for mingw32-make instead."
    where /q "mingw32-make"
    if %errorlevel% == 0 (
        call :Debug "Found 'mingw32-make'."
        set "Bool_MakeFound=1"
        set "String_Make=mingw32-make -j%NumSpawn% "
    ) else (
        call :Debug "'mingw32-make' also cannot be found. Bool_MakeFound = %Bool_MakeFound%."
    )
)

rem where /q "strip"
rem if %errorlevel% == 0 (
rem     call :Debug "Found 'strip'."
rem     set "Bool_StripFound=1"
rem     set "String_Strip=strip"
rem ) else (
rem     call :Debug "'strip' cannot be found."
rem ) 

%ResetERRORLEVEL%

if %Bool_MakeFound% == 0 (
    call :F_PrintTextPause %Color_Red% "%String_MakeNotFound_1%" "%String_GCCNotFound_2%" 
    exit /b 0 
) 

if %Bool_GCCFound% == 0 (
    call :F_PrintTextPause %Color_Red% "%String_GCCNotFound_1%" "%String_GCCNotFound_2%" 
    exit /b 0 
) 

rem if %Bool_StripFound% == 0 (
rem     call :F_PrintText %Color_Default% "%String_StripNotFound%" 
rem ) 

call cmake_generic.bat %BuildTargetFolder% -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DFLOCK_SAFE_LUA=1 -DFLOCK_PACKAGING=1 -DFLOCK_IK=1 -DFLOCK_NETWORK=1 -DFLOCK_LOGGING=1 -DFLOCK_PROFILING=1 -DFLOCK_SECURITY=1 -DFLOCK_SECURITY_KEY="MY_SECURITY_STRING" -DFLOCK_SCENE_EDITOR=1 -DEXTRA_CFLAGS=%ExtraCXXFlags% %* 
start /wait /b %String_Make% --directory=%BuildTargetFolder% 

rem if %Bool_StripFound% == 1 (
rem     call :F_PrintText %Color_Default% "Stripping targets..."
rem     %String_Strip% %StripFlags% %BuildTargetFolder%\bin\*.exe 
rem     %String_Strip% %StripFlags% %BuildTargetFolder%\bin\tool\*.exe 
rem )

exit /b 0 

:F_PrintText  
color %1
echo. 
echo ------------------------------------------
:Top 
if (%2) == () (
    goto Bottom 
) else (
    echo %~2 
    shift 
    goto Top
)
:Bottom
echo ------------------------------------------
echo. 
color 
exit /b 0 

:F_PrintTextPause 
call :F_PrintText %*

pause 
exit /b 0  
rem ??? Profit 
rem exit /b 

:Debug 
if %Debug% == 1 (
    echo. 
    echo [Debug Info] 
    echo %*
    echo.
)
exit /b 0 
