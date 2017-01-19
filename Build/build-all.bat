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

rem There was a separate command for launching and waiting for a batch file to exit here: 
rem set "START_DEFERRED=start /wait cmd /c" 
rem But I later realized that the newly created processes don't share the same variable environment with 
rem the host who invoked them, so I switched to plain "call" and it worked. 

if not [%~1] == [] (
    call .\build-shell.bat %* 
)

set /p "BITS_WIDTH=Which of the available targets (32 or 64) you want to build for? " 

if not "%BITS_WIDTH%" == "32" (
    if not "%BITS_WIDTH%" == "64" (
        echo "Invalid target specified. Must be either 32 or 64, no extra (double) quotes, spaces or characters allowed" 
        exit /b 1 
    )
)

call "build-windows-%BITS_WIDTH%-d3d9.bat" 
call "build-windows-%BITS_WIDTH%-d3d11.bat" 
call "build-windows-%BITS_WIDTH%-gl.bat" 
