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
This module contains core definitions for the Downpour OS API for Linux.
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

downpour_os.linux = {}

downpour_os.linux = {
    ["__log_handle__"] = 0,
    ["__init__"] = false
}

local __os_table__ = {
    ["xmessage"] = "/usr/bin/xmessage"
}

downpour_os.linux["__get_log_handle"] = function ()
    return downpour_os.linux["__log_handle__"]
end

downpour_os.linux["init"] = function ()
    if downpour_os.is_safe() then
        downpour_os.linux["__log_handle__"] = require("./downpour-io")
        local log = downpour_os.linux.__get_log_handle()
        if log then
            log.file.log.write("%s", "[!] - downpour.osutils.linux.init(): Initializing the Linux subsystem...")
        end
    end

    if downpour_os.linux["__init__"] == false then
        if downpour_os.is_safe() and downpour_os.linux.__get_log_handle() then
            (downpour_os.linux.__get_log_handle()).file.log.write("%s", "[!] - downpour.osutils.linux.init(): Linux subsystem successfully initialized.")
        end

        downpour_os.linux["__init__"] = true
        return true
    end
end

downpour_os.linux["is_init"] = function ()
    return downpour_os.linux["__init__"]
end

downpour_os.linux["alert"] = function (title, message)
    local args = __os_table__["xmessage"] .. " " .. title .. " " .. message

    os.execute(args)
end -- // downpour_os.linux = {}
