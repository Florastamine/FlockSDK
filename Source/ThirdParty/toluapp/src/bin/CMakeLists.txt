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

if (NOT CMAKE_PROJECT_NAME STREQUAL Flock)
    # Set CMake minimum version and CMake policy required by Flock-CMake-common module
    if (WIN32)
        cmake_minimum_required (VERSION 3.2.3)      # Going forward all platforms will use this as minimum version
    else ()
        cmake_minimum_required (VERSION 2.8.6)
    endif ()
    if (COMMAND cmake_policy)
        cmake_policy (SET CMP0003 NEW)
        if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
            # INTERFACE_LINK_LIBRARIES defines the link interface
            cmake_policy (SET CMP0022 NEW)
        endif ()
        if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
            # Disallow use of the LOCATION target property - so we set to OLD as we still need it
            cmake_policy (SET CMP0026 OLD)
        endif ()
    endif ()

    # Set project name
    project (Flock-ExternalProject-tolua++)

    # Set CMake modules search path
    set (CMAKE_MODULE_PATH ${BAKED_CMAKE_SOURCE_DIR}/CMake/Modules)

    # Include Flock Cmake common module
    include (Flock-CMake-common)

    # Add dependency targets
    add_subdirectory (${BAKED_CMAKE_SOURCE_DIR}/Source/ThirdParty/LuaJIT host/LuaJIT)
    add_subdirectory (${BAKED_CMAKE_SOURCE_DIR}/Source/ThirdParty/toluapp/src/lib host/toluapp)
endif ()

# Define target name
set (TARGET_NAME tolua++)

# Define generated source files
file (GLOB TOLUA lua/*.lua)
add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/toluabind.c
    COMMAND ${CMAKE_BINARY_DIR}/bin/tool/tolua++ -o ${CMAKE_CURRENT_BINARY_DIR}/generated/toluabind.c -H ${CMAKE_CURRENT_BINARY_DIR}/generated/toluabind.h -n tolua tolua_scons.pkg 2>${NULL_DEVICE} || ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/toluabind.c ${CMAKE_CURRENT_BINARY_DIR}/generated/toluabind.c
    ${UPDATE_COMMAND}
    DEPENDS ${TOLUA} tolua_scons.pkg
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating toluabind.c and toluabind.h")

# Define source files
define_source_files (EXTRA_CPP_FILES tolua.c ${CMAKE_CURRENT_BINARY_DIR}/generated/toluabind.c)

# Define dependency libs
set (LIBS toluapp LuaJIT)
list (APPEND LIBS m)
set (INCLUDE_DIRS ../../include ../../../../ThirdParty/LuaJIT/src)

# Setup target
setup_executable (TOOL NODEPS)
