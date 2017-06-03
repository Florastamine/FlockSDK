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

# Define target name
set (TARGET_NAME Flock)

# Check sincosf support
include (CheckLibraryExists)
check_library_exists (m sincosf "" HAVE_SINCOSF)
if (HAVE_SINCOSF)
    add_definitions (-DHAVE_SINCOSF)
else ()
    include (CheckFunctionExists)
    check_function_exists (__sincosf HAVE_UNDERSCORE_SINCOSF)
    if (HAVE_UNDERSCORE_SINCOSF)
        add_definitions (-DHAVE_UNDERSCORE_SINCOSF)
    endif ()
endif ()

add_definitions (-DFLOCKSDK_IS_BUILDING)
if (DEFINED ENV{ASSET_DIR_INDICATOR})
    add_definitions (-DASSET_DIR_INDICATOR=\"$ENV{ASSET_DIR_INDICATOR}\")
endif ()

if (WIN32)
    set (CMAKE_DEBUG_POSTFIX _d)
endif () 

string (REPLACE ";" "/[^;]+;" EXCLUDE_PATTERNS "${EXCLUDED_SOURCE_DIRS};")
define_source_files (EXCLUDE_PATTERNS ${EXCLUDE_PATTERNS} GLOB_CPP_PATTERNS *.cpp ${GLOB_OBJC_PATTERN} RECURSE GROUP PCH Precompiled.h)

# Define generated source files
if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/librevision.h)
    execute_process (COMMAND ${CMAKE_COMMAND} -DFILENAME=${CMAKE_CURRENT_BINARY_DIR}/librevision.h -P CMake/Modules/GetFlockRevision.cmake
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_QUIET ERROR_QUIET)
endif ()
set (FLOCK_DEPS ${STATIC_LIBRARY_TARGETS})
if (TARGET LuaJIT_universal)
    list (REMOVE_ITEM FLOCK_DEPS LuaJIT)
    set (LuaJIT_universal_symbolic LuaJIT_universal_symbolic)
    add_custom_command (OUTPUT ${LuaJIT_universal_symbolic}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    list (APPEND SYMBOLIC_SOURCES ${LuaJIT_universal_symbolic})
endif ()
add_custom_command (OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/librevision.h
    COMMAND ${CMAKE_COMMAND} -DFILENAME=${CMAKE_CURRENT_BINARY_DIR}/librevision.h -P CMake/Modules/GetFlockRevision.cmake
    DEPENDS ${FLOCK_DEPS} ${CMAKE_SOURCE_DIR}/CMake/Modules/GetFlockRevision.cmake
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating GIT revision number (tag + last commit SHA-1)")
set_source_files_properties (${SYMBOLIC_SOURCES} PROPERTIES SYMBOLIC TRUE)

if (FLOCK_LUA)
    # The host tool must be built natively
    if (CMAKE_CROSSCOMPILING OR FLOCK_LUAJIT)
        # When cross-compiling or using LuaJIT, build the tolua++ host tool as external project using normal Lua (there is not much point using LuaJIT for the tool building even when technically it can)
        include (ExternalProject)
        ExternalProject_Add (tolua++
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source/ThirdParty/toluapp/src/bin
            CMAKE_ARGS -DDEST_RUNTIME_DIR=${CMAKE_BINARY_DIR}/bin/tool -DBAKED_CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR})
    else ()
        # Otherwise, build it internally as per normal
        add_subdirectory (../ThirdParty/toluapp/src/bin ../ThirdParty/toluapp/src/bin)
    endif ()
    # Use the host tool to generate source files for tolua++ API binding
    file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/LuaScript/generated)
    file (GLOB API_PKG_FILES LuaScript/pkgs/*.pkg)
    foreach (DIR IK Navigation Network)
        string (TOUPPER FLOCK_${DIR} OPT)
        if (NOT ${OPT})
            list (REMOVE_ITEM API_PKG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/LuaScript/pkgs/${DIR}LuaAPI.pkg)
        endif ()
    endforeach ()
    foreach (API_PKG_FILE ${API_PKG_FILES})
        get_filename_component (NAME ${API_PKG_FILE} NAME)
        string (REGEX REPLACE LuaAPI\\.pkg$ "" DIR ${NAME})
        set (GEN_CPP_FILE LuaScript/generated/${DIR}LuaAPI.cpp)
        list (APPEND GEN_CPP_FILES ${GEN_CPP_FILE})
        file (GLOB PKG_FILES LuaScript/pkgs/${DIR}/*.pkg)
        add_custom_command (OUTPUT ${GEN_CPP_FILE}
            COMMAND ${CMAKE_BINARY_DIR}/bin/tool/tolua++ -E ${CMAKE_PROJECT_NAME} -L ToCppHook.lua -o ${CMAKE_CURRENT_BINARY_DIR}/${GEN_CPP_FILE} ${NAME}
            DEPENDS tolua++ ${API_PKG_FILE} ${PKG_FILES} LuaScript/pkgs/ToCppHook.lua
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/LuaScript/pkgs
            COMMENT "Generating tolua++ API binding on the fly for ${DIR}")
    endforeach ()
endif ()

# Generate platform specific export header file

generate_export_header (${TARGET_NAME} ${FLOCK_LIB_TYPE} EXPORT_MACRO_NAME FLOCKSDK_API EXPORT_FILE_NAME Flock.h.new)
execute_process (COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/Flock.h.new ${CMAKE_CURRENT_BINARY_DIR}/Flock.h)
file (REMOVE ${CMAKE_CURRENT_BINARY_DIR}/Flock.h.new)

# Aggregate all source files
list (APPEND SOURCE_FILES ${GEN_CPP_FILES} ${ALL_OBJ_FILES} librevision.h Flock.h ${SYMBOLIC_SOURCES})

# Define dependency libs
# Add include directories to find the precompiled header, export header, and installed headers from thirdparty libs
set (INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/${DEST_INCLUDE_DIR}/ThirdParty)
# Bullet library depends on its own include dir to be added in the header search path
# This is more practical than patching its header files in many places to make them work with relative path
list (APPEND INCLUDE_DIRS ${CMAKE_BINARY_DIR}/${DEST_INCLUDE_DIR}/ThirdParty/Bullet)
if (FLOCK_NAVIGATION)
    # DetourTileCache and DetourCrowd libraries depend on Detour's include dir to be added in the header search path
    list (APPEND INCLUDE_DIRS ${CMAKE_BINARY_DIR}/${DEST_INCLUDE_DIR}/ThirdParty/Detour)
endif ()
if (FLOCK_LUA)
    # Lua/LuaJIT library depends on its own include dir to be added in the header search path
    # This is more practical than patching its header files in many places to make them work with relative path
    list (APPEND INCLUDE_DIRS ${CMAKE_BINARY_DIR}/${DEST_INCLUDE_DIR}/ThirdParty/Lua${JIT})
endif ()
# Workaround for GCC 5.4 and above when building a SHARED lib type for Linux platform to fix the undefined symbol "__cpu_model" issue (see #1519)
if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.3.1 AND CMAKE_SYSTEM_NAME STREQUAL Linux AND FLOCK_LIB_TYPE STREQUAL SHARED)  # 5.3.1 was the last known good version
    list (APPEND LIBS gcc)
endif ()

# Setup library output path 
set (OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set_output_directories (${OUTPUT_PATH} ARCHIVE LIBRARY)

# Setup target
setup_library (${FLOCK_LIB_TYPE} ${EXCLUDE_FROM_ALL})
install (TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${DEST_RUNTIME_DIR} LIBRARY DESTINATION ${DEST_LIBRARY_DIR} ARCHIVE DESTINATION ${DEST_LIBRARY_DIR})
if (WIN32 AND FLOCK_LIB_TYPE STREQUAL SHARED)
    set (FLOCK_DLL_INSTALLED TRUE)
endif ()

# Setup dependency frameworks and libraries
if (MINGW)
    set (CMAKE_C_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES_INIT} -lwinmm")
    set (CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES_INIT} -lwinmm")
endif ()
if (NOT MSVC)
    set_property (TARGET ${TARGET_NAME} APPEND PROPERTY LINK_LIBRARIES -Wl,--whole-archive)
    if (WIN32 AND FLOCK_LIB_TYPE STREQUAL SHARED)
        set_target_properties (${TARGET_NAME} PROPERTIES PREFIX "")
    endif ()
endif ()
foreach (TARGET ${STATIC_LIBRARY_TARGETS})
    get_target_property (ARCHIVE ${TARGET} LOCATION)
    if (ARCHIVE)
        if (NOT MSVC)
            set_property (TARGET ${TARGET_NAME} APPEND PROPERTY LINK_LIBRARIES ${FORCE_LOAD} ${ARCHIVE})    # Only works in Apple (Xcode and Makefile) and GCC while building shared library
        endif ()
        if (XCODE OR MSVC)
            set_property (TARGET ${TARGET_NAME} APPEND_STRING PROPERTY STATIC_LIBRARY_FLAGS " \"${ARCHIVE}\"")  # Only works in Apple (Xcode only) and MSVC while building static library
        else ()
            list (APPEND ARCHIVES ${ARCHIVE})
        endif ()
    endif ()
endforeach ()
if (NOT MSVC)
    set_property (TARGET ${TARGET_NAME} APPEND PROPERTY LINK_LIBRARIES -Wl,--no-whole-archive)
    if (NOT FLOCK_LIB_TYPE STREQUAL SHARED)
        if (CMAKE_BINARY_DIR MATCHES "[ +*;,]")
            # Do it the hard way by first extracting the object (bitcode) files and appending them to final archive:
            # b) When the build tree path contains spaces because MRI script does not support spaces in path even with proper escape
            get_filename_component (AR ${CMAKE_AR} NAME_WE)
            if (CMAKE_HOST_WIN32)
                add_custom_command (TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E remove_directory archives
                    COMMAND ${CMAKE_COMMAND} -E echo "Merging all archives into a single static library using ${AR}")
                foreach (ARCHIVE ${ARCHIVES})
                    # Merge per library because Window cmd shell does not handle long argument list
                    add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory archives)
                    add_custom_command (TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_AR} x ${ARCHIVE}
                        COMMAND ${CMAKE_COMMAND} -E echo_append "\\\"${CMAKE_AR}\\\"" q \\\"$<TARGET_FILE:${TARGET_NAME}>\\\" " " >merge.bat
                        COMMAND for /F %%i in ('dir /B *.o*') do ${CMAKE_COMMAND} -E echo_append %%i" " >>merge.bat
                        COMMAND merge.bat WORKING_DIRECTORY archives)
                    add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove_directory archives)
                endforeach ()
                add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_RANLIB} $<TARGET_FILE:${TARGET_NAME}>)
            else ()
                add_custom_command (TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E remove_directory archives
                    COMMAND ${CMAKE_COMMAND} -E make_directory archives COMMENT "Merging all archives into a single static library using ${AR}")
                foreach (ARCHIVE ${ARCHIVES})
                    add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_AR} x ${ARCHIVE} WORKING_DIRECTORY archives)
                endforeach ()
                add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_AR} q $<TARGET_FILE:${TARGET_NAME}> `ls -1` WORKING_DIRECTORY archives)
                add_custom_command (TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_RANLIB} $<TARGET_FILE:${TARGET_NAME}>
                    COMMAND ${CMAKE_COMMAND} -E remove_directory archives)
            endif ()
        else ()
            # GCC ar does not take archives directly as input like Apple libtool, however, it can be scripted to do so
            add_custom_command (TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E rename $<TARGET_FILE:${TARGET_NAME}> $<TARGET_FILE:${TARGET_NAME}>.engine
                COMMAND echo CREATE $<TARGET_FILE:${TARGET_NAME}> >script.ar
                COMMAND echo ADDLIB $<TARGET_FILE:${TARGET_NAME}>.engine >>script.ar)
            foreach (ARCHIVE ${ARCHIVES})
                add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND echo ADDLIB ${ARCHIVE} >>script.ar)
            endforeach ()
            add_custom_command (TARGET ${TARGET_NAME} POST_BUILD
                COMMAND echo SAVE >>script.ar
                COMMAND echo END >>script.ar
                COMMAND ${CMAKE_AR} -M <script.ar
                COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:${TARGET_NAME}>.engine script.ar
                COMMENT "Merging all archives into a single static library using ar")
        endif ()
    endif ()
endif ()
# todo This is a deprecated property in CMake version 2.8.12 - Remove below commands when CMake minimum version is 2.8.12
if (CMAKE_VERSION VERSION_LESS 2.8.12)
    set_target_properties (${TARGET_NAME} PROPERTIES LINK_INTERFACE_LIBRARIES "")
    if (FLOCK_LIB_TYPE STREQUAL SHARED)
        if (NOT MSVC AND CMAKE_VERSION VERSION_LESS 2.8.11)
            get_target_property (LINK_LIBRARIES ${TARGET_NAME} LINK_LIBRARIES)
            target_link_libraries (${TARGET_NAME} LINK_PRIVATE ${LINK_LIBRARIES})
        endif ()
    else ()
        set_target_properties (${TARGET_NAME} PROPERTIES LINK_LIBRARIES "")
    endif ()
endif ()
# end todo
# Use PIC on platforms that support it (shared library type has this property set to true by default, so we only have to deal with those static ones that the shared library links against)
if (FLOCK_LIB_TYPE STREQUAL SHARED)
    set_target_properties (${STATIC_LIBRARY_TARGETS} PROPERTIES POSITION_INDEPENDENT_CODE true)
    if (NOT MSVC AND NOT (MINGW AND CMAKE_CROSSCOMPILING) AND CMAKE_VERSION VERSION_LESS 2.8.9)  # todo: Remove this when CMake minimum version is 2.8.9
        set_property (TARGET ${STATIC_LIBRARY_TARGETS} APPEND PROPERTY COMPILE_FLAGS -fPIC)
    endif ()
endif ()

# Install headers for using the Flock library
install_header_files (FILES ${CMAKE_CURRENT_BINARY_DIR}/Flock.h DESTINATION ${DEST_INCLUDE_DIR} ACCUMULATE GENERATED_HEADERS)
install_header_files (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION ${DEST_INCLUDE_DIR} FILES_MATCHING PATTERN *.h ACCUMULATE FLOCK_HEADERS)    # Note: the trailing slash is significant
# Generate the include-all-headers header file even though we do not encourage Flock library users to use it
list (SORT FLOCK_HEADERS)
list (REVERSE FLOCK_HEADERS)
set (OPTIONAL_SUBS IK Database Lua Navigation Network Physics Urho2D)
foreach (SUB ${OPTIONAL_SUBS})
    if (FLOCK_HEADERS MATCHES "(include/Flock/${SUB}[^;]+)")
        list (FIND FLOCK_HEADERS ${CMAKE_MATCH_1} FOUND_INDEX)
        if (FOUND_INDEX GREATER -1)
            list (INSERT FLOCK_HEADERS ${FOUND_INDEX} "#endif")
        endif ()
    endif ()
endforeach ()
list (REVERSE FLOCK_HEADERS)
foreach (SUB ${OPTIONAL_SUBS})
    if (FLOCK_HEADERS MATCHES "(include/Flock/${SUB}[^;]+)")
        list (FIND FLOCK_HEADERS ${CMAKE_MATCH_1} FOUND_INDEX)
        if (FOUND_INDEX GREATER -1)
            string (TOUPPER ${SUB} SUB)
            list (INSERT FLOCK_HEADERS ${FOUND_INDEX} "#if FLOCK_${SUB}")
        endif ()
    endif ()
endforeach ()
string (REGEX REPLACE "include/[^;]+(OpenGL|Precompiled|SQLite|ToluaUtils)[^;]+;" "" FLOCK_HEADERS "${FLOCK_HEADERS}")
string (REGEX REPLACE "include/([^;]+)" "#include <\\1>" FLOCK_HEADERS "${GENERATED_HEADERS};;${FLOCK_HEADERS};")
string (REPLACE ";" \n FLOCK_HEADERS "${FLOCK_HEADERS}")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/FlockAll.h.in ${CMAKE_CURRENT_BINARY_DIR}/FlockAll.h)
install_header_files (FILES ${CMAKE_CURRENT_BINARY_DIR}/FlockAll.h ${CMAKE_CURRENT_BINARY_DIR}/librevision.h DESTINATION ${DEST_INCLUDE_DIR})

# Generate platform specific pkg-config file for the benefit of Flock library users via SDK without CMake
get_directory_property (FLOCK_COMPILE_DEFINITIONS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_DEFINITIONS)
# Remove those defines that are only used for building the library and not needed by library user
list (REMOVE_ITEM FLOCK_COMPILE_DEFINITIONS HAVE_STDINT_H GLEW_STATIC GLEW_NO_GLU HAVE_SINCOSF HAVE_UNDERSCORE_SINCOSF FLOCKSDK_IS_BUILDING ASSET_DIR_INDICATOR)
if (ABSOLUTE_PATH_LIBS)
    string (REPLACE ";" "\" \"" FLOCK_ABS_PATH_LIBS "\"${ABSOLUTE_PATH_LIBS}\"")   # Note: need to always "stringify" a variable in list context for replace to work correctly, besides the list could be empty
    string (REPLACE "${SYSROOT}" "\${pc_sysrootdir}" FLOCK_ABS_PATH_LIBS "${FLOCK_ABS_PATH_LIBS}")
endif ()
set (LIB_NAME ${TARGET_NAME})
if (MINGW)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        set (LIB_NAME ${LIB_NAME}_d)
    endif ()
    if (FLOCK_LIB_TYPE STREQUAL SHARED)
        set (LIB_NAME ${LIB_NAME}.dll)
    endif ()
endif ()
set (DASH -)
set (LIB_DIR "-L\"\${libdir}\"")
string (REPLACE ";" " -l" FLOCK_LIBS "-l${LIB_NAME};${LIBS}")
string (REPLACE ";" " ${DASH}D" FLOCK_COMPILE_DEFINITIONS "${DASH}D${FLOCK_COMPILE_DEFINITIONS}")
get_directory_property (GLOBAL_INCLUDE_DIRS DIRECTORY ${CMAKE_SOURCE_DIR} INCLUDE_DIRECTORIES)
if (GLOBAL_INCLUDE_DIRS)
    string (REPLACE ";" "\" ${DASH}I\"" GLOBAL_INCLUDE_DIRS "${DASH}I\"${GLOBAL_INCLUDE_DIRS}\"")
    string (REPLACE "${SYSROOT}" "" GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS})
endif ()
set (ENGINE_INCLUDE_DIRS "${DASH}I\"\${includedir}\" ${DASH}I\"\${includedir}/Flock/ThirdParty\"")
# Bullet library depends on its own include dir to be added in the header search path
# This is more practical than patching its header files in many places to make them work with relative path
set (ENGINE_INCLUDE_DIRS "${ENGINE_INCLUDE_DIRS} ${DASH}I\"\${includedir}/Flock/ThirdParty/Bullet\"")
if (FLOCK_LUA)
    # ditto for Lua/LuaJIT
    set (ENGINE_INCLUDE_DIRS "${ENGINE_INCLUDE_DIRS} ${DASH}I\"\${includedir}/Flock/ThirdParty/Lua${JIT}\"")
endif ()
# todo: Reevaluate the command below when the CMake minimum required version is set to 2.8.12 (and remove only when the deprecated add_compiler_export_flags() is not used anymore)
# Remove compiler flags that are only used for building the library
string (REGEX REPLACE " -fvisibility[^ ]+" "" CLEANED_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string (REGEX REPLACE " -include \"[^\"]+\"" "" CLEANED_CMAKE_CXX_FLAGS "${CLEANED_CMAKE_CXX_FLAGS}")
string (REPLACE "${SYSROOT}" "\${pc_sysrootdir}" CLEANED_CMAKE_CXX_FLAGS "${CLEANED_CMAKE_CXX_FLAGS}")
string (REPLACE "${SYSROOT}" "\${pc_sysrootdir}" CLEANED_CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Flock.pc.in ${CMAKE_CURRENT_BINARY_DIR}/Flock.pc${PC_SUFFIX} @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/Flock.pc DESTINATION ${DEST_PKGCONFIG_DIR} ${RENAME})

# Define post build steps
if (FLOCK_LIB_TYPE STREQUAL SHARED AND ARM)
    # Strip the output shared library for embedded devices
    add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${TARGET_NAME}>
        COMMENT "Stripping Flock shared library")
endif ()
if (FLOCK_SCP_TO_TARGET)
    # Ensure SCP is the last command
    add_custom_command (TARGET ${TARGET_NAME} POST_BUILD COMMAND scp $<TARGET_FILE:${TARGET_NAME}> ${FLOCK_SCP_TO_TARGET} || exit 0
        COMMENT "Scp-ing Flock library to target system")
endif ()
