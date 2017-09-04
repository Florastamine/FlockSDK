@echo off

if "%~1" == "%~2" (
    echo Empty/wrong arguments passed. Type LuaScriptMinifier.bat --help for an example on how  to use the batch script.
) else (
    if "%~1" == "--help" (
        echo LuaScriptMinifier.bat "pfiles\main.lua" "pfiles\main_.lua"
    ) else (
        bin\lua scripts\lua-minifier\LuaSrcDiet.lua --maximum --details ..\%~1 -o ..\%~2
    )
)
