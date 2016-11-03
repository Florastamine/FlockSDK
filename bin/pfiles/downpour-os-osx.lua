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
This module contains core definitions for the Downpour OS API for OSX.

This is just a placeholder header as there are no plans of supporting OSX
at the moment.

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

downpour_os.osx = {}

downpour_os.osx = {
    ["__log_handle__"] = 0,
    ["__init__"] = false
}

downpour_os.osx["__get_log_handle"] = function ()
    return downpour_os.osx["__log_handle__"]
end

downpour_os.osx["init"] = function ()
    if downpour_os.is_safe() then
        downpour_os.osx["__log_handle__"] = require("./downpour-io")
        local log = downpour_os.osx.__get_log_handle()
        if log then
            log.file.log.write("%s", "[!] - downpour.osutils.osx.init(): OSX isn't supported at the moment, fail to initialize the OSX subsystem.")
        end
    end

    return false

    --[[
    if downpour_os.osx["__init__"] == false then
        if downpour_os.is_safe() and downpour_os.osx.__get_log_handle() then
            (downpour_os.osx.__get_log_handle()).file.log.write("%s", "[!] - downpour.osutils.osx.init(): OSX subsystem successfully initialized.")
        end

        downpour_os.osx["__init__"] = true
        return true
    end
    ]]--
end

downpour_os.osx["is_init"] = function ()
    return downpour_os.osx["__init__"]
end -- // downpour_os.osx = {}
