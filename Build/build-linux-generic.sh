#!/bin/bash 
# Copyright (c) 2016, Florastamine
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
# IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 

source "build-common.sh" 

SDKName="Downpour/IWBHT_SDK"
BuildTargetFolder="./targets"
ExtraCXXFlags="-O2 -fno-exceptions -Wall" 
StripFlags="-s -R .gnu.version -R .comment"
NumSpawn="4" 

if [ -z "$1" ]; then 
    __f0__=${0##*/} 
    BuildTargetFolder="$BuildTargetFolder/${__f0__%.*}" 
else 
    BuildTargetFolder="$BuildTargetFolder/$1" 
fi 

PrintDebug "SDKName=$SDKName" "BuildTargetFolder=$BuildTargetFolder" "ExtraCXXFlags=$ExtraCXXFlags" "StripFlags=$StripFlags" "NumSpawn=$NumSpawn" 

function CheckToolPrint() {
    # Count how many arguments we have passed into the function. 
    i=$((0)) 
    for j in "$@" # Because i was reserved. 
    do 
        i=$(($i+1))
    done 

    k=$((1)) 
    Print42 
    for j in "$@" 
    do 
        printf "%s" "($k/$i) Checking for $j..."
        k=$(($k+1))
        if CheckTool "$j"; then 
            printf "%s\n" "found $j." 
        else 
            printf "%s\n" "$j could not be found! $j is **required** for building $SDKName." 
            exit 1 
        fi 
    done 
    Print42 
} 

# Check if the tools are installed. 
CheckToolPrint "cmake" "make" "gcc" "g++" "strip" 

# If yes, then begin generating build files and place it into $BuildTargetFolder. 
./cmake_generic.sh ./$BuildTargetFolder -DWIN32=0 -DFLOCK_SAFE_LUA=1 -DFLOCK_LUA_RAW_SCRIPT_LOADER=1 -DFLOCK_TOOLS=1 -DFLOCK_EXTRAS=1 -DFLOCK_PACKAGING=1 -DEXTRA_CFLAGS=$ExtraCXXFlags $* 

# Build the project. 
make -j$NumSpawn --directory=./$BuildTargetFolder 
