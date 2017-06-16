//
// Copyright (c) 2008-2017 Flock SDK developers & contributors. 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "../Container/Str.h"

#include <cstdlib>

namespace FlockSDK
{

class Mutex;

/// Initialize the FPU to round-to-nearest, single precision mode.
FLOCKSDK_API void InitFPU();
/// Display an error dialog with the specified title and message.
FLOCKSDK_API void ErrorDialog(const String &title, const String &message);
/// Exit the application with an error message to the console.
FLOCKSDK_API void ErrorExit(const String &message = String::EMPTY, int exitCode = EXIT_FAILURE);
/// Open a console window.
FLOCKSDK_API void OpenConsoleWindow();
/// Print Unicode text to the console. Will not be printed to the MSVC output window.
FLOCKSDK_API void PrintUnicode(const String &str, bool error = false);
/// Print Unicode text to the console with a newline appended. Will not be printed to the MSVC output window.
FLOCKSDK_API void PrintUnicodeLine(const String &str, bool error = false);
/// Print ASCII text to the console with a newline appended. Uses printf() to allow printing into the MSVC output window.
FLOCKSDK_API void PrintLine(const String &str, bool error = false);
/// Parse arguments from the command line. First argument is by default assumed to be the executable name and is skipped.
FLOCKSDK_API const Vector<String> &ParseArguments(const String &cmdLine, bool skipFirstArgument = true);
/// Parse arguments from the command line.
FLOCKSDK_API const Vector<String> &ParseArguments(const char* cmdLine);
/// Parse arguments from a wide char command line.
FLOCKSDK_API const Vector<String> &ParseArguments(const WString &cmdLine);
/// Parse arguments from a wide char command line.
FLOCKSDK_API const Vector<String> &ParseArguments(const wchar_t* cmdLine);
/// Parse arguments from argc & argv.
FLOCKSDK_API const Vector<String> &ParseArguments(int argc, char** argv);
/// Return previously parsed arguments.
FLOCKSDK_API const Vector<String> &GetArguments();
/// Read input from the console window. Return empty if no input.
FLOCKSDK_API String GetConsoleInput();
/// Return the runtime platform identifier, one of "Windows", "Linux", "Mac OS X", "Android", "iOS", "Web" or "Raspberry Pi".
FLOCKSDK_API String GetPlatform();
/// Return the number of physical CPU cores.
FLOCKSDK_API unsigned GetNumCPUCores();
/// Return the total number of threads (number of physical CPUs * logical CPUs of each physical one). 
FLOCKSDK_API unsigned GetNumCPUThreads();
/// Return the total amount of useable RAM that is available to the OS. (not the amount of physically installed memory!)
FLOCKSDK_API unsigned long long GetTotalMemory(); 
/// Return the currently logged in user. 
FLOCKSDK_API String GetLoginName();
/// Return the machine's name. 
FLOCKSDK_API String GetHostName(); 
/// Return current OS version. On Windows, only the official (marketed) version is returned and not the "real" version. Linux returns the kernel version and 
/// on macOS/Mac OS X, the official version number is returned as well as the Darwin kernel version. 
FLOCKSDK_API String GetOSVersion(); 
/// Return path to the home directory (on Linux) or desktop directory (on Windows).
FLOCKSDK_API String GetHomePath(); 
/// Return path to user-specific temporary directory (on Windows) or "/tmp/" (on Linux, most likely). 
FLOCKSDK_API String GetTemporaryPath(); 
/// Return true if the current machine is operating under big endian mode, and vice versa. 
FLOCKSDK_API bool GetCPUBigEndian();
/// Return true if the current machine is operating under little endian mode, and vice versa. 
FLOCKSDK_API bool GetCPULittleEndian();
/// Return the CPU frequency/clock/speed. (Note: not the current frequency!) 
FLOCKSDK_API unsigned long long GetCPUClock();
/// Return the CPU architecture. 
FLOCKSDK_API String GetCPUArchitecture();
/// Return supported extensions of the current CPU. 
FLOCKSDK_API String GetCPUExtensions(); 
/// Return the vendor ID of the current CPU. 
FLOCKSDK_API String GetCPUVendorID(); 
/// Return the brand name of the CPU. 
FLOCKSDK_API String GetCPUBrandName();
/// Retrieve the system's current locale. 
FLOCKSDK_API String GetLocale();
/// Return true if the environment variable in question exists on the target system. 
FLOCKSDK_API bool HasEnvVar(const String &var);
/// Retrieve the value of a given environment variable. 
FLOCKSDK_API String GetEnvVar(const String &var);
/// Retrieve the current content of the clipboard. Only supports Windows at the moment. 
FLOCKSDK_API String GetClipboard();
}
