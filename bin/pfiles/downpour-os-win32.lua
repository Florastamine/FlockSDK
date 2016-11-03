--[[
Copyright (c) 2016, Florastamine

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------
This module contains core definitions for the Downpour OS API for Windows.
You should not use this interface when making OS calls. Instead, it's generally recommended that you use
the higher, OS-independent interface provided in downpour-os.
------------------------------------------
Dependencies:
[X] downpour
[X] downpour-os
[X] downpour-io (If the safe switch is enabled in <downpour-os> through safe().)
------------------------------------------
]]--

local downpour_os = require("./downpour-os")

downpour_os.win32 = {}

downpour_os.win32 = {
    ["__log_handle__"] = 0,
    ["__init__"] = false,
    ["__ffi_handle__"] = nil,
    ["__user32_handle__"] = nil,
    ["__def_table__"] =
    [[
    typedef void * HANDLE;
    typedef HANDLE HWND;
    typedef const char * LPCSTR;
    typedef char * LPSTR;
    typedef unsigned UINT;
    typedef unsigned long DWORD;
    typedef DWORD * LPDWORD;
    typedef long LONG;
    typedef bool BOOL;

    typedef struct {
        long x;
        long y;
    } POINT;

    typedef POINT * LPPOINT;

    enum {
        MB_OK = 0x00000000L,
        MB_ICONINFORMATION = 0x00000040L
    };

    int MessageBoxA(HWND, LPCSTR, LPCSTR, UINT);
    BOOL GetCursorPos(LPPOINT);
    ]]
}

downpour_os.win32["__get_log_handle"] = function ()
    return downpour_os.win32["__log_handle__"]
end

downpour_os.win32["init"] = function ()
    if downpour_os.is_safe() then
        downpour_os.win32["__log_handle__"] = require("./downpour-io")
        local log = downpour_os.win32.__get_log_handle()
        if log then
            log.file.log.write("%s", "[!] - downpour.osutils.win32.init(): Initializing the Windows subsystem...")
        end
    end

    -- We will initialize the Windows subsystem using a waterfall-like model.
    -- For each successive initialization of one of the systems, we will try to load another subsystem which depends on its parent.
    -- For example, we will load the LuaJIT's FFI library first, and if the loading completes, we will try to feed the definition
    -- table into the FFI handle and load the user32 DLL (which depends on FFI).
    downpour_os.win32["__ffi_handle__"] = require("ffi")

    if downpour_os.win32["__ffi_handle__"] then -- Load FFI
        downpour_os.win32["__ffi_handle__"].cdef(downpour_os.win32["__def_table__"])
        downpour_os.win32["__user32_handle__"] = downpour_os.win32["__ffi_handle__"].load("user32")

        -- TODO: Add more verbose error messages.
        if downpour_os.win32["__user32_handle__"] then
            if downpour_os.win32["__init__"] == false then
                if downpour_os.is_safe() and downpour_os.win32.__get_log_handle() then
                    (downpour_os.win32.__get_log_handle()).file.log.write("%s", "[!] - downpour.osutils.win32.init(): Windows subsystem successfully initialized.")
                end

                downpour_os.win32["__init__"] = true
                return true
            end
        end
    end
end

downpour_os.win32["is_init"] = function()
    return downpour_os.win32["__init__"]
end

-- MessageBoxA() wrapper
downpour_os.win32["alert"] = function (title, message)
    downpour_os.win32["__user32_handle__"].MessageBoxA(nil, title, message, downpour_os.win32["__ffi_handle__"].C.MB_OK + downpour_os.win32["__ffi_handle__"].C.MB_ICONINFORMATION)
end

-- GetCursorPos() wrapper
downpour_os.win32["GetCursorPos"] = function ()
    local t = downpour_os.win32["__ffi_handle__"].new("POINT[?]", 1)
    downpour_os.win32["__user32_handle__"].GetCursorPos(t)

    return t[0].x, t[0].y
end -- // downpour_os.win32 = {}
