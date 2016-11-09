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

<<<<<<< HEAD
# Set CMake minimum version and CMake policy required by Urho3D-CMake-common module
if (WIN32)
    cmake_minimum_required (VERSION 3.2.3)      # Going forward all platforms will use this as minimum version
=======
# Urho3D - commented out in-source tree build prevention as Urho3D supports both out-of-source and in-source tree builds

# Urho3D - commented out setting SDL2 as project name as we want SDL sub-library to remain within Urho3D "umbrella" project
#          instead, just enable the required language support and set the extra variables associated with project() command
enable_language (C)
set (SDL2_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (SDL2_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
# Urho3D - commented out CheckTypeSize, FindPkgConfig, and GNUInstallDirs modules as the corresponding macros are not being used in our modified version
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckIncludeFiles)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckCSourceRuns)
include(CheckCCompilerFlag)
include(CheckStructHasMember)
include(CMakeDependentOption)
# Urho3D - commented out CMAKE_MODULE_PATH configuration so that it does not mess up with ours; besides below two include() use file signature instead of module signature
include(${SDL2_SOURCE_DIR}/cmake/macros.cmake)
include(${SDL2_SOURCE_DIR}/cmake/sdlchecks.cmake)

# General settings
# Edit include/SDL_version.h and change the version, then:
#   SDL_MICRO_VERSION += 1;
#   SDL_INTERFACE_AGE += 1;
#   SDL_BINARY_AGE += 1;
# if any functions have been added, set SDL_INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set SDL_BINARY_AGE and SDL_INTERFACE_AGE to 0.
set(SDL_MAJOR_VERSION 2)
set(SDL_MINOR_VERSION 0)
set(SDL_MICRO_VERSION 5)
set(SDL_INTERFACE_AGE 1)
set(SDL_BINARY_AGE 5)
set(SDL_VERSION "${SDL_MAJOR_VERSION}.${SDL_MINOR_VERSION}.${SDL_MICRO_VERSION}")

# Calculate a libtool-like version number
math(EXPR LT_CURRENT "${SDL_MICRO_VERSION} - ${SDL_INTERFACE_AGE}")
math(EXPR LT_AGE "${SDL_BINARY_AGE} - ${SDL_INTERFACE_AGE}")
math(EXPR LT_MAJOR "${LT_CURRENT}- ${LT_AGE}")
set(LT_REVISION "${SDL_INTERFACE_AGE}")
set(LT_RELEASE "${SDL_MAJOR_VERSION}.${SDL_MINOR_VERSION}")
set(LT_VERSION "${LT_MAJOR}.${LT_AGE}.${LT_REVISION}")

# Urho3D - commented out debug print

# General settings & flags
# Urho3D - commented out LIBRARY_OUTPUT_DIRECTORY configuration so that it does not mess up with our global configuration setup
# Check for 64 or 32 bit
# Urho3D - bug fix - using CMAKE_SIZEOF_VOID_P is not accurate in a situation where a multilib-capable compiler is chosen and user has opted to use non-native ABI; instead using URHO3D_64BIT build option as it also captures the user intention
if(URHO3D_64BIT)
  set(ARCH_64 TRUE)
  set(PROCESSOR_ARCH "x64")
else()
  set(ARCH_64 FALSE)
  set(PROCESSOR_ARCH "x86")
endif()
# Urho3D - commented out redundant CMake variables that do not appear to be used anywhere

# Get the platform
# Urho3D - rearrange the if-elseif blocks (specific case before generic case), simplify the regex, and remove unused variables
if (WIN32)
  set(WINDOWS TRUE)
elseif (HAIKU OR ANDROID OR IOS OR TVOS)   # IOS/TVOS CMake variables are defined in Urho3D when targeting iOS/tvOS platforms, respectively
  # Do nothing here but this should prevent these platforms from entering the more generic case below, i.e.
  # - preventing LINUX variable being set for Android (when using older CMake)
  # - preventing DARWIN/MACOSX variabe being set for iOS
  # - preventing UNIX_SYS variable being set for Haiku
elseif (APPLE)
  if (CMAKE_SYSTEM_NAME MATCHES Darwin)
    set (DARWIN TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES MacOS)
    set (MACOSX TRUE)
  endif ()
elseif (UNIX)
  set (UNIX_SYS TRUE)
  if (CMAKE_SYSTEM_NAME MATCHES Linux)
    set (LINUX TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES FreeBSD|DragonFly)
    set (FREEBSD TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES NetBSD)
    set (NETBSD TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES OpenBSD)
    set (OPENBSD TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES GNU)
    set (GNU TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES BSDI)
    set (BSDI TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES SYSV5)
    set (SYSV5 TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES Solaris)
    set (SOLARIS TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES HP-UX)
    set (HPUX TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES AIX)
    set (AIX TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES Minix)
    set (MINIX TRUE)
  elseif (CMAKE_SYSTEM_NAME MATCHES BeOS)
    message_error ("BeOS support has been removed as of SDL 2.0.2.")
  endif ()
endif()

# Default option knobs
# Urho3D - commented out OPT_DEF_SSEMATH default variable as the underyling SSEMATH build option is also being commented out by us
if(UNIX OR MINGW OR MSYS)
  set(OPT_DEF_LIBC ON)
endif()

# Compiler info
if(CMAKE_COMPILER_IS_GNUCC)
  set(USE_GCC TRUE)
  set(OPT_DEF_ASM TRUE)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(USE_CLANG TRUE)
  set(OPT_DEF_ASM TRUE)
elseif(MSVC_VERSION GREATER 1400) # VisualStudio 8.0+
  set(OPT_DEF_ASM TRUE)
  #set(CMAKE_C_FLAGS "/ZI /WX- /
else()
  set(OPT_DEF_ASM FALSE)
endif()

# Urho3D - commented out compiler/linker flags setup as we have configured all our compiler/linker flags as we want globally

# Those are used for pkg-config and friends, so that the SDL2.pc, sdl2-config, # etc. are created correctly.
# Urho3D - we do not use these variables for pkg-config setup for SDL library but we may need them later for Urho3D library
set (SDL_LIBS)
set (SDL_CFLAGS)

# Emscripten toolchain has a nonempty default value for this, and the checks
# in this file need to change that, so remember the original value, and
# restore back to that afterwards. For check_function_exists() to work in
# Emscripten, this value must be at its default value.
# Urho3D - also due to a CMake's try_compile() bug on iOS platform, the CMAKE_REQUIRED_FLAGS contains our SmileyHack-injection on iOS platform
set (ORIG_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})
# Urho3D - save the original CMake global settings, do not leave them to chances
set (ORIG_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
set (ORIG_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES})

if (RPI)
  # Urho3D - TODO - move the find_package(VideoCore REQUIRED) call from Urho3D common module to here later after we have refactored the library dependency handling
  # Urho3D - bug fix - adjust the header search path for subsequent pthread, EGL and OpenGLES2 checks
  list (APPEND CMAKE_REQUIRED_INCLUDES ${VIDEOCORE_INCLUDE_DIRS})
endif ()

if(CYGWIN)
  # We build SDL on cygwin without the UNIX emulation layer
  # Urho3D - bug fix - we do not support Cygwin build so this fix is untested, just for completeness sake
  include_directories(/usr/include/mingw)
  set(CMAKE_REQUIRED_FLAGS "-mno-cygwin ${ORIG_CMAKE_REQUIRED_FLAGS}")
  check_c_source_compiles("int main(int argc, char **argv) {}"
    HAVE_GCC_NO_CYGWIN)
  set(CMAKE_REQUIRED_FLAGS ${ORIG_CMAKE_REQUIRED_FLAGS})
  if(HAVE_GCC_NO_CYGWIN)
    list(APPEND EXTRA_LDFLAGS "-mno-cygwin")
    list(APPEND SDL_LIBS "-mno-cygwin")
  endif()
  set(SDL_CFLAGS "${SDL_CFLAGS} -I/usr/include/mingw")
endif()

add_definitions(-DUSING_GENERATED_CONFIG_H)
# General includes
# Urho3D - use 'generated' path suffix for generated config header file, the suffic prevents overwriting the header file with the same name in the source tree in case of non out-of-source build tree is being used
set (INCLUDE_DIRS ${SDL2_BINARY_DIR}/include/generated ${SDL2_SOURCE_DIR}/include)

# All these ENABLED_BY_DEFAULT vars will default to ON if not specified, so
#  you only need to have a platform override them if they are disabling.
# Urho3D - bug fix - commented out double initialization of the OPT_DEF_ASM variable, it has been initialized properly in "Compiler info" section above
if(EMSCRIPTEN)
  # Set up default values for the currently supported set of subsystems:
  # Emscripten/Javascript does not have assembly support, a dynamic library
  # loading architecture, low-level CPU inspection or multithreading.
  set(OPT_DEF_ASM FALSE)
  set(SDL_SHARED_ENABLED_BY_DEFAULT OFF)
  # Urho3D - leave SDL_ATOMIC and SDL_LOADSO enable for Emscripten
  set(SDL_THREADS_ENABLED_BY_DEFAULT OFF)
  set(SDL_CPUINFO_ENABLED_BY_DEFAULT OFF)
endif()

# Urho3D - we always build internal sub-libraries as STATIC library type, so this build option is not applicable to us
set (SDL_SHARED_ENABLED_BY_DEFAULT OFF)
# Urho3D - we prefer to disable 2D render subsystem by default
if (NOT DEFINED SDL_RENDER_ENABLED_BY_DEFAULT)
    set (SDL_RENDER_ENABLED_BY_DEFAULT OFF)
endif ()

set(SDL_SUBSYSTEMS
    Atomic Audio Video Render Events Joystick Haptic Power Threads Timers
    File Loadso CPUinfo Filesystem Dlopen)
foreach(_SUB ${SDL_SUBSYSTEMS})
  string(TOUPPER ${_SUB} _OPT)
  if (NOT DEFINED SDL_${_OPT}_ENABLED_BY_DEFAULT)
    set(SDL_${_OPT}_ENABLED_BY_DEFAULT ON)
  endif()
  # Urho3D - bug fix - prevent SDL_DLOPEN build option from being declared twice
  if (NOT _OPT MATCHES DLOPEN|HAPTIC)
    option(SDL_${_OPT} "Enable the ${_SUB} subsystem" ${SDL_${_OPT}_ENABLED_BY_DEFAULT})
  endif ()
endforeach()
# Urho3D - declare SDL_HAPTIC as a dependent option to SDL_JOYSTICK as haptic requires some private functions from the joystick subsystem
dep_option (SDL_HAPTIC "Enable the Haptic subsystem" ${SDL_HAPTIC_ENABLED_BY_DEFAULT} SDL_JOYSTICK OFF)

# Urho3D - TODO - put all these options into SDL-specific namespace group, they are littering CMake-gui all over the place as it is now
#          Also there seems to be duplicate between SDL_SUBSYSTEMS above with build options below: SDL_ATOMIC vs GCC_ATOMICS, SDL_THREADS vs PTHREADS, SDL_DLOPEN (subsystem) vs SDL_DLOPEN (option)
option_string(ASSERTIONS "Enable internal sanity checks (auto/disabled/release/enabled/paranoid)" "auto")
#set_option(DEPENDENCY_TRACKING "Use gcc -MMD -MT dependency tracking" ON)
set_option(LIBC                "Use the system C library" ${OPT_DEF_LIBC})
# Urho3D - bug fix - also enable GCC_ATOMICS by default for Clang (including Emscripten which is a Clang derivative)
dep_option(GCC_ATOMICS         "Use gcc builtin atomics" ON "USE_GCC OR USE_CLANG" OFF)
set_option(ASSEMBLY            "Enable assembly routines" ${OPT_DEF_ASM})
# Urho3D - commented out SSEMATH, SSE, SSE2, MMX, 3DNOW, ALTIVEC build options as they have been reclassifified as Urho3D own build options
set_option(DISKAUDIO           "Support the disk writer audio driver" ON)
set_option(DUMMYAUDIO          "Support the dummy audio driver" ON)
set_option(VIDEO_DIRECTFB      "Use DirectFB video driver" OFF)
dep_option(DIRECTFB_SHARED     "Dynamically load directfb support" ON VIDEO_DIRECTFB OFF)
set_option(FUSIONSOUND         "Use FusionSound audio driver" OFF)
dep_option(FUSIONSOUND_SHARED  "Dynamically load fusionsound audio support" ON FUSIONSOUND OFF)
set_option(VIDEO_DUMMY         "Use dummy video driver" ON)
set_option(VIDEO_OPENGL        "Include OpenGL support" ON)
# Urho3D - hide OpenGLES option from MinGW platform because it does not have the necessary header file to build OpenGLES video driver
dep_option(VIDEO_OPENGLES      "Include OpenGL ES support" ON "NOT MINGW" OFF)
# Urho3D - only make PTHREADS option available on UNIX-like platforms
dep_option(PTHREADS            "Use POSIX threads for multi-threading" ON UNIX OFF)
dep_option(PTHREADS_SEM        "Use pthread semaphores" ON PTHREADS OFF)
# Urho3D - SDL_DLOPEN was originally set as option twice (one here and one in the subsystem section above), we keep the one here so that we can make the option dependent on other conditions
dep_option(SDL_DLOPEN          "Use dlopen for shared object loading" ON "UNIX\;NOT IOS OR NOT IPHONEOS_DEPLOYMENT_TARGET OR NOT IPHONEOS_DEPLOYMENT_TARGET VERSION_LESS 8" OFF)
# Urho3D - hide Unix-specific audio driver options from Android platform
dep_option(OSS                 "Support the OSS audio API" ON UNIX_SYS OFF)
dep_option(ALSA                "Support the ALSA audio API" ON UNIX_SYS OFF)
dep_option(ALSA_SHARED         "Dynamically load ALSA audio support" ON ALSA OFF)
dep_option(ESD                 "Support the Enlightened Sound Daemon" ON UNIX_SYS OFF)
dep_option(ESD_SHARED          "Dynamically load ESD audio support" ON ESD OFF)
dep_option(PULSEAUDIO          "Use PulseAudio" ON UNIX_SYS OFF)
dep_option(PULSEAUDIO_SHARED   "Dynamically load PulseAudio support" ON PULSEAUDIO OFF)
dep_option(ARTS                "Support the Analog Real Time Synthesizer" ON UNIX_SYS OFF)
dep_option(ARTS_SHARED         "Dynamically load aRts audio support" ON ARTS OFF)
dep_option(NAS                 "Support the NAS audio API" ON UNIX_SYS OFF)
dep_option(NAS_SHARED          "Dynamically load NAS audio API" ON NAS OFF)
dep_option(SNDIO               "Support the Roar audio API" ON UNIX_SYS OFF)
# Urho3D - commented out RPATH as an option because Urho3D is configured to always use RPATH, so init the variable to always TRUE
set (RPATH TRUE)
# Urho3D - prefer clock_gettime() when it is available (hide the option for Windows and Apple)
dep_option(CLOCK_GETTIME       "Use clock_gettime() instead of gettimeofday()" ON "NOT WINDOWS AND NOT APPLE" OFF)
set_option(INPUT_TSLIB         "Use the Touchscreen library for input" ${UNIX_SYS})
dep_option(VIDEO_WAYLAND       "Use Wayland video driver" ON UNIX_SYS OFF)
dep_option(WAYLAND_SHARED      "Dynamically load Wayland support" ON VIDEO_WAYLAND OFF)
# Urho3D - change the VIDEO_WAYLAND_QT_TOUCH default value to FALSE (this is only needed by Sailfish[?] but we don't know way to auto-detect that yet)
dep_option(VIDEO_WAYLAND_QT_TOUCH  "QtWayland server support for Wayland video driver" OFF VIDEO_WAYLAND OFF)
dep_option(VIDEO_MIR           "Use Mir video driver" ON UNIX_SYS OFF)
dep_option(MIR_SHARED          "Dynamically load Mir support" ON VIDEO_MIR OFF)
# Urho3D - only make VIDEO_RPI option available on Raspberry-Pi platform
dep_option(VIDEO_RPI           "Use Raspberry Pi video driver" ON RPI OFF)
# Urho3D - bug fix - make VIDEO_X11 build option available for desktop UNIX (including OSX) only
dep_option(VIDEO_X11           "Use X11 video driver" ON "UNIX_SYS OR APPLE\;NOT RPI AND NOT IOS AND NOT TVOS" OFF)
dep_option(X11_SHARED          "Dynamically load X11 support" ON VIDEO_X11 OFF)
foreach(_SUB Xcursor Xinerama XInput Xrandr Xscrnsaver XShape Xvm)
  string(TOUPPER "VIDEO_X11_${_SUB}" _OPT)
  dep_option(${_OPT}           "Enable ${_SUB} support" ON VIDEO_X11 OFF)
endforeach()
# Urho3D - bug fix - only make the COCOA option available on OSX platform but not iOS/tvOS; it does not make too much sense now for user to disable it because SDL/Apple has dropped support for XQuartz (X11 on Mac), so Cocoa is the only video driver available
#          But we still keep it as an option here because it should be possible to add back the XQuartz support
dep_option(VIDEO_COCOA         "Use Cocoa video driver" ON "APPLE AND NOT IOS AND NOT TVOS" OFF)
# Urho3D - only make DIRECTX option available on Windows platform when URHO3D_OPENGL is enabled, i.e. DIRECTX variable must always be ON (not an option) when URHO3D_OPENGL is disabled because in this case it must depend on Direct3D which is a component of DirectX
if (WINDOWS)
  dep_option(DIRECTX           "Use DirectX for Windows audio/video" ON URHO3D_OPENGL ON)
>>>>>>> 8eb652a... Squashed 'Source/ThirdParty/SDL/' changes from a901827..fe37f87
else ()
    cmake_minimum_required (VERSION 2.8.6)
endif ()
<<<<<<< HEAD
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - so we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
=======
# Urho3D - commented out RENDER_D3D as an option to avoid potential conflict with our URHO3D_OPENGL and URHO3D_D3D11 build options on Windows platform
#          Instead just initialize the variable according to our build options; Urho3D also by default disables the SDL renderer subsystem
if (WINDOWS)
  if (URHO3D_OPENGL)
    set (RENDER_D3D FALSE)
  else ()
    set (RENDER_D3D TRUE)
  endif ()
endif ()
set_option(VIDEO_VIVANTE       "Use Vivante EGL video driver" ${UNIX_SYS})

# TODO: We should (should we?) respect cmake's ${BUILD_SHARED_LIBS} flag here
# The options below are for compatibility to configure's default behaviour.
set(SDL_SHARED ${SDL_SHARED_ENABLED_BY_DEFAULT} CACHE BOOL "Build a shared version of the library")
set(SDL_STATIC ON CACHE BOOL "Build a static version of the library")

dep_option(SDL_STATIC_PIC      "Static version of the library should be built with Position Independent Code" OFF "SDL_STATIC" OFF)

# General source files
set (SOURCE_FILE_PATTERNS
  ${SDL2_SOURCE_DIR}/src/*.c
  ${SDL2_SOURCE_DIR}/src/atomic/*.c
# Urho3D - we always disable dynamic API, so commented out ${SDL2_SOURCE_DIR}/src/dynapi/*.c
  ${SDL2_SOURCE_DIR}/src/libm/*.c
  ${SDL2_SOURCE_DIR}/src/stdlib/*.c
  ${SDL2_SOURCE_DIR}/src/thread/*.c
  ${SDL2_SOURCE_DIR}/src/timer/*.c)
# Urho3D - exclude source files from disabled subsystems, except SDL_ATOMIC, SDL_THREADS and SDL_TIMERS as SDL always needs them even when they may be just generic or dummy implementations
foreach (_SUB AUDIO CPUINFO EVENTS FILE HAPTIC JOYSTICK POWER RENDER VIDEO)
  string (TOLOWER ${_SUB} _DIR)
  if (${SDL_${_SUB}})
    list (APPEND SOURCE_FILE_PATTERNS ${SDL2_SOURCE_DIR}/src/${_DIR}/*.c)
  endif ()
endforeach ()
if (SDL_RENDER)
  list (APPEND ${SDL2_SOURCE_DIR}/src/render/*/*.c)   # Recurse one more level
endif ()
file(GLOB SOURCE_FILES ${SOURCE_FILE_PATTERNS})

if(ASSERTIONS STREQUAL "auto")
  # Do nada - use optimization settings to determine the assertion level
elseif(ASSERTIONS STREQUAL "disabled")
  set(SDL_DEFAULT_ASSERT_LEVEL 0)
elseif(ASSERTIONS STREQUAL "release")
  set(SDL_DEFAULT_ASSERT_LEVEL 1)
elseif(ASSERTIONS STREQUAL "enabled")
  set(SDL_DEFAULT_ASSERT_LEVEL 2)
elseif(ASSERTIONS STREQUAL "paranoid")
  set(SDL_DEFAULT_ASSERT_LEVEL 3)
else()
  message_error("unknown assertion level")
endif()
set(HAVE_ASSERTIONS ${ASSERTIONS})

# Compiler option evaluation
if(USE_GCC OR USE_CLANG)
  # Check for -Wall first, so later things can override pieces of it.
  check_c_compiler_flag(-Wall HAVE_GCC_WALL)
  if(HAVE_GCC_WALL)
    list(APPEND EXTRA_CFLAGS "-Wall")
    if(HAIKU)
      # Urho3D - TODO - since we do not support HAIKU for now, we can leave the cflags configuration here
      #          However, when we do then it must be moved to our common module and/or just commented out
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-multichar")
    endif()
  endif()

  check_c_compiler_flag(-Wdeclaration-after-statement HAVE_GCC_WDECLARATION_AFTER_STATEMENT)
  if(HAVE_GCC_WDECLARATION_AFTER_STATEMENT)
    check_c_compiler_flag(-Werror=declaration-after-statement HAVE_GCC_WERROR_DECLARATION_AFTER_STATEMENT)
    if(HAVE_GCC_WERROR_DECLARATION_AFTER_STATEMENT)
      list(APPEND EXTRA_CFLAGS "-Werror=declaration-after-statement")
    endif()
    list(APPEND EXTRA_CFLAGS "-Wdeclaration-after-statement")
  endif()

  if(DEPENDENCY_TRACKING)
    check_c_source_compiles("
        #if !defined(__GNUC__) || __GNUC__ < 3
        #error Dependency tracking requires GCC 3.0 or newer
        #endif
        int main(int argc, char **argv) { }" HAVE_DEPENDENCY_TRACKING)
  endif()

  if(GCC_ATOMICS)
    check_c_source_compiles("int main(int argc, char **argv) {
        int a;
        void *x, *y, *z;
        __sync_lock_test_and_set(&a, 4);
        __sync_lock_test_and_set(&x, y);
        __sync_fetch_and_add(&a, 1);
        __sync_bool_compare_and_swap(&a, 5, 10);
        __sync_bool_compare_and_swap(&x, y, z); }" HAVE_GCC_ATOMICS)
    if(NOT HAVE_GCC_ATOMICS)
      check_c_source_compiles("int main(int argc, char **argv) {
          int a;
          __sync_lock_test_and_set(&a, 1);
          __sync_lock_release(&a); }" HAVE_GCC_SYNC_LOCK_TEST_AND_SET)
    endif()
  endif()

  set(CMAKE_REQUIRED_FLAGS "-mpreferred-stack-boundary=2 ${ORIG_CMAKE_REQUIRED_FLAGS}")
  check_c_source_compiles("int x = 0; int main(int argc, char **argv) {}"
    HAVE_GCC_PREFERRED_STACK_BOUNDARY)
  set(CMAKE_REQUIRED_FLAGS ${ORIG_CMAKE_REQUIRED_FLAGS})

# Urho3D - we rely on GenerateExportHeader CMake module for configuring GCC/Clang visibility attribute support

  check_c_compiler_flag(-Wshadow HAVE_GCC_WSHADOW)
  if(HAVE_GCC_WSHADOW)
    list(APPEND EXTRA_CFLAGS "-Wshadow")
  endif()

  set(CMAKE_REQUIRED_FLAGS "-Wl,--no-undefined ${ORIG_CMAKE_REQUIRED_FLAGS}")
  check_c_compiler_flag("" HAVE_NO_UNDEFINED)
  set(CMAKE_REQUIRED_FLAGS ${ORIG_CMAKE_REQUIRED_FLAGS})
  if(HAVE_NO_UNDEFINED)
    list(APPEND EXTRA_LDFLAGS "-Wl,--no-undefined")
  endif()
endif()

if(ASSEMBLY)
  if(USE_GCC OR USE_CLANG)
    set(SDL_ASSEMBLY_ROUTINES 1)
    # TODO: Those all seem to be quite GCC specific - needs to be
    # reworked for better compiler support
    set(HAVE_ASSEMBLY TRUE)

    # Urho3D - move the altivec, mmx, 3dnow, sse and sse2 checks to Urho3D common module which then configure the compiler flags globally

    # Urho3D - bug fix - do not use "-mfpmath=387" in EXTRA_CFLAGS, instead let the compiler to choose what's the best by itself (i.e. '387' for x86 and 'sse' for x86_64) and also to keep SDL library being built with the same option as Urho3D library and all the other 3rd-party sub-libraries; besides the '387' is invalid when targeting arm64

    # Urho3D - commented out the HAVE_SSEMATH variable as it is not being used anywhere currently; furthermore compiler already emits __SSE_MATH__ or __SSE2_MATH__ as necessary on x86_64 ABI only; so instead of using HAVE_SSEMATH variable, we should actually use the __SSE_MATH__ or __SSE2_MATH__ compiler define if that is the original intention of having this variable

    if(URHO3D_ALTIVEC)
      set(SDL_ALTIVEC_BLITTERS 1)
    endif()
  elseif(MSVC_VERSION GREATER 1500)
    # TODO: SDL_cpuinfo.h needs to support the user's configuration wish
    # Urho3D - move the HAVE_MMX, HAVE_3DNOW, HAVE_SSE and HAVE_SSE2 initialization to Urho3D common module
    set(SDL_ASSEMBLY_ROUTINES 1)
  endif()
endif()

# Urho3D - move from Unix platform-specific section below to here
if(CLOCK_GETTIME)
  # Urho3D - bug fix - use different variables for different checks because of CMake caches the result variable; this fix the detection on Android platform
  check_library_exists(rt clock_gettime "" FOUND_CLOCK_GETTIME_IN_RT)
  if(FOUND_CLOCK_GETTIME_IN_RT)
    list(APPEND EXTRA_LIBS rt)
    set(HAVE_CLOCK_GETTIME 1)
  else()
    check_library_exists(c clock_gettime "" FOUND_CLOCK_GETTIME_IN_C)
    if(FOUND_CLOCK_GETTIME_IN_C)
      set(HAVE_CLOCK_GETTIME 1)
    endif()
  endif()
endif()

# TODO: Can't deactivate on FreeBSD? w/o LIBC, SDL_stdinc.h can't define
# anything.
if(LIBC)
  if(WINDOWS AND NOT MINGW)
    set(HAVE_LIBC TRUE)
    foreach(_HEADER stdio.h string.h ctype.h math.h)
      string(TOUPPER "HAVE_${_HEADER}" _UPPER)
      string(REPLACE "." "_" _HAVE_H ${_UPPER})
      set(${_HAVE_H} 1)
    endforeach()
    set(HAVE_SIGNAL_H 1)
    foreach(_FN
            malloc calloc realloc free qsort abs memset memcpy memmove memcmp
            strlen _strrev _strupr _strlwr strchr strrchr strstr itoa _ltoa
            _ultoa strtol strtoul strtoll strtod atoi atof strcmp strncmp
            _stricmp _strnicmp sscanf atan atan2 acos asin ceil copysign cos
            cosf fabs floor log pow scalbn sin sinf sqrt sqrtf tan tanf)
      string(TOUPPER ${_FN} _UPPER)
      set(HAVE_${_UPPER} 1)
    endforeach()
    if(NOT CYGWIN AND NOT MINGW)
      set(HAVE_ALLOCA 1)
    endif()
    set(HAVE_M_PI 1)
    add_definitions(-D_USE_MATH_DEFINES) # needed for M_PI
    set(STDC_HEADERS 1)
  else()
    set(HAVE_LIBC TRUE)
    # Urho3D - temporary bug fix - starting since Raspbian Jessie some of the headers are installed in the arch-specific directory and it seems that older CMake (prior to release 3.2.3) does not include this directory by default yet, so we have to do that manually
    if (RPI AND CMAKE_LIBRARY_ARCHITECTURE)
      include_directories (${SYSROOT}/usr/include/${CMAKE_LIBRARY_ARCHITECTURE})    # SYSROOT is empty when we are not cross-compiling
>>>>>>> 8eb652a... Squashed 'Source/ThirdParty/SDL/' changes from a901827..fe37f87
    endif ()
endif ()

# Set project name
project (Urho3D)

<<<<<<< HEAD
# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
=======
  if(SDL_AUDIO)
    set(SDL_AUDIO_DRIVER_COREAUDIO 1)
    file(GLOB AUDIO_SOURCES ${SDL2_SOURCE_DIR}/src/audio/coreaudio/*.m)
    set(SOURCE_FILES ${SOURCE_FILES} ${AUDIO_SOURCES})
    set(HAVE_SDL_AUDIO TRUE)
    set(SDL_FRAMEWORK_COREAUDIO 1)
    set(SDL_FRAMEWORK_AUDIOTOOLBOX 1)
  endif()
>>>>>>> 8eb652a... Squashed 'Source/ThirdParty/SDL/' changes from a901827..fe37f87

# Include Urho3D Cmake common module
include (Urho3D-CMake-common)

# Setup SDK install destinations
set (PATH_SUFFIX Urho3D)
if (WIN32)
    set (SCRIPT_EXT .bat)
    if (CMAKE_HOST_WIN32)
        set (PATH_SUFFIX .)
    endif ()
else ()
    set (SCRIPT_EXT .sh)
endif ()
if (ANDROID)
    # For Android platform, install to a path similar to ANDROID_LIBRARY_OUTPUT_PATH variable, e.g. libs/armeabi-v7a
    set (LIB_SUFFIX s/${ANDROID_NDK_ABI_NAME})
elseif (URHO3D_64BIT)
    # Install to 'lib64' when one of these conditions is true
    if ((MINGW AND CMAKE_CROSSCOMPILING) OR URHO3D_USE_LIB64_RPM OR (HAS_LIB64 AND NOT URHO3D_USE_LIB_DEB))
        set (LIB_SUFFIX 64)
    endif ()
<<<<<<< HEAD
endif ()
set (DEST_INCLUDE_DIR include/Urho3D)    # The include directory path contains the 'Urho3D' suffix regardless of PATH_SUFFIX variable
set (DEST_SHARE_DIR share/${PATH_SUFFIX})
set (DEST_BUNDLE_DIR ${DEST_SHARE_DIR}/Applications)
# Note to package maintainer: ${PATH_SUFFIX} for library could be removed if the extra path is not desired, but if so then the RPATH setting in Source's CMakeLists.txt needs to be adjusted accordingly
set (DEST_LIBRARY_DIR lib${LIB_SUFFIX}/${PATH_SUFFIX})
set (DEST_PKGCONFIG_DIR lib${LIB_SUFFIX}/pkgconfig)
# Install application launcher scripts
file (GLOB APP_SCRIPTS ${CMAKE_SOURCE_DIR}/bin/*${SCRIPT_EXT})
install (PROGRAMS ${APP_SCRIPTS} DESTINATION ${DEST_RUNTIME_DIR})   # DEST_RUNTIME_DIR variable is set by the set_output_directories() macro call in the Urho3D-CMake-common module
# Install resource directories required by applications built with Urho3D library
install (DIRECTORY ${CMAKE_SOURCE_DIR}/bin/Autoload ${CMAKE_SOURCE_DIR}/bin/pfiles DESTINATION ${DEST_SHARE_DIR}/Resources)
# Install CMake modules and toolchains provided by and for Urho3D
install (DIRECTORY ${CMAKE_SOURCE_DIR}/CMake/ DESTINATION ${DEST_SHARE_DIR}/CMake)    # Note: the trailing slash is significant
# Install CMake build scripts
file (GLOB CMAKE_SCRIPTS ${CMAKE_SOURCE_DIR}/*${SCRIPT_EXT})
install (PROGRAMS ${CMAKE_SCRIPTS} DESTINATION ${DEST_SHARE_DIR}/Scripts)

# Setup package variables
set (URHO3D_DESCRIPTION "Urho3D is a free lightweight, cross-platform 2D and 3D game engine implemented in C++ and released under the MIT license. Greatly inspired by OGRE (http://www.ogre3d.org) and Horde3D (http://www.horde3d.org).")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${URHO3D_DESCRIPTION})
set (URHO3D_URL "https://github.com/urho3d/Urho3D")
set (CPACK_PACKAGE_VENDOR ${URHO3D_URL})
set (CPACK_PACKAGE_CONTACT ${URHO3D_URL})
execute_process (COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CMake/Modules/GetUrho3DRevision.cmake WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE URHO3D_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
set (CPACK_PACKAGE_VERSION ${URHO3D_VERSION})
string (REGEX MATCH "([^.]+)\\.([^.]+)\\.(.+)" MATCHED ${URHO3D_VERSION})
if (MATCHED)
    set (CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
    set (CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
    set (CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})
endif ()
set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/License.txt)
set (CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set (CPACK_GENERATOR TGZ)
if (ANDROID)
    set (CPACK_SYSTEM_NAME Android)
elseif (IOS)
    set (CPACK_SYSTEM_NAME iOS)
elseif (APPLE)
    set (CPACK_SYSTEM_NAME OSX)
elseif (WIN32)
    set (CPACK_GENERATOR ZIP)
elseif (WEB)
    set (CPACK_SYSTEM_NAME Web)
elseif (CPACK_SYSTEM_NAME STREQUAL Linux)
    if (RPI)
        set (CPACK_SYSTEM_NAME Raspberry-Pi)
    elseif (ARM)
        set (CPACK_SYSTEM_NAME ARM)     # Generic ARM
    endif ()
    if (NOT URHO3D_64BIT OR HAS_LIB64)
        list (APPEND CPACK_GENERATOR RPM)
    endif ()
    if (NOT URHO3D_64BIT OR NOT HAS_LIB64)
        list (APPEND CPACK_GENERATOR DEB)
=======
    set(SOURCE_FILES ${SOURCE_FILES} ${POWER_SOURCES})
    set_source_files_properties(${POWER_SOURCES} PROPERTIES LANGUAGE C)
    set(HAVE_SDL_POWER TRUE)
  endif()

  if(SDL_FILESYSTEM)
    set(SDL_FILESYSTEM_COCOA 1)
    file(GLOB FILESYSTEM_SOURCES ${SDL2_SOURCE_DIR}/src/filesystem/cocoa/*.m)
    set_source_files_properties(${FILESYSTEM_SOURCES} PROPERTIES LANGUAGE C)
    set(SOURCE_FILES ${SOURCE_FILES} ${FILESYSTEM_SOURCES})
    set(HAVE_SDL_FILESYSTEM TRUE)
  endif()

  # Actually load the frameworks at the end so we don't duplicate include.
  # Urho3D - TODO - this list in complete, do this when we refactor the library dependency handling
  if(SDL_FRAMEWORK_COREVIDEO)
    find_library(COREVIDEO CoreVideo)
    list(APPEND EXTRA_LIBS ${COREVIDEO})
  endif()
  if(SDL_FRAMEWORK_COCOA)
    find_library(COCOA_LIBRARY Cocoa)
    list(APPEND EXTRA_LIBS ${COCOA_LIBRARY})
  endif()
  if(SDL_FRAMEWORK_IOKIT)
    find_library(IOKIT IOKit)
    list(APPEND EXTRA_LIBS ${IOKIT})
  endif()
  if(SDL_FRAMEWORK_FF)
    find_library(FORCEFEEDBACK ForceFeedback)
    list(APPEND EXTRA_LIBS ${FORCEFEEDBACK})
  endif()
  if(SDL_FRAMEWORK_CARBON)
    find_library(CARBON_LIBRARY Carbon)
    list(APPEND EXTRA_LIBS ${CARBON_LIBRARY})
  endif()
  if(SDL_FRAMEWORK_COREAUDIO)
    find_library(COREAUDIO CoreAudio)
    list(APPEND EXTRA_LIBS ${COREAUDIO})
  endif()
  if(SDL_FRAMEWORK_AUDIOTOOLBOX)
    find_library(AUDIOTOOLBOX AudioToolbox)
    list(APPEND EXTRA_LIBS ${AUDIOTOOLBOX})
  endif()

  # Urho3d - bug fix - make it work for iOS platform
  if(SDL_VIDEO)
    if (VIDEO_COCOA)
      # Urho3D - these are moved from commented CheckCOCOA macro
      set(SDL_VIDEO_DRIVER_COCOA 1)
      file(GLOB COCOA_SOURCES ${SDL2_SOURCE_DIR}/src/video/cocoa/*.m)
      set_source_files_properties(${COCOA_SOURCES} PROPERTIES LANGUAGE C)
      set(SOURCE_FILES ${SOURCE_FILES} ${COCOA_SOURCES})
      set(HAVE_SDL_VIDEO TRUE)
    elseif (IOS OR TVOS)
      set (SDL_VIDEO_DRIVER_UIKIT 1)
      file (GLOB UIKIT_SOURCES ${SDL2_SOURCE_DIR}/src/video/uikit/*.m)
      set_source_files_properties (${UIKIT_SOURCES} PROPERTIES LANGUAGE C)
      set (SOURCE_FILES ${SOURCE_FILES} ${UIKIT_SOURCES})
      set (HAVE_SDL_VIDEO TRUE)
>>>>>>> 8eb652a... Squashed 'Source/ThirdParty/SDL/' changes from a901827..fe37f87
    endif ()
    if (URHO3D_64BIT)
        if (URHO3D_USE_LIB64_RPM AND NOT HAS_LIB64)
            set (CPACK_GENERATOR RPM)
        elseif (URHO3D_USE_LIB_DEB AND HAS_LIB64)
            set (CPACK_GENERATOR DEB)
        endif ()
    endif ()
    # Note to package maintainer: comment out below line to revert the prefix from '/usr/local' (default for CMAKE_INSTALL_PREFIX) to '/usr' (default for CPACK_PACKAGING_INSTALL_PREFIX)
    set (CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
endif ()
if (URHO3D_64BIT)
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-64bit)
else ()
    set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
    set (CPACK_RPM_PACKAGE_ARCHITECTURE i686)       # The 'package' target should be built with the help of 'setarch i686' command on a 64-bit host system
endif ()
set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-${URHO3D_LIB_TYPE})
if (WIN32 AND NOT URHO3D_OPENGL)
    if (URHO3D_D3D11)
        set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-3D11)   # The artifact name has a space constraint on our website when viewed in a mobile browser, 3D11 stands for Direct3D 11
    else ()
        set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-3D9)
    endif ()
elseif (ANDROID AND X86)    # Take advantage of Android toolchain setting X86 variable to true for both 'x86' and 'x86_64' ABIs
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-IA)     # Stands for Intel Architecture
elseif (RPI AND RPI_ABI MATCHES ^armeabi-v7a)
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-v7a)
elseif (ARM_ABI_FLAGS)
    string (REGEX REPLACE "^.*mcpu=([^ ]+).*$" -\\1 CPU_NAME "${ARM_ABI_FLAGS}")
    string (REGEX REPLACE .*- - CPU_NAME "${CPU_NAME}")
    string (TOUPPER "${CPU_NAME}" CPU_NAME)
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}${CPU_NAME})
endif ()
if (NOT DEFINED ENV{RELEASE_TAG})
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-snapshot)
endif ()
include (CPack)

# Setup MacOSX and iOS bundle variables
if (IOS OR URHO3D_MACOSX_BUNDLE)
    if (NOT MACOSX_BUNDLE_ICON_FILE)
        set (MACOSX_BUNDLE_ICON_FILE UrhoIcon)
    endif ()
    if (NOT MACOSX_BUNDLE_BUNDLE_VERSION)
        set (MACOSX_BUNDLE_BUNDLE_VERSION ${URHO3D_VERSION})
    endif ()
<<<<<<< HEAD
    if (NOT MACOSX_BUNDLE_LONG_VERSION_STRING)
        set (MACOSX_BUNDLE_LONG_VERSION_STRING ${URHO3D_VERSION})
    endif ()
    if (NOT MACOSX_BUNDLE_SHORT_VERSION_STRING)
        set (MACOSX_BUNDLE_SHORT_VERSION_STRING ${URHO3D_VERSION})
    endif ()
    if (NOT MACOSX_BUNDLE_COPYRIGHT)
        set (MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2008-2016 the Urho3D project.")
    endif ()
endif ()
=======
    # Urho3D - bug fix - dbus/dbus.h is installed under path suffix 'dbus-1.0', so the following find_path() is needed even when not cross-compiling
    find_path (DBUS_H_INCLUDE_DIRS NAMES dbus/dbus.h PATH_SUFFIXES dbus-1.0)
    # Cater for both multilib and multiarch, native and cross-compiling build
    if (URHO3D_64BIT)
      set (DBUS_INC_SEARCH_PATH /usr/lib64/dbus-1.0/include)
    else ()
      set (DBUS_INC_SEARCH_PATH /usr/lib/dbus-1.0/include)
    endif ()
    find_path (DBUS_ARCH_DEPS_H_INCLUDE_DIRS NAMES dbus/dbus-arch-deps.h PATHS ${DBUS_INC_SEARCH_PATH} /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/dbus-1.0/include)
    set (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES_UDEV_DBUS_SAVED} ${DBUS_H_INCLUDE_DIRS} ${DBUS_ARCH_DEPS_H_INCLUDE_DIRS})
    check_include_file("dbus/dbus.h" HAVE_DBUS_DBUS_H)
    if (HAVE_DBUS_DBUS_H AND DBUS_H_INCLUDE_DIRS AND DBUS_ARCH_DEPS_H_INCLUDE_DIRS)
      include_directories (${DBUS_H_INCLUDE_DIRS} ${DBUS_ARCH_DEPS_H_INCLUDE_DIRS})
      list(APPEND EXTRA_LIBS dbus-1)
    endif ()
    set (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES_UDEV_DBUS_SAVED})

    # Urho3D - TODO - add ibus and fcitx support

    # Urho3D - bug fix - moved below logic from generic Unix block to Linux-specific block
    if(SDL_POWER)
      set(SDL_POWER_LINUX 1)
      file(GLOB POWER_SOURCES ${SDL2_SOURCE_DIR}/src/power/linux/*.c)
      set(SOURCE_FILES ${SOURCE_FILES} ${POWER_SOURCES})
      set(HAVE_SDL_POWER TRUE)
    endif()
    if(SDL_JOYSTICK)
      set(SDL_JOYSTICK_LINUX 1)
      file(GLOB JOYSTICK_SOURCES ${SDL2_SOURCE_DIR}/src/joystick/linux/*.c)
      set(SOURCE_FILES ${SOURCE_FILES} ${JOYSTICK_SOURCES})
      set(HAVE_SDL_JOYSTICK TRUE)
    endif()
  else ()
    if(SDL_JOYSTICK)
      CheckUSBHID()   # seems to be BSD specific - limit the test to BSD only?
    endif()
  endif()

  if(INPUT_TSLIB)
    # Urho3D - bug fix - when cross-compiling the headers are rooted, either use "--sysroot" compiler flag or use CMAKE_REQUIRED_INCLUDES (e.g. on RPI) to cater for it
    set (CMAKE_REQUIRED_INCLUDES_TSLIB_SAVED ${CMAKE_REQUIRED_INCLUDES})
    if (CMAKE_CROSSCOMPILING AND NOT "${CMAKE_C_FLAGS} ${CMAKE_REQUIRED_FLAGS}" MATCHES sysroot)
      find_path (TSLIB_H_INCLUDE_DIRS NAMES tslib.h)
      if (TSLIB_H_INCLUDE_DIRS)
        # Assume the header search path has not been adjusted elsewhere yet, there is no harm anyway when a same entry is added twice into the list
        list (APPEND CMAKE_REQUIRED_INCLUDES ${TSLIB_H_INCLUDE_DIRS})
      endif ()
    endif ()
    check_include_file (tslib.h HAVE_INPUT_TSLIB)
    if(HAVE_INPUT_TSLIB)
      set(SDL_INPUT_TSLIB 1)
      list(APPEND EXTRA_LIBS ts)
    endif()
    set (CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES_TSLIB_SAVED})
  endif()

  # Urho3D - move CLOCK_GETTIME detection logic to compiler option evaluation section above

  # Urho3D - commented out setting of HAVE_LINUX_VERSION_H compiler define via cflags as that define is not being used at all, besides we prefer add_definitions()

  # Urho3D - bug fix - do not use Unix filesystem for Android platform (Android platform should not be reaching this if-else block now)
  if(SDL_FILESYSTEM)
    set(SDL_FILESYSTEM_UNIX 1)
    file(GLOB FILESYSTEM_SOURCES ${SDL2_SOURCE_DIR}/src/filesystem/unix/*.c)
    set(SOURCE_FILES ${SOURCE_FILES} ${FILESYSTEM_SOURCES})
    set(HAVE_SDL_FILESYSTEM TRUE)
  endif()

  # Urho3D - move Unix timer to general section above as it can be used for a number of Unix-like platforms

  if(RPATH)
    set(SDL_RLD_FLAGS "")
    if(BSDI OR FREEBSD OR LINUX OR NETBSD)
      set(CMAKE_REQUIRED_FLAGS "-Wl,--enable-new-dtags ${ORIG_CMAKE_REQUIRED_FLAGS}")
      check_c_compiler_flag("" HAVE_ENABLE_NEW_DTAGS)
      set(CMAKE_REQUIRED_FLAGS ${ORIG_CMAKE_REQUIRED_FLAGS})
      if(HAVE_ENABLE_NEW_DTAGS)
        set(SDL_RLD_FLAGS "-Wl,-rpath,\${libdir} -Wl,--enable-new-dtags")
      else()
        set(SDL_RLD_FLAGS "-Wl,-rpath,\${libdir}")
      endif()
    elseif(SOLARIS)
      set(SDL_RLD_FLAGS "-R\${libdir}")
    endif()
    # Urho3D - commented out RPATH setup as we have configured ours globally
    set(HAVE_RPATH TRUE)
  endif()

elseif(WINDOWS)
  # Urho3D - bug fix - when using MinGW in cross-compiling build, the windres tool is being prefixed as other GCC cross-compiling tools
  if (MINGW)
    if (CMAKE_CROSSCOMPILING)
      set (WINDRES ${CMAKE_RC_COMPILER})  # The CMAKE_RC_COMPILER variable is already initialized in our CMake/MinGW toolchain file
    else ()
      find_program(WINDRES windres)
    endif ()
  endif ()

  # Urho3D - replace the windows.h check by using check_include_file() instead of check_c_source_files()
  check_include_file (windows.h HAVE_WIN32_CC)

  file(GLOB CORE_SOURCES ${SDL2_SOURCE_DIR}/src/core/windows/*.c)
  set(SOURCE_FILES ${SOURCE_FILES} ${CORE_SOURCES})

  # Urho3D - commented out MSVC-specific compiler flags setup as we have configured MSVC compiler flags globally in Urho3D common module

  # Check for DirectX
  if(DIRECTX)
    # Urho3D - bug fix - use our own FindDirectX.cmake module which does not rely on DXSDK_DIR environment variable directly when DirectX SDK is being used
    # Urho3D - TODO - move the find_package(DirectX) call from Urho3D common module to here later after we have refactored the library dependency handling
  endif()

  if(SDL_AUDIO)
    set(SDL_AUDIO_DRIVER_WINMM 1)
    file(GLOB WINMM_AUDIO_SOURCES ${SDL2_SOURCE_DIR}/src/audio/winmm/*.c)
    set(SOURCE_FILES ${SOURCE_FILES} ${WINMM_AUDIO_SOURCES})
    set(HAVE_SDL_AUDIO TRUE)

    if(HAVE_DSOUND_H)
      set(SDL_AUDIO_DRIVER_DSOUND 1)
      file(GLOB DSOUND_AUDIO_SOURCES ${SDL2_SOURCE_DIR}/src/audio/directsound/*.c)
      set(SOURCE_FILES ${SOURCE_FILES} ${DSOUND_AUDIO_SOURCES})
    endif()

    if(HAVE_XAUDIO2_H)
      set(SDL_AUDIO_DRIVER_XAUDIO2 1)
      file(GLOB XAUDIO2_AUDIO_SOURCES ${SDL2_SOURCE_DIR}/src/audio/xaudio2/*.c)
      set(SOURCE_FILES ${SOURCE_FILES} ${XAUDIO2_AUDIO_SOURCES})
    endif()
  endif()

  if(SDL_VIDEO)
    # requires SDL_LOADSO on Windows (IME, DX, etc.)
    if(NOT SDL_LOADSO)
      message_error("SDL_VIDEO requires SDL_LOADSO, which is not enabled")
    endif()
    set(SDL_VIDEO_DRIVER_WINDOWS 1)
    file(GLOB WIN_VIDEO_SOURCES ${SDL2_SOURCE_DIR}/src/video/windows/*.c)
    set(SOURCE_FILES ${SOURCE_FILES} ${WIN_VIDEO_SOURCES})

    if(RENDER_D3D AND HAVE_D3D_H)
      set(SDL_VIDEO_RENDER_D3D 1)
      set(HAVE_RENDER_D3D TRUE)
    endif()
    if(RENDER_D3D AND HAVE_D3D11_H)
      set(SDL_VIDEO_RENDER_D3D11 1)
      set(HAVE_RENDER_D3D TRUE)
    endif()
    set(HAVE_SDL_VIDEO TRUE)
    # Urho3D - bug fix - we are going to need this later when we refactor the library dependency handling
    list (APPEND EXTRA_LIBS ${DIRECT3D_LIBRARIES})    # Empty when none of the D3D components is used
  endif()

  if(SDL_THREADS)
    set(SDL_THREAD_WINDOWS 1)
    set(SOURCE_FILES ${SOURCE_FILES}
      ${SDL2_SOURCE_DIR}/src/thread/windows/SDL_sysmutex.c
      ${SDL2_SOURCE_DIR}/src/thread/windows/SDL_syssem.c
      ${SDL2_SOURCE_DIR}/src/thread/windows/SDL_systhread.c
      ${SDL2_SOURCE_DIR}/src/thread/windows/SDL_systls.c
      ${SDL2_SOURCE_DIR}/src/thread/generic/SDL_syscond.c)
    set(HAVE_SDL_THREADS TRUE)
  endif()

  if(SDL_POWER)
    set(SDL_POWER_WINDOWS 1)
    set(SOURCE_FILES ${SOURCE_FILES} ${SDL2_SOURCE_DIR}/src/power/windows/SDL_syspower.c)
    set(HAVE_SDL_POWER TRUE)
  endif()

  if(SDL_FILESYSTEM)
    set(SDL_FILESYSTEM_WINDOWS 1)
    file(GLOB FILESYSTEM_SOURCES ${SDL2_SOURCE_DIR}/src/filesystem/windows/*.c)
    set(SOURCE_FILES ${SOURCE_FILES} ${FILESYSTEM_SOURCES})
    set(HAVE_SDL_FILESYSTEM TRUE)
  endif()

  # Libraries for Win32 native and MinGW
  list(APPEND EXTRA_LIBS user32 gdi32 winmm imm32 ole32 oleaut32 version uuid)

  # TODO: in configure.in the check for timers is set on
  # cygwin | mingw32* - does this include mingw32CE?
  if(SDL_TIMERS)
    set(SDL_TIMER_WINDOWS 1)
    file(GLOB TIMER_SOURCES ${SDL2_SOURCE_DIR}/src/timer/windows/*.c)
    set(SOURCE_FILES ${SOURCE_FILES} ${TIMER_SOURCES})
    set(HAVE_SDL_TIMERS TRUE)
  endif()

  if(SDL_LOADSO)
    set(SDL_LOADSO_WINDOWS 1)
    file(GLOB LOADSO_SOURCES ${SDL2_SOURCE_DIR}/src/loadso/windows/*.c)
    set(SOURCE_FILES ${SOURCE_FILES} ${LOADSO_SOURCES})
    set(HAVE_SDL_LOADSO TRUE)
  endif()

  file(GLOB CORE_SOURCES ${SDL2_SOURCE_DIR}/src/core/windows/*.c)
  set(SOURCE_FILES ${SOURCE_FILES} ${CORE_SOURCES})

  if(SDL_VIDEO)
    if(VIDEO_OPENGL)
      set(SDL_VIDEO_OPENGL 1)
      set(SDL_VIDEO_OPENGL_WGL 1)
      set(SDL_VIDEO_RENDER_OGL 1)
      set(HAVE_VIDEO_OPENGL TRUE)
    endif()

    if(VIDEO_OPENGLES)
      set(SDL_VIDEO_OPENGL_EGL 1)
      set(SDL_VIDEO_OPENGL_ES2 1)
      set(SDL_VIDEO_RENDER_OGL_ES2 1)
      set(HAVE_VIDEO_OPENGLES TRUE)
    endif()
  endif()

  if(SDL_JOYSTICK)
    file(GLOB JOYSTICK_SOURCES ${SDL2_SOURCE_DIR}/src/joystick/windows/*.c)
    set(SOURCE_FILES ${SOURCE_FILES} ${JOYSTICK_SOURCES})
    if(HAVE_DINPUT_H)
      set(SDL_JOYSTICK_DINPUT 1)
      list(APPEND EXTRA_LIBS dinput8)
      if(CMAKE_COMPILER_IS_MINGW)
        list(APPEND EXTRA_LIBS dxerr8)
      elseif (NOT USE_WINSDK_DIRECTX)
        list(APPEND EXTRA_LIBS dxerr)
      endif()
    endif()
    if(HAVE_XINPUT_H)
      set(SDL_JOYSTICK_XINPUT 1)
    endif()
    if(NOT HAVE_DINPUT_H AND NOT HAVE_XINPUT_H)
      set(SDL_JOYSTICK_WINMM 1)
    endif()
    set(HAVE_SDL_JOYSTICK TRUE)

    if(SDL_HAPTIC)
      if(HAVE_DINPUT_H OR HAVE_XINPUT_H)
        file(GLOB HAPTIC_SOURCES ${SDL2_SOURCE_DIR}/src/haptic/windows/*.c)
        if(HAVE_DINPUT_H)
          set(SDL_HAPTIC_DINPUT 1)
        endif()
        if(HAVE_XINPUT_H)
          set(SDL_HAPTIC_XINPUT 1)
        endif()
      else()
        file(GLOB HAPTIC_SOURCES ${SDL2_SOURCE_DIR}/src/haptic/dummy/*.c)
        set(SDL_HAPTIC_DUMMY 1)
      endif()
      set(SOURCE_FILES ${SOURCE_FILES} ${HAPTIC_SOURCES})
      set(HAVE_SDL_HAPTIC TRUE)
    endif()
  endif()

  # Urho3D - commented out adding main entry point for Windows platform
  # Urho3D - commented out '-mwindows' linker flags setup as it appears to be automatically added by CMake; also commented out 'mingw32' library dependency as it does not appear to be needed, at least that the case for MinGW
endif()

# Dummies
# configure.in does it differently:
# if not have X
#   if enable_X {  SDL_X_DISABLED = 1 }
#   [add dummy sources]
# so it always adds a dummy, without checking, if it was actually requested.
# This leads to missing internal references on building, since the
# src/X/*.c does not get included.
if(NOT HAVE_SDL_JOYSTICK)
  set(SDL_JOYSTICK_DISABLED 1)
  if(SDL_JOYSTICK AND NOT APPLE) # results in unresolved symbols on OSX
    file(GLOB JOYSTICK_SOURCES ${SDL2_SOURCE_DIR}/src/joystick/dummy/*.c)
    set(SOURCE_FILES ${SOURCE_FILES} ${JOYSTICK_SOURCES})
  endif()
endif()
if(NOT HAVE_SDL_HAPTIC)
  set(SDL_HAPTIC_DISABLED 1)
  file(GLOB HAPTIC_SOURCES ${SDL2_SOURCE_DIR}/src/haptic/dummy/*.c)
  set(SOURCE_FILES ${SOURCE_FILES} ${HAPTIC_SOURCES})
endif()
if(NOT HAVE_SDL_LOADSO)
  set(SDL_LOADSO_DISABLED 1)
  file(GLOB LOADSO_SOURCES ${SDL2_SOURCE_DIR}/src/loadso/dummy/*.c)
  set(SOURCE_FILES ${SOURCE_FILES} ${LOADSO_SOURCES})
endif()
if(NOT HAVE_SDL_FILESYSTEM)
  set(SDL_FILESYSTEM_DISABLED 1)
  file(GLOB FILESYSTEM_SOURCES ${SDL2_SOURCE_DIR}/src/filesystem/dummy/*.c)
  set(SOURCE_FILES ${SOURCE_FILES} ${FILESYSTEM_SOURCES})
endif()

# We always need to have threads and timers around
if(NOT HAVE_SDL_THREADS)
  set(SDL_THREADS_DISABLED 1)
  file(GLOB THREADS_SOURCES ${SDL2_SOURCE_DIR}/src/thread/generic/*.c)
  set(SOURCE_FILES ${SOURCE_FILES} ${THREADS_SOURCES})
endif()
if(NOT HAVE_SDL_TIMERS)
  set(SDL_TIMERS_DISABLED 1)
  file(GLOB TIMER_SOURCES ${SDL2_SOURCE_DIR}/src/timer/dummy/*.c)
  set(SOURCE_FILES ${SOURCE_FILES} ${TIMER_SOURCES})
endif()

# Urho3D - commented out inclusion of dummy main

# Append the -MMD -MT flags
# if(DEPENDENCY_TRACKING)
#   if(COMPILER_IS_GNUCC)
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -MMD -MT \$@")
#   endif()
# endif()

# Urho3D - use 'generated' path suffix for generated config header file (need the suffix as we support both out-of-source in-source build tree)
configure_file("${SDL2_SOURCE_DIR}/include/SDL_config.h.cmake"
  "${SDL2_BINARY_DIR}/include/generated/SDL_config.h")

# Prepare the flags and remove duplicates
if(EXTRA_LDFLAGS)
  list(REMOVE_DUPLICATES EXTRA_LDFLAGS)
endif()
if(EXTRA_LIBS)
  list(REMOVE_DUPLICATES EXTRA_LIBS)
endif()
if(EXTRA_CFLAGS)
  list(REMOVE_DUPLICATES EXTRA_CFLAGS)
endif()
listtostr(EXTRA_CFLAGS _EXTRA_CFLAGS)
set(EXTRA_CFLAGS ${_EXTRA_CFLAGS})

# Compat helpers for the configuration files
if(NOT WINDOWS OR CYGWIN)
  # Urho3D - commeted out the call to updaterev.sh script as it won't work on any platform as it assumes hg to be the version source control which is not true in our case

  set(prefix ${CMAKE_INSTALL_PREFIX})
  set(exec_prefix "\${prefix}")
  set(libdir "\${exec_prefix}/lib${LIB_SUFFIX}")
  set(bindir "\${exec_prefix}/bin")
  set(includedir "\${prefix}/include")
  if(SDL_STATIC)
    set(ENABLE_STATIC_TRUE "")
    set(ENABLE_STATIC_FALSE "#")
  else()
    set(ENABLE_STATIC_TRUE "#")
    set(ENABLE_STATIC_FALSE "")
  endif()
  if(SDL_SHARED)
    set(ENABLE_SHARED_TRUE "")
    set(ENABLE_SHARED_FALSE "#")
  else()
    set(ENABLE_SHARED_TRUE "#")
    set(ENABLE_SHARED_FALSE "")
  endif()

  # Clean up the different lists
  listtostr(EXTRA_LIBS _EXTRA_LIBS "-l")
  set(SDL_STATIC_LIBS ${SDL_LIBS} ${EXTRA_LDFLAGS} ${_EXTRA_LIBS})
  list(REMOVE_DUPLICATES SDL_STATIC_LIBS)
  listtostr(SDL_STATIC_LIBS _SDL_STATIC_LIBS)
  set(SDL_STATIC_LIBS ${_SDL_STATIC_LIBS})
  listtostr(SDL_LIBS _SDL_LIBS)
  set(SDL_LIBS ${_SDL_LIBS})

  # Urho3D - commented out pkg-config configuration file generation
  # Urho3D - TODO - utilize the populated variables for pkg-config configuration file generation for Urho3D.pc
endif()

# Urho3D - restore the original CMake global settings
set (CMAKE_REQUIRED_FLAGS ${ORIG_CMAKE_REQUIRED_FLAGS})
set (CMAKE_REQUIRED_INCLUDES ${ORIG_CMAKE_REQUIRED_INCLUDES})
set (CMAKE_REQUIRED_LIBRARIES ${ORIG_CMAKE_REQUIRED_LIBRARIES})

# Urho3D - only echo the status once during initial configuration
if (NOT SDL_INFO_ECHOED_STATUS)
  set (SDL_INFO_ECHOED_STATUS TRUE CACHE INTERNAL "SDL info echoed status")
##### Info output #####
message(STATUS "")
message(STATUS "SDL2 was configured with the following options:")
message(STATUS "")
message(STATUS "Platform: ${CMAKE_SYSTEM}")
message(STATUS "64-bit:   ${ARCH_64}")
message(STATUS "Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "")
message(STATUS "Subsystems:")
foreach(_SUB ${SDL_SUBSYSTEMS})
  string(TOUPPER ${_SUB} _OPT)
  # Urho3D - use the same amount of spaces for padding
  string(LENGTH ${_SUB} _SUBLEN)
  math(EXPR _PADLEN "11 - ${_SUBLEN}")
  string(RANDOM LENGTH ${_PADLEN} ALPHABET " " _PADDING)
  message_bool_option(${_SUB} SDL_${_OPT} ${_PADDING})
endforeach()
message(STATUS "")
message(STATUS "Options:")
list(SORT ALLOPTIONS)
foreach(_OPT ${ALLOPTIONS})
  # Longest option is VIDEO_X11_XSCREENSAVER = 22 characters
  # Get the padding
  string(LENGTH ${_OPT} _OPTLEN)
  math(EXPR _PADLEN "23 - ${_OPTLEN}")
  string(RANDOM LENGTH ${_PADLEN} ALPHABET " " _PADDING)
  message_tested_option(${_OPT} ${_PADDING})
endforeach()
message(STATUS "")
message(STATUS " CFLAGS:        ${CMAKE_C_FLAGS}")
message(STATUS " EXTRA_CFLAGS:  ${EXTRA_CFLAGS}")
message(STATUS " EXTRA_LDFLAGS: ${EXTRA_LDFLAGS}")
message(STATUS " EXTRA_LIBS:    ${EXTRA_LIBS}")
message(STATUS "")
# Urho3D - commented out messages informing user to set the CFLAGS and LDFLAGS environment variables to workaround detection problems, our modified version does not support that
endif (NOT SDL_INFO_ECHOED_STATUS)

# Ensure that the extra cflags are used at compile time
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")

# Urho3D - commented out targets defined by original CMakeLists.txt, instead we setup our own target that meets our own needs

# Urho3D - anything below this line is Urho3D specific
>>>>>>> 8eb652a... Squashed 'Source/ThirdParty/SDL/' changes from a901827..fe37f87

# Setup SDK-like include dir in the build tree for building the Urho3D library
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${DEST_INCLUDE_DIR}/ThirdParty)

# Urho3D source
add_subdirectory (Source)

