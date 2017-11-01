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

# Set CMake minimum version and CMake policy required by Flock-CMake-common module 
cmake_minimum_required (VERSION 3.2.3)

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
project (Flock)

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# Include Flock Cmake common module
include (Flock-CMake-common)

# Setup SDK install destinations
set (PATH_SUFFIX Flock)
if (WIN32)
    set (SCRIPT_EXT .bat)
    if (CMAKE_HOST_WIN32)
        set (PATH_SUFFIX .)
    endif ()
else ()
    set (SCRIPT_EXT .sh)
endif ()

if (FLOCK_64BIT)
    # Install to 'lib64' when one of these conditions is true
    if ((MINGW AND CMAKE_CROSSCOMPILING) OR FLOCK_USE_LIB64_RPM OR (HAS_LIB64 AND NOT FLOCK_USE_LIB_DEB))
        set (LIB_SUFFIX 64)
    endif ()
endif ()

set (DEST_INCLUDE_DIR include/Flock)    # The include directory path contains the 'Flock' suffix regardless of PATH_SUFFIX variable
set (DEST_SHARE_DIR share/${PATH_SUFFIX})
set (DEST_BUNDLE_DIR ${DEST_SHARE_DIR}/Applications)
# Note to package maintainer: ${PATH_SUFFIX} for library could be removed if the extra path is not desired, but if so then the RPATH setting in Source's CMakeLists.txt needs to be adjusted accordingly
set (DEST_LIBRARY_DIR lib${LIB_SUFFIX}/${PATH_SUFFIX})
set (DEST_PKGCONFIG_DIR lib${LIB_SUFFIX}/pkgconfig)
# Install application launcher scripts
file (GLOB APP_SCRIPTS ${CMAKE_SOURCE_DIR}/bin/*${SCRIPT_EXT})
install (PROGRAMS ${APP_SCRIPTS} DESTINATION ${DEST_RUNTIME_DIR})   # DEST_RUNTIME_DIR variable is set by the set_output_directories() macro call in the Flock-CMake-common module
# Install resource directories required by applications built with Flock library
install (DIRECTORY ${CMAKE_SOURCE_DIR}/bin/pfiles/early ${CMAKE_SOURCE_DIR}/bin/pfiles DESTINATION ${DEST_SHARE_DIR}/Resources)
# Install CMake modules and toolchains provided by and for Flock
install (DIRECTORY ${CMAKE_SOURCE_DIR}/CMake/ DESTINATION ${DEST_SHARE_DIR}/CMake)    # Note: the trailing slash is significant
# Install CMake build scripts
file (GLOB CMAKE_SCRIPTS ${CMAKE_SOURCE_DIR}/*${SCRIPT_EXT})
install (PROGRAMS ${CMAKE_SCRIPTS} DESTINATION ${DEST_SHARE_DIR}/Scripts)

# Setup SDK-like include dir in the build tree for building the Flock library
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${DEST_INCLUDE_DIR}/ThirdParty)

# Flock source
add_subdirectory (Source)
