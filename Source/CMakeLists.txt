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

# Check existence of stdint.h for LibCpuId
include (CheckIncludeFiles)
check_include_files (stdint.h HAVE_STDINT_H)
if (HAVE_STDINT_H)
    add_definitions (-DHAVE_STDINT_H)
endif ()

# Setup RPATH settings
if (FLOCK_LIB_TYPE STREQUAL SHARED AND NOT WIN32)
    # Add RPATH entries when building
    set (CMAKE_SKIP_BUILD_RPATH FALSE)
    # But don't set them yet in the build tree
    set (CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set (ORIGIN $ORIGIN)
    # When installing/packaging set the first RPATH entry to the library location relative to the executable
    set (CMAKE_INSTALL_RPATH ${ORIGIN}/../lib${LIB_SUFFIX}/${PATH_SUFFIX})    # The library location is based on SDK install destination
    # The second entry to the library location relative to the executable in the tool directory
    list (APPEND CMAKE_INSTALL_RPATH ${ORIGIN}/../../lib${LIB_SUFFIX}/${PATH_SUFFIX})    # The tools are installed one directory further down from normal executable
    # The last entry to the install destination of the library, if the destination location is not in the system default search path
    list (FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX} isSystemDir)
    if (isSystemDir STREQUAL -1)
        list (APPEND CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${DEST_LIBRARY_DIR})
    endif ()
endif () 

# Add targets
foreach (TARGET STB GLEW JO TinyAES LZ4 SQLite StanHull Lua LuaJIT toluapp/src/lib PugiXml rapidjson Detour DetourCrowd DetourTileCache Recast FreeType Box2D SDL Bullet) 
    IF (EXISTS "${PROJECT_SOURCE_DIR}/Source/ThirdParty/${TARGET}")
        message (STATUS "Found 3rd-party target: " ${TARGET})
        add_subdirectory (ThirdParty/${TARGET})
    endif()
endforeach ()

if (FLOCK_SCENE_EDITOR)
    message (STATUS "The following optional component(s) will be installed: Editor")
    add_subdirectory (ThirdParty/AngelScript)
endif ()

if (FLOCK_IK)
    message (STATUS "The following optional component(s) will be installed: IK")
    add_subdirectory (ThirdParty/ik)
endif ()

if (FLOCK_NETWORK)
    message (STATUS "The following optional component(s) will be installed: Networking")
    add_subdirectory (ThirdParty/Civetweb)
    add_subdirectory (ThirdParty/kNet)
endif ()

# Flock game engine library
add_subdirectory (Flock)

# Flock tools
add_subdirectory (Tools)
