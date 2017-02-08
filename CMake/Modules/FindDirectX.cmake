#
# Copyright (c) 2008-2016 the Urho3D project.
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

# For MSVC compiler, find Microsoft DirectX installation in Windows SDK or in June 2010 DirectX SDK or later
# For MinGW compiler, assume MinGW not only comes with the necessary headers & libraries but also has the headers & libraries directories in its default search path
# (use 'echo |$MINGW_PREFIX-gcc -v -E -' and '$MINGW_PREFIX-gcc -print-search-dirs', respectively, to double check)
# The MinGW headers on Linux host are usually shipped in a development package which should be installed automatically as part of the package dependency by the package manager when installing MinGW
# (in Debian-based it is 'mingw-w64-dev' and in RedHat-based 'mingw64-headers' for 64-bit; 'mingw-w64-i686-dev' and 'mingw32-headers', respectively, for 32-bit)
#
#  DIRECTX_FOUND (TRUE when any of the components is found)
#  DIRECTX_INCLUDE_DIRS
#  DIRECTX_LIBRARY_DIRS
#  HAVE_DIRECTX (Synonym to DIRECTX_FOUND)
#  USE_WINSDK_DIRECTX (TRUE when using Windows SDK or FALSE when using DirectX SDK; not defined when using MinGW)
#
# When corresponding component is being searched for and found:
#  DirectX_DInput_FOUND
#  DirectX_DSound_FOUND
#  DirectX_XAudio2_FOUND
#  DirectX_XInput_FOUND
#
# When corresponding header listed below is found:
#  HAVE_<UPCASE_NAME>_H
#  HAVE_D3D_H (Currently synonym to HAVE_D3D9_H)
#

set (DIRECTX_HEADERS dsound.h dinput.h dxgi.h xaudio2.h xinput.h)

# Optional input variables (see corresponding code comments for details):
#  DIRECTX_INC_SEARCH_PATHS
#  DIRECTX_LIB_SEARCH_PATHS
#

if (CMAKE_CL_64)
    set (PATH_SUFFIX x64)
else ()
    set (PATH_SUFFIX x86)
endif ()
set (CMAKE_REQUIRED_INCLUDES_SAVED ${CMAKE_REQUIRED_INCLUDES})

# Version older than VS2012 fallbacks to search for the DirectX SDK
if (NOT MSVC_VERSION GREATER 1600 OR MINGW)     # MinGW reuses the logic below to check for the existence of DirectX headers and libraries
    if (MINGW)
        if (NOT MINGW_SYSROOT)
            if (DEFINED ENV{MINGW_SYSROOT})
                file (TO_CMAKE_PATH $ENV{MINGW_SYSROOT} MINGW_SYSROOT)
            else ()
                execute_process (COMMAND ${CMAKE_COMMAND} -E echo "#include <_mingw.h>" COMMAND ${CMAKE_C_COMPILER} -E -M - OUTPUT_FILE ${CMAKE_BINARY_DIR}/find_mingw_sysroot_output ERROR_QUIET)
                file (STRINGS ${CMAKE_BINARY_DIR}/find_mingw_sysroot_output MINGW_SYSROOT REGEX _mingw\\.h)
                string (REGEX REPLACE "^[^ ]* *(.*)/include.*$" \\1 MINGW_SYSROOT "${MINGW_SYSROOT}")  # Stringify for string replacement
                string (REPLACE "\\ " " " MINGW_SYSROOT "${MINGW_SYSROOT}")
                execute_process (COMMAND ${CMAKE_COMMAND} -E remove find_mingw_sysroot_output)
            endif ()
            if (NOT EXISTS ${MINGW_SYSROOT})
                message (FATAL_ERROR "Could not find MinGW system root. "
                    "Use MINGW_SYSROOT environment variable or build option to specify the location of system root.")
            endif ()
            set (MINGW_SYSROOT ${MINGW_SYSROOT} CACHE PATH "Path to MinGW system root (MinGW build only); should only be used when the system root could not be auto-detected" FORCE)
        endif ()
        # MinGW Cross-compiling uses CMAKE_FIND_ROOT_PATH while MinGW on Windows host uses CMAKE_PREFIX_PATH
        if (CMAKE_HOST_WIN32)
            set (CMAKE_PREFIX_PATH ${MINGW_SYSROOT})
        endif ()
        # MinGW does not need search paths as DirectX headers and libraries (when installed) are in its default search path
        # However, we do not explicitly unset the DIRECTX_INC_SEARCH_PATHS and DIRECTX_LIB_SEARCH_PATHS variables here, so module user could set these two variables externally when for some reasons the DirectX headers and libraries are not installed in MinGW default search path
    else ()
        list (APPEND DIRECTX_INC_SEARCH_PATHS
            "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
            "C:/Program Files/Microsoft DirectX SDK (June 2010)/Include"
            $ENV{DIRECTX_ROOT}/Include
            $ENV{DXSDK_DIR}/Include)
        list (APPEND DIRECTX_LIB_SEARCH_PATHS
            "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/${PATH_SUFFIX}"
            "C:/Program Files/Microsoft DirectX SDK (June 2010)/Lib/${PATH_SUFFIX}"
            $ENV{DIRECTX_ROOT}/Lib/${PATH_SUFFIX}
            $ENV{DXSDK_DIR}/Lib/${PATH_SUFFIX})
        if (NOT CMAKE_CL_64)
            list (APPEND DIRECTX_LIB_SEARCH_PATHS
                "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib"
                "C:/Program Files/Microsoft DirectX SDK (June 2010)/Lib"
                $ENV{DIRECTX_ROOT}/Lib
                $ENV{DXSDK_DIR}/Lib)
        endif ()
    endif ()
    find_path (DIRECTX_INCLUDE_DIRS NAMES ${DIRECTX_HEADERS} PATHS ${DIRECTX_INC_SEARCH_PATHS} DOC "DirectX include directory")
    if (MINGW)
        set (FAIL_MESSAGE "Could NOT find DirectX using MinGW default search paths")
    else ()
        set (FAIL_MESSAGE "Could NOT find DirectX using DirectX SDK default search paths")
    endif ()
    mark_as_advanced (DIRECTX_INCLUDE_DIRS)
endif ()

# For now take shortcut for the other DirectX components by just checking on the headers and not the libraries
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
foreach (NAME ${DIRECTX_HEADERS})
    string (REPLACE . _ BASE_NAME ${NAME})
    string (TOUPPER ${BASE_NAME} UPCASE_NAME)
    if (NAME STREQUAL xinput.h)
        # Workaround an issue in finding xinput.h using check_include_file() as it depends on windows.h but not included it by itself in WinSDK
        check_include_files (windows.h\;${NAME} HAVE_${UPCASE_NAME})
    else ()
        check_include_file_cxx (${NAME} HAVE_${UPCASE_NAME})
    endif ()
endforeach ()
foreach (COMPONENT DInput DSound XAudio2 XInput)
    string (TOUPPER ${COMPONENT} UPCASE_NAME)
    if (HAVE_${UPCASE_NAME}_H)
        set (DirectX_${COMPONENT}_FOUND TRUE)
        set (HAVE_DIRECTX TRUE)
    endif ()
endforeach ()
set (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES_SAVED})

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (DirectX REQUIRED_VARS HAVE_DIRECTX HANDLE_COMPONENTS FAIL_MESSAGE ${FAIL_MESSAGE})
