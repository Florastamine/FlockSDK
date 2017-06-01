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

# Find Flock include directories and libraries in the Flock SDK installation or build tree
# This module should be able to find Flock automatically when the SDK is installed in a system-wide default location
# If the SDK installation location is non-default or the Flock library is not installed at all (i.e. still in its build tree) then
#   use FLOCK_HOME environment variable or build option to specify the location of the non-default SDK installation or build tree
# When setting FLOCK_HOME variable, it should be set to a parent directory containing both the "include" and "lib" subdirectories
#   e.g. set FLOCK_HOME=/home/john/usr/local, if the SDK is installed using DESTDIR=/home/john and CMAKE_INSTALL_PREFIX=/usr/local

#
#  FLOCK_FOUND
#  FLOCK_INCLUDE_DIRS
#  FLOCK_LIBRARIES
#  FLOCK_VERSION
#  FLOCK_64BIT (may be used as input variable for multilib-capable compilers; must be anyway specified as input variable for MSVC due to CMake/VS generator limitation)
#  FLOCK_LIB_TYPE (may be used as input variable as well to limit the search of library type)
#  FLOCK_SSE
#  FLOCK_LUAJIT
#
# WIN32 only:
#  FLOCK_LIBRARIES_REL
#  FLOCK_LIBRARIES_DBG
#  FLOCK_DLL
#  FLOCK_DLL_REL
#  FLOCK_DLL_DBG
#

set (AUTO_DISCOVER_VARS FLOCK_SSE)
set (PATH_SUFFIX Flock)
if (CMAKE_PROJECT_NAME STREQUAL Flock AND TARGET Flock)
    # A special case where library location is already known to be in the build tree of Flock project
    set (FLOCK_HOME ${CMAKE_BINARY_DIR})
    set (FLOCK_INCLUDE_DIRS ${FLOCK_HOME}/include ${FLOCK_HOME}/include/Flock/ThirdParty)

    # Bullet library depends on its own include dir to be added in the header search path
    # This is more practical than patching its header files in many places to make them work with relative path
    list (APPEND FLOCK_INCLUDE_DIRS ${FLOCK_HOME}/include/Flock/ThirdParty/Bullet)

    if (FLOCK_LUA)
        # ditto for Lua/LuaJIT
        list (APPEND FLOCK_INCLUDE_DIRS ${FLOCK_HOME}/include/Flock/ThirdParty/Lua${JIT})
    endif ()
    set (FLOCK_LIBRARIES Flock)
    set (FOUND_MESSAGE "Found Flock: as CMake target")
    set (FLOCK_COMPILE_RESULT TRUE)
else ()
    # Library location would be searched (based on FLOCK_HOME variable if provided and in system-wide default location)
    if (NOT FLOCK_HOME AND DEFINED ENV{FLOCK_HOME})
        file (TO_CMAKE_PATH "$ENV{FLOCK_HOME}" FLOCK_HOME)
    endif ()
    # Convert to integer literal to match it with our internal cache representation; it also will be used as foreach loop control variable
    if (FLOCK_64BIT)
        set (FLOCK_64BIT 1)
    else ()
        set (FLOCK_64BIT 0)
    endif ()
    # If either of the FLOCK_64BIT or FLOCK_LIB_TYPE or FLOCK_HOME build options changes then invalidate all the caches
    if (NOT FLOCK_64BIT EQUAL FLOCK_FOUND_64BIT OR NOT FLOCK_LIB_TYPE STREQUAL FLOCK_FOUND_LIB_TYPE OR NOT FLOCK_BASE_INCLUDE_DIR MATCHES "^${FLOCK_HOME}/include/Flock$")
        unset (FLOCK_BASE_INCLUDE_DIR CACHE)
        unset (FLOCK_LIBRARIES CACHE)
        unset (FLOCK_FOUND_64BIT CACHE)
        unset (FLOCK_FOUND_LIB_TYPE CACHE)
        unset (FLOCK_COMPILE_RESULT CACHE)
        if (WIN32)
            unset (FLOCK_LIBRARIES_DBG CACHE)
            unset (FLOCK_DLL_REL CACHE)
            unset (FLOCK_DLL_DBG CACHE)
        endif ()
        # Flock prefers static library type by default while CMake prefers shared one, so we need to change CMake preference to agree with Flock
        set (CMAKE_FIND_LIBRARY_SUFFIXES_SAVED ${CMAKE_FIND_LIBRARY_SUFFIXES})
        if (NOT CMAKE_FIND_LIBRARY_SUFFIXES MATCHES ^\\.\(a|lib\))
            list (REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
        endif ()
        # If library type is specified then only search for the requested library type
        if (NOT MSVC AND FLOCK_LIB_TYPE)      # MSVC static lib and import lib have a same extension, so cannot use it for searches
            if (FLOCK_LIB_TYPE STREQUAL STATIC)
                set (CMAKE_FIND_LIBRARY_SUFFIXES .a)
            elseif (FLOCK_LIB_TYPE STREQUAL SHARED)
                if (MINGW)
                    set (CMAKE_FIND_LIBRARY_SUFFIXES .dll.a)
                else ()
                    set (CMAKE_FIND_LIBRARY_SUFFIXES .so)
                endif ()
            else ()
                message (FATAL_ERROR "Library type: '${FLOCK_LIB_TYPE}' is not supported")
            endif ()
        endif ()
        # The PATH_SUFFIX does not work for CMake on Windows host system, it actually needs a prefix instead
        if (CMAKE_HOST_WIN32)
            set (CMAKE_SYSTEM_PREFIX_PATH_SAVED ${CMAKE_SYSTEM_PREFIX_PATH})
            string (REPLACE ";" "\\Flock;" CMAKE_SYSTEM_PREFIX_PATH "${CMAKE_SYSTEM_PREFIX_PATH_SAVED};")    # Stringify for string replacement
            if (NOT FLOCK_64BIT)
                list (REVERSE CMAKE_SYSTEM_PREFIX_PATH)
            endif ()
        endif ()
    endif ()
    # FLOCK_HOME variable should be an absolute path, so use a non-rooted search even when we are cross-compiling
    if (FLOCK_HOME)
        list (APPEND CMAKE_PREFIX_PATH ${FLOCK_HOME})
        set (SEARCH_OPT NO_CMAKE_FIND_ROOT_PATH)
    endif ()
    find_path (FLOCK_BASE_INCLUDE_DIR Flock.h PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT} DOC "Flock include directory")
    if (FLOCK_BASE_INCLUDE_DIR)
        get_filename_component (FLOCK_INCLUDE_DIRS ${FLOCK_BASE_INCLUDE_DIR} PATH)
        if (NOT FLOCK_HOME)
            # FLOCK_HOME is not set when using SDK installed on system-wide default location, so set it now
            get_filename_component (FLOCK_HOME ${FLOCK_INCLUDE_DIRS} PATH)
        endif ()
        list (APPEND FLOCK_INCLUDE_DIRS ${FLOCK_BASE_INCLUDE_DIR}/ThirdParty)
        list (APPEND FLOCK_INCLUDE_DIRS ${FLOCK_BASE_INCLUDE_DIR}/ThirdParty/Bullet)
        if (FLOCK_LUA)
            list (APPEND FLOCK_INCLUDE_DIRS ${FLOCK_BASE_INCLUDE_DIR}/ThirdParty/Lua${JIT})
        endif ()
        # Intentionally do no cache the FLOCK_VERSION as it has potential to change frequently
        file (STRINGS ${FLOCK_BASE_INCLUDE_DIR}/librevision.h FLOCK_VERSION REGEX "^const char\\* revision=\"[^\"]*\".*$")
        string (REGEX REPLACE "^const char\\* revision=\"([^\"]*)\".*$" \\1 FLOCK_VERSION "${FLOCK_VERSION}")      # Stringify to guard against empty variable
    endif ()
    if (FLOCK_64BIT AND MINGW AND CMAKE_CROSSCOMPILING)
        # For 64-bit MinGW on Linux host system, force to search in 'lib64' path even when the Windows platform is not defaulted to use it
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
    endif ()
    set (FLOCK_LIB_TYPE_SAVED ${FLOCK_LIB_TYPE})  # We need this to reset the auto-discovered FLOCK_LIB_TYPE variable before looping
    foreach (ABI_64BIT RANGE ${FLOCK_64BIT} 0)
        # Break if the compiler is not multilib-capable and the ABI is not its native
        if ((MSVC OR ARM) AND NOT ABI_64BIT EQUAL NATIVE_64BIT)
            break ()
        endif ()
        # Set to search in 'lib' or 'lib64' based on the ABI being tested
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ${ABI_64BIT})    # Leave this global property setting afterwards, do not restore it to its previous value
        find_library (FLOCK_LIBRARIES NAMES Flock ${FLOCK_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT} DOC "Flock library directory")
        if (WIN32)
            # For Windows platform, give a second chance to search for a debug version of the library
            find_library (FLOCK_LIBRARIES_DBG NAMES Flock_d ${FLOCK_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
            set (FLOCK_LIBRARIES_REL ${FLOCK_LIBRARIES})
            if (NOT FLOCK_LIBRARIES)
                set (FLOCK_LIBRARIES ${FLOCK_LIBRARIES_DBG})
            endif ()
        endif () 
        # Ensure the module has found the right one if the library type is specified
        if (FLOCK_LIBRARIES)
            get_filename_component (EXT ${FLOCK_LIBRARIES} EXT)
            if (EXT STREQUAL .a)
                set (FLOCK_LIB_TYPE STATIC)
                # For Non-MSVC compiler the static define is not baked into the export header file so we need to define it for the try_run below
                set (COMPILER_STATIC_DEFINE COMPILE_DEFINITIONS -DFLOCK_STATIC_DEFINE)
            else ()
                set (FLOCK_LIB_TYPE SHARED)
                unset (COMPILER_STATIC_DEFINE)
            endif ()
        endif ()
        # For shared library type, also initialize the FLOCK_DLL variable for later use
        if (WIN32 AND FLOCK_LIB_TYPE STREQUAL SHARED AND FLOCK_HOME)
            find_file (FLOCK_DLL_REL Flock.dll HINTS ${FLOCK_HOME}/bin NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH DOC "Flock release DLL")
            if (FLOCK_DLL_REL)
                list (APPEND FLOCK_DLL ${FLOCK_DLL_REL})
            endif ()
            find_file (FLOCK_DLL_DBG Flock_d.dll HINTS ${FLOCK_HOME}/bin NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH DOC "Flock debug DLL")
            if (FLOCK_DLL_DBG)
                list (APPEND FLOCK_DLL ${FLOCK_DLL_DBG})
            endif ()
        endif ()
        # Ensure the module has found the library with the right ABI for the chosen compiler and FLOCK_64BIT build option (if specified)
        if (FLOCK_COMPILE_RESULT)
            break ()    # Use the cached result instead of redoing try_run() each time
        elseif (FLOCK_LIBRARIES)
            if (NOT (MSVC OR ARM OR XCODE) AND NOT ABI_64BIT)
                set (COMPILER_32BIT_FLAG -m32)
            endif ()
            # Below variables are loop invariant but there is no harm to keep them here
            if (WIN32)
                set (CMAKE_TRY_COMPILE_CONFIGURATION_SAVED ${CMAKE_TRY_COMPILE_CONFIGURATION})
                if (FLOCK_LIBRARIES_REL)
                    set (CMAKE_TRY_COMPILE_CONFIGURATION Release)
                else ()
                    set (CMAKE_TRY_COMPILE_CONFIGURATION Debug)
                endif ()
            endif ()
            # Since in cross-compiling mode we cannot run the test target executable and auto-discover the build options used by the found library,
            # the next best thing is to evaluate the found export header indirectly (assuming the found library was built using the same export header)
            if (CMAKE_CROSSCOMPILING)
                set (FLOCK_RUN_RESULT 0)
                file (READ ${FLOCK_BASE_INCLUDE_DIR}/Flock.h FLOCK_RUN_RESULT__TRYRUN_OUTPUT)
            endif ()
            # Due to a bug in CMake where setting the CMAKE_EXECUTABLE_SUFFIX variable in the current local scope does not being honored by try_run(), we could not tell the command the correct suffix to expect (.js); it still thinks the suffix is empty string (as per Linux platform, see also comments in emscripten.toolchain.cmake module)
            # Workaround it by just doing try_compile() and fake the run output which is anyway the case for all the cross-compiling cases
            if (EMSCRIPTEN) 
            else ()
                set (COMPILER_FLAGS "${COMPILER_32BIT_FLAG} ${CMAKE_REQUIRED_FLAGS}")
                while (NOT FLOCK_COMPILE_RESULT)
                    try_run (FLOCK_RUN_RESULT FLOCK_COMPILE_RESULT ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR}/CheckFlockLibrary.cpp
                        CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${COMPILER_FLAGS} -DLINK_LIBRARIES:STRING=${FLOCK_LIBRARIES} -DINCLUDE_DIRECTORIES:STRING=${FLOCK_INCLUDE_DIRS} ${COMPILER_STATIC_DEFINE} ${COMPILER_STATIC_RUNTIME_FLAGS}
                        COMPILE_OUTPUT_VARIABLE TRY_COMPILE_OUT RUN_OUTPUT_VARIABLE TRY_RUN_OUT)
                        break ()    # Other compilers break immediately rendering the while-loop a no-ops
                endwhile ()
            endif ()
            set (FLOCK_COMPILE_RESULT ${FLOCK_COMPILE_RESULT} CACHE INTERNAL "FindFlock module's compile result")
            if (FLOCK_COMPILE_RESULT AND FLOCK_RUN_RESULT EQUAL 0)
                set (FLOCK_64BIT ${ABI_64BIT} CACHE BOOL "Enable 64-bit build, the value is auto-discovered based on the found Flock library" FORCE) # Force it as it is more authoritative than user-specified option
                set (FLOCK_LIB_TYPE ${FLOCK_LIB_TYPE} CACHE STRING "Flock library type, the value is auto-discovered based on the found Flock library" FORCE) # Use the Force, Luke
                foreach (VAR ${AUTO_DISCOVER_VARS})
                    if (TRY_RUN_OUT MATCHES "#define ${VAR}")
                        set (AUTO_DISCOVERED_${VAR} 1)
                    else ()
                        set (AUTO_DISCOVERED_${VAR} 0)
                    endif ()
                    set (AUTO_DISCOVERED_${VAR} ${AUTO_DISCOVERED_${VAR}} CACHE INTERNAL "Auto-discovered ${VAR} build option")
                endforeach ()
                break ()
            else ()
                # Prepare for the second attempt by resetting the variables as necessary
                set (FLOCK_LIB_TYPE ${FLOCK_LIB_TYPE_SAVED})
                unset (FLOCK_LIBRARIES CACHE)
            endif ()
        endif ()
    endforeach ()
    # If both the non-debug and debug version of the libraries are found on Windows platform then use them both
    if (FLOCK_LIBRARIES_REL AND FLOCK_LIBRARIES_DBG)
        set (FLOCK_LIBRARIES ${FLOCK_LIBRARIES_REL} ${FLOCK_LIBRARIES_DBG})
    endif ()
    # Ensure auto-discovered variables always prefail over user settings in all the subsequent cmake rerun (even without redoing try_run)
    foreach (VAR ${AUTO_DISCOVER_VARS})
        if (DEFINED ${VAR} AND DEFINED AUTO_DISCOVERED_${VAR})  # Cannot combine these two ifs due to variable expansion error when it is not defined
            if ((${VAR} AND NOT ${AUTO_DISCOVERED_${VAR}}) OR (NOT ${VAR} AND ${AUTO_DISCOVERED_${VAR}}))
                message (WARNING "Conflicting ${VAR} build option is ignored.")
                unset (${VAR} CACHE)
            endif ()
        endif ()
        set (${VAR} ${AUTO_DISCOVERED_${VAR}})
    endforeach ()
    # Restore CMake global settings
    if (CMAKE_FIND_LIBRARY_SUFFIXES_SAVED)
        set (CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_SAVED})
    endif ()
    if (CMAKE_SYSTEM_PREFIX_PATH_SAVED)
        set (CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH_SAVED})
    endif ()
    if (CMAKE_TRY_COMPILE_CONFIGURATION_SAVED)
        set (CMAKE_TRY_COMPILE_CONFIGURATION ${CMAKE_TRY_COMPILE_CONFIGURATION_SAVED})
    endif ()
endif ()

if (FLOCK_INCLUDE_DIRS AND FLOCK_LIBRARIES AND FLOCK_LIB_TYPE AND FLOCK_COMPILE_RESULT)
    set (FLOCK_FOUND 1)
    if (NOT FOUND_MESSAGE)
        set (FOUND_MESSAGE "Found Flock: ${FLOCK_LIBRARIES}")
        if (FLOCK_VERSION)
            set (FOUND_MESSAGE "${FOUND_MESSAGE} (found version \"${FLOCK_VERSION}\")")
        endif ()
    endif ()
    include (FindPackageMessage)
    find_package_message (Flock ${FOUND_MESSAGE} "[${FLOCK_LIBRARIES}][${FLOCK_INCLUDE_DIRS}]")
    set (FLOCK_HOME ${FLOCK_HOME} CACHE PATH "Path to Flock build tree or SDK installation location" FORCE)
    set (FLOCK_FOUND_64BIT ${FLOCK_64BIT} CACHE INTERNAL "True when 64-bit ABI was being used when test compiling Flock library")
    set (FLOCK_FOUND_LIB_TYPE ${FLOCK_LIB_TYPE} CACHE INTERNAL "Lib type (if specified) when Flock library was last found")
elseif (Flock_FIND_REQUIRED)
    if (FLOCK_LIB_TYPE)
        set (NOT_FOUND_MESSAGE "Ensure the specified location contains the Flock library of the requested library type. ${NOT_FOUND_MESSAGE}")
    endif ()
    message (FATAL_ERROR
        "Could NOT find compatible Flock library in Flock SDK installation or build tree. "
        "Use FLOCK_HOME environment variable or build option to specify the location of the non-default SDK installation or build tree. ${NOT_FOUND_MESSAGE} ${TRY_COMPILE_OUT}")
endif ()

mark_as_advanced (FLOCK_BASE_INCLUDE_DIR FLOCK_LIBRARIES FLOCK_LIBRARIES_DBG FLOCK_DLL_REL FLOCK_DLL_DBG)
