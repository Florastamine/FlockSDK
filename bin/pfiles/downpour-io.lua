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
This module contains core definitions for the Downpour IO API, mostly extensions to built-in Lua functions.
------------------------------------------
Dependencies:
[X] downpour
[X] lfs (downpour.ioutils.file.get_list_unpack(); downpour.ioutils.file.clone())
------------------------------------------
+ get_version()
++ Retrieves the current version number of the Downpour IO API.
------------------------------------------
+ printf(fs, ...)
++ Prints information to the standard output channel. Note that, printing to the output channels is, generally
++ slow, because they do little to no buffering. To avoid this behaviour, you may want to adjust the buffering mode
++ of stdout, for example: io.stdout:setvbuf("full") to tell stdout that it should only flush the buffer if only buffer is full.
------------------------------------------
+ ternary(cond, t, f)
++ Mimics the ternary operator in the C-like languages, which Lua currently lacks.
++ Returns f if cond evaluates to either false or nil, t otherwise.
------------------------------------------
+ safe(val)
++ Enables explicit logging for this module. If any of the calls in the (sub)module in which safe() was enabled behaves
++ unexpectedly, the error message will be logged into a file on the physical disk in order to be viewed later.
------------------------------------------
+ is_safe()
++ Returns true if the current module does have explicitly logging enabled, false otherwise.
------------------------------------------
+ get_script_name()
++ Returns the currently running script name. (full path included)
++ In Lua 5.2 or later, you probably don't need this function as there is one way to acquire the script name
++ through the return value of require().
------------------------------------------
+ numericutils.from_bool(bool)
++ Converts the specified boolean value into its integer equivalent: 0 if the value is false, or 1 otherwise.
------------------------------------------
+ numericutils.is_digit(string)
++ Returns true if the specified string is a valid digit [0-9]. If the string contains more one character,
++ the function will only take out the very first character of the string sequence, and everything else will be ignored.
------------------------------------------
+ stringutils.from_bool(bool)
++ Same as numericutils.from_bool(), but returns the value of the boolean under the string form instead.
++ You can do the same with ternary(): local r = downpour.ioutils.ternary(bool ~= false, "true", "false")
------------------------------------------
+ stringutils.split(_string, token)
+ stringutils.parse(_string, token)
++ Splits a string into smaller substrings, separated by token, and return a table containing the splitted strings.
++ By default, the token is the whitespace character (" ").
++ This function behaves similarly to C's strtok(), except it does everything in one call.
++ parse() is an alias for split() (which does exactly the same thing).
------------------------------------------ 
+ stringutils.match(_string, rpattern, method) 
++ A (complete?) replacement of C's strtok() (or C++'s std::strtok()). It offers various methods for searching substrings but doesn't 
++ offer some kind of iterator which can be found in strtok(), so you're pretty much stuck because it only returns the very first matched 
++ result. It also doesn't any provide support for pattern matching (just raw strings), which a lot of Lua string functions take 
++ advantage of (this is intentional). 
------------------------------------------
+ stringutils.match_table(_string, _table, _table_length)
++ Returns true (and the position of the item in _table) if _string matches one of the items in table _table, false otherwise.
++ _table_length specifies the maximum number of items to be searched for in _table, which can be suppressed (match_table() will then
++ use the length of the table instead.)
------------------------------------------  
+ stringutils.get_char(_string, i)
++ Extracts a character from the string, at index i, starting from 1. Index's default value is 1.
++ You can use string.sub() to achieve the same: local c = s:sub(i, i) (not tested :p, but it will most likely work)
------------------------------------------
+ tableutils.is_key_valid(_t, key)
++ Tests if the specified key is in the table _t without creating one.
------------------------------------------
+ file.log.get_name()/file.log.set_name()
++ Retrieves/Sets the location and name of the log file. (you can combine everything, for example: set_name("./folder/file.log")
------------------------------------------
+ file.log.new()/file.log.free()
++ Creates/destroys the log file object. Before anything can be written into the log file, it has to be opened first.
++ So new() does exactly that, and free() does the reverse: closes the log file.
------------------------------------------
+ file.log.is_valid()
++ Checks if the log file object has already been opened.
------------------------------------------
+ file.set_temp_ext()/file.get_temp_ext()
++ Sets/Gets the extension of the files which will be created as temporary handles. (see below: file.new_temp())
------------------------------------------
+ file.new_temp()/file.free_temp()
++ Creates a new, unnamed temporary file which you can use to write temporary data, and returns its handle
++ (so normal I/O functionalities can be used, like write()/read()/flush() and so on).
++ It's similar to io.tmpfile(), but you get to explicitly decide where and when to free the file and you
++ get to know the file's name (file.new_temp() returns the handle of the file and the file name).
++ Example:
++ local handle, name = downpour.ioutils.file.new_temp()
++ -- *store name elsewhere*
++ -- *do things with handle*
++ handle:setvbuf("no")
++ handle:write("This string stream will be flushed immediately.")
++
++ For better performance, remember to append a "/" at the end of the passed path, so Downpour
++ will not have to create a new string which contains one (which takes time).
------------------------------------------
+ downpour.ioutils.file.size(file, factor)
++ Returns the size of a file, which will be divided with factor. The returned result is bytes
++ and the factor by default is 1. Thus, passing a factor of 1024 will return the result as KBs, because, for example, 1024 bytes
++ divided by 1024 is 1 KB.
------------------------------------------
+ downpour.ioutils.file.copy()/downpour.ioutils.file.copy_binary()
++ Copies a file (copy()) or a binary file (binary_copy()) into another one, overwriting the
++ destination file. On Windows systems, you may want to use copy() for ordinary files and copy_binary()
++ for binary files, but in Linux-based systems you can call  copy() for both file types as Linux doesn't
++ distinguish between file types.
------------------------------------------
+ downpour.ioutils.file.get_ext()
++ Retrieves a given file name's extension. Note that this function may not work well under Linux, as hidden files/folders
++ have in additional a leading dot character (.) at the beginning of file names, thus using get_ext() on those items does not
++ return nil, but their full names instead. (TODO: fix it)
------------------------------------------
+ downpour.ioutils.file.get_list_unpack(path, ext, func)
++ Recursively scans a folder and lists all files that matches a certain extension. The second parameter can be omitted to 
++ perform a full scan. Additionally, the third parameter specifies a user-made function which takes one argument to process 
++ the scanned file. (i. e., removing the files that matches a certain extension while iterating through the list.)
++ 
++ get_list() comes in two flavours: 
+++ get_list_unpack(), which scans for and returns two tables, one containing a list of found files and the other contains a list of found directories.
+++ get_list_pack(), which does the same as get_list_unpack(), but it returns a merged list instead (and thus returns only one table). 
+++ It uses get_list_unpack() internally. 
+++ get_list() is the shorthand form of get_list_pack(). 
------------------------------------------
+ downpour.ioutils.file.exists(name)
++ Returns true if the given file exists, false otherwise. 
------------------------------------------
+ downpour.ioutils.file.clone(src, dest)/downpour.ioutils.file.mirror(src, dest) 
++ Clones/mirrors the whole directory hierarchy inside src to dest, including files and subfolders inside src. 
++ Both the source directory and the destination directory must exist. 
------------------------------------------
]]--

local downpour = require("./downpour")
local lfs = require("lfs")

downpour.ioutils = {}

downpour.ioutils = {
    ["__safe__"] = false
}

-- TODO: Make this function accepts fs as a table and try to parse data from it.
downpour.ioutils["printf"] = function (fs, ...)
    if fs ~= nil and type(fs) == "string" then
        io.stdout:write(string.format(fs, ...))
    end
end

downpour.ioutils["ternary"] = function(cond, t, f)
    if (cond) == nil or (cond) == false then return f end
    return t
end

downpour.ioutils["safe"] = function (switch)
    downpour.ioutils["__safe__"] = switch
end

downpour.ioutils["is_safe"] = function ()
    return downpour.ioutils["__safe__"]
end

downpour.ioutils["get_script_name"] = function ()
    local block = debug.getinfo(1, "S")

    return block.source
end

downpour.ioutils.numericutils = {}

downpour.ioutils.numericutils["__normalize"] = function (n) 
	return (n % 0x80000000)
end 

downpour.ioutils.numericutils["bit_and"] = function (a, b) 
	local r, m = 0, 0
	for m = 0, 31 do
		if (a % 2 == 1) and (b % 2 == 1) then
			r = r + 2 ^ m
		end
		if a % 2 ~= 0 then a = a - 1 end
		if b % 2 ~= 0 then b = b - 1 end
	end 

	return downpour.ioutils.numericutils.__normalize(r) 
end 

downpour.ioutils.numericutils["bit_or"] = function (a, b) 
	local r, m = 0, 0
	for m = 0, 31 do
		if (a % 2 == 1) and (b % 2 == 1) then
			r = r + 2 ^ m
		end
		if a % 2 ~= 0 then a = a - 1 end
		if b % 2 ~= 0 then b = b - 1 end
		a = a / 2
		b = b / 2
	end

	return downpour.ioutils.numericutils.__normalize(r) 
end 

downpour.ioutils.numericutils["bit_xor"] = function (a, b) 
	local r, m = 0, 0
	for m = 0, 31 do
		if a % 2 ~= b % 2 then r = r + 2 ^ m end
		if a % 2 ~= 0 then a = a - 1 end
		if b % 2 ~= 0 then b = b - 1 end
		a = a / 2
		b = b / 2
	end

	return downpour.ioutils.numericutils.__normalize(r)  
end 

downpour.ioutils.numericutils["from_bool"] = function (bool)
    if bool ~= nil and type(bool) == "boolean" then
        if bool ~= true then
            do return 0 end
        end

        return 1
    end

    return 0
end

downpour.ioutils.numericutils["is_digit"] = function (cchar)
    if (not cchar) or (type(cchar) ~= "string") then
        if downpour.ioutils.is_safe() then
            downpour.ioutils.file.log.write("%s", "[!] - downpour.ioutils.numericutils.is_digit(): Bad argument passed.")
        end
    end

    local range = string.byte(cchar, 1)

    if range >= 48 and range <= 57 then
        return true
    end

    return false
end -- // downpour.ioutils.numericutils = {}

downpour.ioutils.stringutils = {} 

downpour.ioutils.stringutils["from_bool"] = function (bool)
    if downpour.ioutils.numericutils(bool) == 1 then
        return "true"
    end

    return "false"
end

downpour.ioutils.stringutils["split"] = function (_string, token)
    if (not _string or not token) or (type(_string) ~= "string" or type(token) ~= "string") then
        if downpour.ioutils.is_safe() then
            downpour.ioutils.file.log.write("%s", "[!] - downpour.ioutils.stringutils.split(): Bad argument passed.")
        end

        return nil
    end

    token = token or " "
    local ret, t = {}, 1
    for _string in string.gmatch(_string, "([^" .. token .. "]+)") do
        ret[t] = tostring(_string)
        t = t + 1
    end

    return ret
end

downpour.ioutils.stringutils["parse"] = function (_string, token)
    return downpour.ioutils.stringutils.split(_string, token)
end

downpour.ioutils.stringutils["match_table"] = function (_string, _table, _table_length)
    local fallout = (type(_string) ~= "string" or type(_table_length) ~= "number" or type(_table) ~= "table") or ((not _table) or #_table <= 0)

    if fallout then
        if downpour.ioutils.is_safe() then
            downpour.ioutils.file.log.write("%s", "[!] - downpour.ioutils.stringutils.match_table(): Bad argument passed.")
        end
    else
        _string       = _string or tostring(_table[1])
        _table_length = _table_length or #_table

        for k, v in ipairs(_table) do
            if k <= _table_length then
                if _string == v then
                    return true, k
                end
            end
        end
    end

    return false, -1
end 

downpour.ioutils.stringutils["match"] = function (_string, rpattern, method)  
    local start_index, end_index, string_length = -1, -1, -1 

    if _string ~= nil and type(_string) == "string" then 
        method = method or "double_loop" 
        rpattern = rpattern or tostring(_string) 

        -- The simplest method to understand and implement is the double loop method (I made up this term myself). 
        -- Basically, it iterates over all elements of the given string, and continuously checks if one of the character 
        -- in the string matches the very first character of the pattern string. If they matches, it will continue to compare  
        -- consecutive characters from the pattern with the string (starting from the position where the first matched character 
        -- was found). If they all matches, then the pattern is considered a substring of the given string. 
        if method == "double_loop" then 
            local j, k, plen = 0, 1, rpattern:len()

            for i = 1, _string:len() do 
                if (_string:byte(i) == rpattern:byte(1)) and ((i + plen - 1) <= _string:len()) then  
                    j = i 
                    k = 1 
                    while _string:byte(j) == rpattern:byte(k) do  
                        j = j + 1
                        k = k + 1
                    end 

                    if k - 1 == plen then 
                        start_index = i 
                        end_index = i + plen 
                        string_length = k 

                        break  
                    end 
                end  
            end 
        else 
            -- Other methods of substring searching/matching should be implemented here. 
        end 

    else 
        if downpour.ioutils.is_safe() then  
            downpour.ioutils.file.log.write("%s", "[!] - downpour.ioutils.stringutils.match(): Bad argument passed.") 
        end 
    end 

    return start_index, end_index, string_length 
end 

downpour.ioutils.stringutils["get_char"] = function (_string, index)
    if (not _string) or type(_string) ~= "string" then
        if downpour.ioutils.is_safe() then
            downpour.ioutils.file.log.write("%s", "[!] - downpour.ioutils.stringutils.get_char(): Bad argument passed.");
        end

        return nil
    end

    index = downpour.ioutils.ternary(index <= #_string and index >= 1, index, 1) or 1

    return string.char(string.byte(_string, index))
end -- // downpour.ioutils.stringutils = {}

downpour.ioutils.tableutils = {} 

downpour.ioutils.tableutils["is_key_valid"] = function (_t, key)
    if _t and key and type(_t) == "table" then
        for _, k in pairs(_t) do
            if k == key then return true end
        end
    end

    return false
end -- // downpour.ioutils.tableutils = {}

downpour.ioutils.file = {}

downpour.ioutils.file = {
    ["__tmpname_ext__"] = "000"
}

downpour.ioutils.file.log = {}

downpour.ioutils.file.log = {
    ["__name__"] = "./downpour.log",
    ["__handle__"] = 0
}

downpour.ioutils.file.log["get_name"] = function ()
    return downpour.ioutils.file.log["__name__"]
end

downpour.ioutils.file.log["set_name"] = function (name)
    if name then
        downpour.ioutils.file.log["__name__"] = tostring(name)
    end
end

--[[
downpour.ioutils.file.log["get_handle"] = function ()
    return downpour.ioutils.file.log["__handle__"]
end
]]--

downpour.ioutils.file.log["new"] = function ()
    downpour.ioutils.file.log["__handle__"], hexception = io.open(downpour.ioutils.file.log["__name__"], "w+")

    if not hexception then
        downpour.ioutils.file.log["__handle__"]:setvbuf("line")

        local date = os.date()
        local s = string.format("[%s]: Logging channel opened.\n[%s]: %s\n", date, date, downpour.get_version_extended())
        downpour.ioutils.file.log["__handle__"]:write(s)

        return true
    end

    return false
end

downpour.ioutils.file.log["write"] = function(fs, ...)
    local s = downpour.ioutils.file.log["__handle__"]

    if s ~= nil then
        s:write(string.format("[%s]: %s\n", os.date(), string.format(fs, ...)))
    end
end

downpour.ioutils.file.log["free"] = function ()
    local r = downpour.ioutils.file.log["__handle__"]

    if r then
        r:flush()
        r:close()

        downpour.ioutils.file.log["__handle__"] = 0
    end
end

downpour.ioutils.file.log["is_valid"] = function ()
    return downpour.ioutils.ternary(downpour.ioutils.file.log["__handle__"] ~= 0, true, false)
end -- // downpour.ioutils.file.log = {}

downpour.ioutils.file["set_temp_ext"] = function (fext)
    -- if fext then
        downpour.ioutils.file["__tmpname_ext__"] = tostring(fext)
    -- end
end -- // downpour.ioutils.file.log = {}

downpour.ioutils.file["get_ext"] = function (file)
    local str = nil

    if file and file ~= "" then
        local i, len = nil, -(#file) -- Invert the string length because we are going to search backwards.

        for i = -1, len, -1 do
            if file:byte(i) == 46 then -- 46 is the numeric ASCII code of the "." (.) (dot) character.
                str = file:sub(i + 1, -len)
                break
            end
        end
    end

    return str
end

downpour.ioutils.file["get_temp_ext"] = function ()
    return downpour.ioutils.file["__tmpname_ext__"]
end

downpour.ioutils.file["new_temp"] = function (path)
    local tmpname, name = os.tmpname(), ""
    local handle, handle_exception = nil, nil

    if tmpname then
        if path then
            local last_char = downpour.ioutils.stringutils.get_char(path, #path)
            if last_char ~= "/" and last_char ~= "\\" then
                path = tostring(path .. "/")
            end
        else
            path = "./"
        end

        name = path .. string.sub(tmpname, 2, #tmpname - 1)

        local name_ext = downpour.ioutils.file.get_temp_ext()
        if name_ext then
            name = name .. "." .. name_ext
        end

        handle, handle_exception = io.open(name, "w+") -- io.open() forbids me to declare "war".
        if handle_exception and downpour.ioutils.is_safe() then
            downpour.ioutils.file.log.write("[!] - downpour.ioutils.file.new_temp(): Cannot open %s for use as a temporary file. (reason: %s)", name, tostring(handle_exception))
        end
    end

    return handle
end

-- This function exists only for the sole purpose of maintaining consistency with its new_temp() counterpart.
-- You can just call io.close() on the returned handle instead.
downpour.ioutils.file["free_temp"] = function (handle)
    if handle ~= nil then
        handle:close()
    end
end

downpour.ioutils.file["exists"] = function (name)
    local r = io.open(name, "rb")

    if r ~= nil then
        r:close()
        return true
    end

    return false
end

downpour.ioutils.file["size"] = function (file, factor)
    local handle, handle_exception = io.open(file, "rb")

    if handle_exception and downpour.ioutils.is_safe() then
        downpour.ioutils.file.log.write("[!] - downpour.ioutils.file.size(): Cannot open %s to measure the size. (reason: %s)", file, tostring(handle_exception))
        return -1
    end

    factor = downpour.ioutils.ternary(factor ~= 0, factor, 1) or 1

    handle:seek("set", 0)
    local size = handle:seek("end")
    handle:close()

    return size / factor
end

downpour.ioutils.file["__copy_internal__"] = function (src, dest, fsrc, fdest, bsize)
    local src_handle, src_exception = io.open(src, fsrc)
    local dest_handle, dest_exception = io.open(dest, fdest)

    if src_exception ~= nil and downpour.ioutils.is_safe() then
        downpour.ioutils.file.log.write("[!] - downpour.ioutils.file.__copy_internal__(): Cannot open source file %s for copying data. (reason: %s)", src, tostring(src_exception))
        return false
    end

    if dest_exception ~= nil and downpour.ioutils.is_safe() then
        downpour.ioutils.file.log.write("[!] - downpour.ioutils.file.__copy_internal__(): Cannot create destination file %s for writing data. (reason: %s)", dest, tostring(dest_exception))
        return false
    end

    bsize = (bsize or 8) * 1024

    while true do
        local buffer_data = src_handle:read(bsize)
        if not buffer_data then break end

        dest_handle:write(buffer_data)
    end

    src_handle:close()
    dest_handle:close()

    return true
end

downpour.ioutils.file["copy"] = function (src, dest, bsize)
    return downpour.ioutils.file.__copy_internal__(src, dest, tostring('r'), tostring('w+'), bsize)
end

downpour.ioutils.file["copy_binary"] = function (src, dest, bsize)
    return downpour.ioutils.file.__copy_internal__(src, dest, tostring('rb'), tostring('w+b'), bsize)
end 

downpour.ioutils.file["get_list_unpack"] = function (path, ext, func) 
    -- Because we're going to check for logging eligibility (and possibility writing to the log file) a lot,
    -- let's make a few shortcuts to save time.
    local safe = downpour.ioutils.is_safe()
    local log  = downpour.ioutils.file.log.write

    if not path or path == "" then
        if safe then 
            log("[!] - downpour.ioutils.file.get_list_unpack(): Supplied path is either empty or nil. Using the current folder (.).")
        end

        path = "./"
    else
        local ascii = path:byte(#path)
        if ascii ~= 47 and ascii ~= 92 then -- "\\" and "/", respectively. Append the "/" character if the last character of path does not contain either "\\" or "/".
            path = path .. "/"
        end
    end

    ext = ext or '*'
    local ext_exists = ext ~= '*'
    local dir_stack, ext_stack = { path }, { }
    local ofile_stack, odir_stack = { }, { } 

    -- Parses out the extension list (separated with the semicolon ';' sign).
    -- This aids in parsing a directory while selecting files with multiple extensions.
    if ext_exists then
        ext_stack = downpour.ioutils.stringutils.parse(ext, ';')
    end

    -- Emulate the recursive feature that is normally found in functions like directory traversal,
    -- by creating a parameter stack and push parameters into it.
    -- Apparently because recursive benefits nothing but puts burden into the internal stack, and most implementations
    -- resort to the stack anyway.
    while #dir_stack > 0 do
        -- Pops the top frame of the stack, and pass it to current_dir.
        local current_dir = tostring(dir_stack[#dir_stack])
        dir_stack[#dir_stack] = nil

        if safe then
            log("[_] - downpour.ioutils.file.get_list_unpack(): Processing %s...", current_dir)
        end

        -- Processes the current stack frame.
        for file in lfs.dir(current_dir) do
            if file ~= "." and file ~= ".." then
                local file_name = current_dir .. file
                local file_mode = lfs.attributes(file_name, "mode")

                if safe then
                    log("[_] - downpour.ioutils.file.get_list_unpack(): Processing file %s (of type %s) in (sub) directory %s...", file_name, file_mode, current_dir)
                end 

                if file_mode == "file" then -- If the current "file" is an actual file, then push it to the file stack.
                    if ext_exists then
                        if downpour.ioutils.stringutils.match_table(downpour.ioutils.file.get_ext(file_name), ext_stack, #ext_stack) then
                            ofile_stack[#ofile_stack + 1] = tostring(file_name)

                            if func then
                                func(file_name)
                            end
                        end
                    else
                        ofile_stack[#ofile_stack + 1] = tostring(file_name)

                        if func then
                            func(file_name)
                        end
                    end
                elseif file_mode == "directory" then -- If it's a directory, then push the directory into the directory stack, so we can have another folder to process.
                    dir_stack[#dir_stack + 1] = file_name .. '/' 

                    -- Additionally, push the directory into our output directory stack.
                    odir_stack[#odir_stack + 1] = file_name .. '/' 
                end
            end
        end
    end

    return ofile_stack, odir_stack, #ofile_stack, #odir_stack 
end 

downpour.ioutils.file["get_list_pack"] = function (path, ext, func)
    local file_table, dir_table, _, _ = downpour.ioutils.file.get_list_unpack(path, ext, func)
    local table = { }

    for _, v in pairs(dir_table)  do table[#table + 1] = v end 
    for _, v in pairs(file_table) do table[#table + 1] = v end 

    return table 
end 

downpour.ioutils.file["get_list"] = function (path, ext, func) 
    return downpour.ioutils.file.get_list_pack(path, ext, func) 
end 

downpour.ioutils.file["clone"] = function (src, dest) 
    local fallout = src ~= nil and dest ~= nil and type(src) == "string" and type(dest) == "string" 

    if fallout ~= false then 
        local sb, db = src:byte(#src), dest:byte(#dest) 
        if sb ~= 92 and sb ~= 47 then src = src .. "/" end 
        if db ~= 92 and db ~= 47 then dest = dest .. "/" end 

        local file_table, dir_table, _, _ = downpour.ioutils.file.get_list_unpack(src) 

        for _, vdir in pairs(dir_table) do 
            local _, pos = downpour.ioutils.stringutils.match(vdir, src) 

            lfs.mkdir(dest .. vdir:sub(pos, #vdir - 1)) 
        end 

        for _, vfile in pairs(file_table) do 
            local _, pos =  downpour.ioutils.stringutils.match(vfile, src) 

            downpour.ioutils.file.copy_binary(vfile, dest .. vfile:sub(pos, #vfile)) 
        end 

        return true 
    else 
        if downpour.ioutils.is_safe() then 
            downpour.ioutils.file.log.write("%s", "[!] - downpour.ioutils.file.clone(): Bad argument passed.")
        end 
    end 

    return false 
end  

downpour.ioutils.file["mirror"] = function (src, dest) 
    return downpour.ioutils.file.clone(src, dest)
end 

return downpour.ioutils -- // downpour.ioutils = {}
