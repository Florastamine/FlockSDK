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
This module implements a very simple mechanism for detecting integrity of binary files using MD5.

It is _NOT_ intended for cryptography purposes, as MD5 is broken already in this aspect, and the
implementation of <downpour-md5> is very simple and only guarantees integrity checking of files
(so you can always test if the player has done something evil to the game files and punish them for that).
Besides, why not CRC32? Because 32 bits is too short to guarantee a good collision probability.
------------------------------------------
Dependencies:
[X] glue
[X] md5 (+ md5.dll)
------------------------------------------
+ safe(val)
++ Enables explicit logging for this module. If any of the calls in the (sub)module in which safe() was enabled behaves
++ unexpectedly, the error message will be logged into a file on the physical disk in order to be viewed later.
------------------------------------------
+ is_safe()
++ Returns true if the current module does have explicitly logging enabled, false otherwise.
------------------------------------------
+ write_hash(file, overwrite)
++ Opens a binary file and writes its hash value (under hexadecimal form) at the end of the binary.
++ The (optional) second parameter is a boolean specifies whether write_hash() should create a .bak version
++ of the file before attempting to modify the original file. By default, the function always creates a backup
++ version of the file before appending its checksum at the end of the original file.
------------------------------------------
+ read_hash(file)
++ Opens a binary file and returns its hash value (under hexadecimal form), which lies at the last 32 bytes of the file.
++ The file must have its hash written before using write_hash().
------------------------------------------
+ compare_hash(file)
++ Re-calculates the hash of the given file and compares it against the file's stored hash. Returns true if two hashes match, false otherwise.
------------------------------------------
TODO:
- Write get_hash(file) which retrieves the hash of a given file without writing hash value to the file. (May render compare_hash() obsolete
as now it can be implemented as a get_hash() combined with read_hash().)
- Write is_hash_written(file) which checks if the file has got its hash values written before (using write_hash()).
- Update write_hash(file, overwrite) to check if the back up file exists and take appropriate actions.
]]--

local downpour = require("./downpour")
local glue     = require("./lj/glue")
local md5      = require("./lj/md5")

downpour.ioutils.md5 = {}

downpour.ioutils.md5 = {
    ["__safe__"] = false,
    ["binary_hash_size"] = 16
}

downpour.ioutils.md5["safe"] = function (switch)
    downpour.ioutils.md5["__safe__"] = switch
end

downpour.ioutils.md5["is_safe"] = function ()
    return downpour.ioutils.md5["__safe__"]
end

downpour.ioutils.md5["write_hash"] = function (file, overwrite)
    local safe = downpour.ioutils.md5.is_safe()
    local log  = downpour.ioutils.file.log.write

    if downpour.ioutils.file.exists(file) then
        overwrite = overwrite or false

        -- If we've explicitly passed the overwrite flag to the second parameter slot,
        -- then clone the current file.
        if overwrite then
            local backup_file = file .. ".bak"
            local r = downpour.ioutils.file.copy_binary(file, backup_file)

            if not r then
                if safe then
                    log("[!] - downpour.ioutils.md5.write_hash(): Cannot perform back up, failed to write to the target file. (%s)", backup_file)
                end
            end
        end

        -- Open our file, calculates its MD5 hash and append it to the end of the file.
        local f = io.open(file, "r+b") -- Somehow, w+b doesn't work with appending contents to a binary file.
                                       -- There is an explanation on StackOverflow here: http://stackoverflow.com/questions/12103674/appending-binary-files

        if f then
            local buffer = f:read(downpour.ioutils.file.size(file))

            f:seek("end")
            f:write(md5.sum(buffer))

            f:flush()
            f:close()
        else
            if safe then
                log("[!] - downpour.ioutils.md5.write_hash(): Cannot open the specified file for writing hash. (%s)", file)
            end
        end
    else
        if safe then
            log("[!] - downpour.ioutils.md5.write_hash(): The specified file (%s) cannot be found.", file)
        end
    end

    return false
end

downpour.ioutils.md5["read_hash"] = function (file)
    local safe = downpour.ioutils.md5.is_safe()
    local log  = downpour.ioutils.file.log.write

    local hash = 0

    if downpour.ioutils.file.exists(file) then
        local f = io.open(file, "rb")

        if f then
            -- Seek to the last 16 bytes of binary data (specified in binary_hash_size) and read everything from there, which is the stored hash of the file.
            f:seek("set", downpour.ioutils.file.size(file) - downpour.ioutils.md5.binary_hash_size)
            hash = f:read(tonumber(downpour.ioutils.md5.binary_hash_size))
            f:close()
        else
            if safe then
                log("[!] - downpour.ioutils.md5.read_hash(): Cannot open the target file (%s) for reading hash value.", file)
            end
        end
    else
        if safe then
            log("[!] - downpour.ioutils.md5.read_hash(): The specified file (%s) cannot be found.", file)
        end
    end

    return glue.tohex(hash)
end

downpour.ioutils.md5["compare_hash"] = function (file)
    local safe = downpour.ioutils.md5.is_safe()
    local log  = downpour.ioutils.file.log.write

    if downpour.ioutils.file.exists(file) then
        local f = io.open(file, "rb")

        -- Re-calculate our file's hash value, then compare the hash with its previously stored one.
        if f then
            local buffer = f:read(downpour.ioutils.file.size(file) - downpour.ioutils.md5.binary_hash_size)
            f:close()

            -- This is our previously stored hash, which we've got using read_hash().
            local stored_hash = downpour.ioutils.md5.read_hash(file)

            -- This is the newly created hash value, calculated from our data buffer.
            local new_hash    = glue.tohex(md5.sum(buffer))

            if stored_hash == new_hash then
                return true
            end
        else
            if safe then
                log("[!] - downpour.ioutils.md5.compare_hash(): Cannot open the target file (%s) for comparing hash value.", file)
            end
        end
    else
        if safe then
            log("[!] - downpour.ioutils.md5.compare_hash(): The specified file (%s) cannot be found.", file)
        end
    end

    return false
end

return downpour.ioutils.md5 -- // downpour.ioutils.md5
