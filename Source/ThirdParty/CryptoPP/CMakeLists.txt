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

include (TestBigEndian)

# Define target name
set (TARGET_NAME CryptoPP)

#============================================================================
# Settable options
#============================================================================
option (DISABLE_ASM "Disable ASM" OFF)
option (DISABLE_SSSE3 "Disable SSSE3" OFF)
option (DISABLE_AESNI "Disable AES-NI" OFF)

#============================================================================
# Internal compiler options
#============================================================================
# Only set when cross-compiling, http://www.vtk.org/Wiki/CMake_Cross_Compiling
if (NOT (CMAKE_SYSTEM_VERSION AND CMAKE_SYSTEM_PROCESSOR))
	set(CRYPTOPP_CROSS_COMPILE 1)
else()
	set(CRYPTOPP_CROSS_COMPILE 0)
endif ()

# Don't use RPATH's. The resulting binary could fail a security audit.
if (NOT CMAKE_VERSION VERSION_LESS 2.8.12)
  set(CMAKE_MACOSX_RPATH 0)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	add_definitions (-wd68 -wd186 -wd279 -wd327 -wd161 -wd3180)
endif ()

# Endianess
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if (IS_BIG_ENDIAN)
	add_definitions (-DIS_BIG_ENDIAN)
endif ()

if (DISABLE_ASM)
	add_definitions (-DCRYPTOPP_DISABLE_ASM)
endif ()

if (DISABLE_SSSE3)
	add_definitions (-DCRYPTOPP_DISABLE_SSSE3)
endif ()

if (DISABLE_AESNI)
	add_definitions (-DCRYPTOPP_DISABLE_AESNI)
endif ()

# We need the output 'uname -s' for Unix and Linux system detection
if (NOT CRYPTOPP_CROSS_COMPILE)
	set (UNAME_CMD "uname")
	set (UNAME_ARG "-s")
	execute_process(COMMAND ${UNAME_CMD} ${UNAME_ARG}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		RESULT_VARIABLE UNAME_RESULT
		OUTPUT_VARIABLE UNAME_SYSTEM)
		string(REGEX REPLACE "\n$" "" UNAME_SYSTEM "${UNAME_SYSTEM}")
endif ()

# We need the output 'uname -m' for Unix and Linux platform detection
if (NOT CRYPTOPP_CROSS_COMPILE)
	set (UNAME_CMD "uname")
	set (UNAME_ARG "-m")
	execute_process(COMMAND ${UNAME_CMD} ${UNAME_ARG}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		RESULT_VARIABLE UNAME_RESULT
		OUTPUT_VARIABLE UNAME_MACHINE)
	string(REGEX REPLACE "\n$" "" UNAME_MACHINE "${UNAME_MACHINE}")
endif ()

# Enable PIC for all targets except Windows and 32-bit x86.
#   Avoid on 32-bit x86 due to register pressures.
if ((NOT CRYPTOPP_CROSS_COMPILE) AND (NOT (WINDOWS OR WINDOWS_STORE OR WINDOWS_PHONE)))
	# Use Regex; match i386, i486, i586 and i686
	if (NOT (${UNAME_MACHINE} MATCHES "i.86"))
		SET(CMAKE_POSITION_INDEPENDENT_CODE 1)
	endif ()
endif ()

# -march=native for GCC, Clang and ICC in any version that does support it.
if ((NOT CRYPTOPP_CROSS_COMPILE) AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|Intel"))
	CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_OPT_ARCH_NATIVE_SUPPORTED)
	if (COMPILER_OPT_ARCH_NATIVE_SUPPORTED AND NOT CMAKE_CXX_FLAGS MATCHES "-march=")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
	endif ()
endif ()

# Link is driven through the compiler, but CXXFLAGS are not used. Also see
#   http://public.kitware.com/pipermail/cmake/2003-June/003967.html
if (NOT (WINDOWS OR WINDOWS_STORE OR WINDOWS_PHONE))
	SET(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()

#============================================================================
# Sources & headers
#============================================================================
# Library headers
file(GLOB H_FILES *.h)

# Library sources
file(GLOB CPP_FILES *.cpp)
list(REMOVE_ITEM CPP_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/integer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/simple.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/winpipes.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib_bds.cpp)
set(CPP_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/cryptlib.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/integer.cpp
		${CPP_FILES})

if (MINGW OR WIN32)
	list(APPEND CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/winpipes.cpp)
endif ()

set (SOURCE_FILES ${CPP_FILES} ${H_FILES})

# Define dependency libs
set (INCLUDE_DIRS .)

# Setup target
setup_library ()

# Install headers for building and using the Flock library (install dependency for Flock/Physics/PhysicsWorld.h, Flock/Physics/RigidBody.h, and Flock/Physics/PhysicsUtils.h)
install_header_files (DIRECTORY ./ DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty/CryptoPP FILES_MATCHING PATTERN *.h)  # Note: the trailing slash is significant
