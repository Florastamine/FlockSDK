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
This module contains core definitions for the Downpour OS API.

The Downpour OS API officially supports Windows and Linux-based popular distributions.
For example, alert() does have bindings for both Linux and Windows.

You can call (mostly) all functions available from the OS API without qualifying its
platform beforehand (except functions that are available on only one platform and not
on another). For example, you can type downpour.osutils.alert()
instead of downpour.osutils.win32.alert(), provided that you've called init() to let
Downpour detect your platform prior to calling OS functions.
------------------------------------------
Dependencies:
[X] downpour
------------------------------------------
+ get_name()
++ Retrieves the current OS family in use, not taking in account other factors (OS version, architecture, ...)
------------------------------------------
+ init()
++ Detects and initializes the current OS platform's API calls.
++ Must be called prior to calling general, platform-independent functions (see below).
------------------------------------------
+ alert(title, message)
++ Sends a custom message through a GUI-based dialog box. On Windows, MessageBoxA() is used. On Linux, it's /usr/bin/xmessage.
]]--

local downpour = require("./downpour")

downpour.osutils = {}

downpour.osutils = {
    ["__safe__"] = false
}

local __os_table__ = {
    ["win32"] = "windows/win32/win64",
    ["linux"] = "linux/linux32/linux64",
    ["other"] = "undetermined"
}

downpour.osutils["safe"] = function (switch)
    downpour.osutils["__safe__"] = switch
end

downpour.osutils["is_safe"] = function ()
    return downpour.osutils["__safe__"]
end

downpour.osutils["get_name"] = function ()
    local v = package.config:sub(1, 1)

    if v == '\\' then
        return __os_table__["win32"]
    elseif v == '/' then
        return __os_table__["linux"]
    else
        return __os_table__["other"]
    end
end

downpour.osutils["__get_platform_call"] = function (caller)
    if caller ~= nil then
        local s = 'downpour' .. '.osutils'
        local v = downpour.osutils.get_name()

        if v:find("windows") ~= nil then
            s = s .. ".win32" .. tostring(caller)
            return loadstring("return " .. s .. "(...)")
        elseif v:find("linux") ~= nil then
            s = s .. ".linux" .. tostring(caller)
            return loadstring("return " .. s .. "(...)")
        else
        end
    end
end

downpour.osutils["alert"] = function (title, message)
    local v = downpour.osutils.__get_platform_call(".alert")(title, message)

    if v ~= nil then
        return v
    end
end

downpour.osutils["init"] = function ()
    local os_name = downpour.osutils.get_name()

    if os_name:find("windows") ~= nil then
        return downpour.osutils.win32.init()
    elseif os_name:find("linux") ~= nil then
        return downpour.osutils.linux.init()
    else
        return downpour.osutils.dummy.init()
    end
end

return downpour.osutils -- // downpour.osutils = {}
