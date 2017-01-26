/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file SDL_platform.h
 *
 *  Try to get a standard set of platform defines.
 */

#ifndef _SDL_platform_h
#define _SDL_platform_h

#if defined(bsdi) || defined(__bsdi) || defined(__bsdi__)
#undef __BSDI__
#define __BSDI__    1
#endif
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
#undef __FREEBSD__
#define __FREEBSD__ 1
#endif
#if (defined(linux) || defined(__linux) || defined(__linux__))
#undef __LINUX__
#define __LINUX__   1
#endif

#if defined(__NetBSD__)
#undef __NETBSD__
#define __NETBSD__  1
#endif
#if defined(__OpenBSD__)
#undef __OPENBSD__
#define __OPENBSD__ 1
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__)
/* Try to find out if we're compiling for WinRT or non-WinRT */
/* If _USING_V110_SDK71_ is defined it means we are using the v110_xp or v120_xp toolset. */
#if (defined(_MSC_VER) && (_MSC_VER >= 1700) && !_USING_V110_SDK71_)	/* _MSC_VER==1700 for MSVC 2012 */
#include <winapifamily.h>
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#undef __WINDOWS__
#define __WINDOWS__   1
/* See if we're compiling for WinRT: */
#elif WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#undef __WINRT__
#define __WINRT__ 1
#endif
#else
#undef __WINDOWS__
#define __WINDOWS__   1
#endif /* _MSC_VER < 1700 */
#endif /* defined(WIN32) || defined(_WIN32) || defined(__CYGWIN__) */

#if defined(__WINDOWS__)
#undef __WIN32__
#define __WIN32__ 1
#endif

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief Gets the name of the platform.
 */
extern DECLSPEC const char * SDLCALL SDL_GetPlatform (void);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif /* _SDL_platform_h */

/* vi: set ts=4 sw=4 expandtab: */