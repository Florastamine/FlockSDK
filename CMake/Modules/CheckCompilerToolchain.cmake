#
# Copyright (c) 2008-2017 Flock SDK developers & contributors. 
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Check the chosen compiler toolchain in the build tree
#
# Native ABI:
#  NATIVE_64BIT 
#
# Compiler version in major.minor.patch format. 
# 
# CPU SIMD instruction extensions support:
#  HAVE_MMX
#  HAVE_3DNOW
#  HAVE_SSE
#  HAVE_SSE2
#  HAVE_ALTIVEC
#

macro (check_native_define REGEX OUTPUT_VAR)
    if (NOT DEFINED ${OUTPUT_VAR})
        string (REGEX MATCH "#define +${REGEX} +([^;]+)" matched "${NATIVE_PREDEFINED_MACROS}")
        if (matched)
            string (REGEX MATCH "\\(.*\\)" captured "${REGEX}")
            if (captured)
                set (GROUP 2)
            else ()
                set (GROUP 1)
            endif ()
            string (REGEX REPLACE "#define +${REGEX} +([^;]+)" \\${GROUP} matched "${matched}")
            set (${OUTPUT_VAR} ${matched})
        else ()
            set (${OUTPUT_VAR} 0)
        endif ()
        set (${OUTPUT_VAR} ${${OUTPUT_VAR}} CACHE INTERNAL "Compiler toolchain has predefined macros matching ${REGEX}")
    endif ()
endmacro ()

# Determine the native ABI based on the size of pointer
check_native_define (__SIZEOF_POINTER__ SIZEOF_POINTER)
if (SIZEOF_POINTER EQUAL 8)
    set (NATIVE_64BIT 1)
endif ()
# GCC/Clang and all their derivatives should understand this command line option to get the compiler version
if (NOT DEFINED COMPILER_VERSION)
    execute_process (COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE COMPILER_VERSION ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    set (COMPILER_VERSION ${COMPILER_VERSION} CACHE INTERNAL "GCC/Clang Compiler version")
endif ()

macro (check_extension CPU_INSTRUCTION_EXTENSION)
    string (TOUPPER "${CPU_INSTRUCTION_EXTENSION}" UCASE_EXT_NAME)   # Stringify to guard against empty variable
    if (NOT DEFINED HAVE_${UCASE_EXT_NAME})
        execute_process (COMMAND ${CMAKE_C_COMPILER} -m${CPU_INSTRUCTION_EXTENSION} -E -dM -xc ${NULL_DEVICE${EMCC_FIX}} RESULT_VARIABLE CC_EXIT_STATUS OUTPUT_VARIABLE PREDEFINED_MACROS ERROR_QUIET)
        if (NOT CC_EXIT_STATUS EQUAL 0)
            message (FATAL_ERROR "Could not check compiler toolchain CPU instruction extension as it does not handle '-E -dM' compiler flags correctly")
        endif ()
        if (NOT ${ARGN} STREQUAL "")
            set (EXPECTED_MACRO ${ARGN})
        else ()
            set (EXPECTED_MACRO __${UCASE_EXT_NAME}__)
        endif ()
        string (REGEX MATCH "#define +${EXPECTED_MACRO} +1" matched "${PREDEFINED_MACROS}")
        if (matched)
            set (matched 1)
        else ()
            set (matched 0)
        endif ()
        set (HAVE_${UCASE_EXT_NAME} ${matched} CACHE INTERNAL "Compiler toolchain supports ${UCASE_EXT_NAME} CPU instruction extension")
    endif ()
endmacro ()

if (MINGW AND COMPILER_VERSION VERSION_LESS 4.9.1)
    if (NOT DEFINED FLOCK_SSE)     # Only give the warning once during initial configuration
        # Certain MinGW versions fail to compile SSE code. This is the initial guess for known "bad" version range, and can be tightened later
        message (WARNING "Disabling SSE by default due to MinGW version. It is recommended to upgrade to MinGW with GCC >= 4.9.1. You can also try to re-enable SSE with CMake option -DFLOCK_SSE=1, but this may result in compile errors.")
    endif ()
elseif (NOT EMSCRIPTEN)     # Emscripten does not support SSE/SSE2 (yet) now but erroneously responding positively to our probe, so skip them for Emscripten for now
    check_extension (sse)
    check_extension (sse2)
endif ()
if (NOT WIN32)    # Linux only
    check_extension (mmx)
    check_extension (3dnow __3dNOW__)
endif ()

# Explicitly set the variable to 1 when it is defined and truthy or 0 when it is not defined or falsy
foreach (VAR NATIVE_64BIT HAVE_MMX HAVE_3DNOW HAVE_SSE HAVE_SSE2 HAVE_ALTIVEC)
    if (${VAR})
        set (${VAR} 1)
    else ()
        set (${VAR} 0)
    endif ()
endforeach ()
