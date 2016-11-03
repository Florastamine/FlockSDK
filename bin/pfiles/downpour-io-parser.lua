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
This module contains utilities and tools for parsing and executing command sequences.
------------------------------------------
Dependencies:
[X] downpour
[X] downpour-io
------------------------------------------
+ safe(val)
++ Enables explicit logging for this module. If any of the calls in the (sub)module in which safe() was enabled behaves
++ unexpectedly, the error message will be logged into a file on the physical disk in order to be viewed later.
------------------------------------------
+ is_safe()
++ Returns true if the current module does have explicitly logging enabled, false otherwise.
------------------------------------------
+ stosp(string)
++ Converts a given string into its pattern representation. When doing the parsing, parse() (see below) doesn't
++ use the string-based command directly, but instead processing its pattern instead. This function is not intended
++ for using outside parse(), but feel free to do so.
------------------------------------------
+ parse(command, table, table_name)
++ Performs parsing the specified command, using the passed action table on where to look the action up. The third parameter, however,
++ you must pass the name of the action table. For example, if your table's name is "atable", you must pass "atable" to the third parameter.
++ (with the quotes). A more concise example:
++ local mytable = {}
++ <...>
++ local r = downpour.ioutils.parser.parse("my command", mytable, "mytable")
++ There wasn't a solid reason behind this, but this requirement comes from the fact that there isn't an obvious way to convert
++ an identifier to its string representation in Lua 5.1.x (you can do this very easily in C, though).
++
++ TODO: Write an instruction on how to construct a proper action table in order to pass to parse().
]]--

local downpour_io = require("./downpour-io")

downpour_io.parser = {}

downpour_io.parser = {
    ["__safe__"] = false
}

downpour_io.parser.__rule_table = {
    ["ALPHABET_VALUE"] = "a",
    ["NUMERICAL_VALUE"] = "0",
}

-- Lua 5.1.x
downpour_io.parser.__illegal_token_table = {
    "and", "break", "do", "else", "elseif",
    "end", "false", "for", "function", "if",
    "in", "local", "nil", "not", "or",
    "repeat", "return", "then", "true", "until", "while"
}

downpour_io.parser["safe"] = function (switch)
    downpour_io.parser["__safe__"] = switch
end

downpour_io.parser["is_safe"] = function ()
    return downpour_io.parser["__safe__"]
end

downpour_io.parser["__get_safe_token"] = function (token)
    if token ~= nil and type(token) == "string" then
        for _, v in pairs(downpour_io.parser.__illegal_token_table) do
            if token == v then
                return "_" .. token
            end
        end
    else
        if downpour_io.parser.is_safe() then
            downpour_io.file.log.write("[!] - downpour.ioutils.parser.__get_safe_token(): Cannot identify \"%s\" as a valid token.", tostring(token))
        end
    end

    return token
end

downpour_io.parser["stosp"] = function (command)
    local safe = downpour_io.parser.is_safe
    local log  = downpour_io.file.log.write

    if (not command) or (type(command) ~= "string") then
        if safe() then
            log("%s", "[!] - downpour.ioutils.parser.stosp(): Cannot convert the command into pattern, it's either nil, empty or not a string.")
        end

        return nil
    else
        local ret, i, cchar = "", 1, 0
        local token_table = downpour_io.stringutils.split(command, " ")
        local rtable = downpour_io.parser.__rule_table

        for _, v in pairs(token_table) do
            cchar = downpour_io.stringutils.get_char(v, 1)
            local boolean = downpour_io.numericutils.is_digit(cchar)

            if not boolean then -- It's a string character.
                ret = ret .. rtable["ALPHABET_VALUE"]
            elseif boolean then -- It's a number character
                ret = ret .. rtable["NUMERICAL_VALUE"]
            else end
        end

        if safe() then
            log("[!] - downpour.ioutils.parser.stosp(): Parsed pattern: %s", ret)
        end

        return ret
    end
end

downpour_io.parser["parse"] = function (command, table, table_name)
    local safe = downpour_io.parser.is_safe
    local log  = downpour_io.file.log.write

    local fallout = (not command or not table or not table_name) or (type(command) ~= "string" or type(table) ~= "table" or type(table_name) ~= "string")

    if fallout then
        if safe() then log("%s", "[!] - downpour.ioutils.parser.parse(): Bad command, table, or table name.") end

        return false
    end

    local function_stack, param_stack = {}, {} -- Function stack, parameter stack along with their iterators.
    local fstack_iter, fparam_iter = 1, 1

    local pattern = downpour_io.parser.stosp(command)
    local plen, pit = #pattern, 1 -- Get the length of the pattern and set the iterator to the initial position.

    local ctable = downpour_io.stringutils.split(command, " ") -- Parse the command into a new table for easier lookup.
    if #ctable < 1 then -- This should not happen as if the table is empty, then mostly the string is also empty.
        if safe() then
            log("%s", "[!] - downpour.ioutils.parser.parse(): ctable (parsed command table) is empty.")
        end

        return false
    end

    if safe() then log("[_] - downpour.ioutils.parser.parse(): ctable length = %i, plen = %i", #ctable, plen) end

    -- Scans through the pattern string and put commands/parameters into their equivalent stacks.
    for pit = 1, plen, 1 do
        local ch = downpour_io.stringutils.get_char(pattern, pit)
        if safe() then log("[_] - downpour.ioutils.parser.parse(): Retrieved \"%s\" from the pattern string.", ch) end

        if ch == downpour_io.parser.__rule_table["ALPHABET_VALUE"] then
            if safe() then log("[_] - downpour.ioutils.parser.parse(): \"%s\" is a ALPHABET_VALUE value.", ch) end

            function_stack[fstack_iter] = downpour_io.parser.__get_safe_token(ctable[pit])
            fstack_iter = fstack_iter + 1

            if safe() then log("[_] - downpour.ioutils.parser.parse(): Pushed a ALPHABET_VALUE value into the stack at position = %i and value = %s", fstack_iter - 1, function_stack[fstack_iter - 1]) end
        elseif ch == downpour_io.parser.__rule_table["NUMERICAL_VALUE"] then
            if safe() then log("[_] - downpour.ioutils.parser.parse(): \"%s\" is a NUMERICAL_VALUE value.", ch) end

            param_stack[fparam_iter] = tonumber(ctable[pit])
            fparam_iter = fparam_iter + 1

            if safe() then log("[_] - downpour.ioutils.parser.parse(): Pushed a NUMERICAL_VALUE value into the stack at position = %i and value = %i", fparam_iter - 1, param_stack[fparam_iter - 1]) end
        else
            if safe() then
                log("%s", "[!] - downpour.ioutils.parser.parse(): Unrecognized token found in the pattern string. The conversion will still be continued, but the result call will probably not behaving as expected.")
                log("[!] - downpour.ioutils.parser.parse(): Dumping current content from the last get_char() call: pattern string = (%s), parsed token = (%s), parsed token in ASCII = (%i), i = (%i)", pattern, ch, string.byte(ch), pit)
            end
        end
    end

    -- After that, the stacks should be filled with function names and parameters, so let's organize and call them.
    if #function_stack < 1 then -- We can't call a parameter alone. We need a function to call it from.
        if safe() then
            log("%s", "[!] - downpour.ioutils.parser.parse(): For some reason, the function stack is empty. Abort.")
        end

        return false
    end

    -- Check if the very last property has a .play() function implemented.
    -- Begin constructing a chain of properties that were collected before that leads to the play() method,
    -- and test if that method is available to call.
    local t_call, t_iter = "", 1

    if table_name ~= nil then
        t_call = t_call .. table_name
        if "." ~= downpour_io.stringutils.get_char(table_name, #table_name) then
            t_call = t_call .. "."
        end
    else
        if safe() then
            log("%s", "[!] - downpour.ioutils.parser.parse(): Cannot construct the function call (missing table name). Please pass one to parse().")
        end

        return false
    end

    for _, v in pairs(function_stack) do
        t_call = t_call .. v .. "."
    end

    t_call = t_call .. "play"
    local _t_func = "return " .. t_call .. "(...)"
    local func = loadstring(_t_func)

    if safe() then
        log("[!] - downpour.ioutils.parser.parser(): Preparing to call (%s) with the following command: (%s)", _t_func, command)
    end

    if func ~= nil then
        if safe() then
            log("[!] - downpour.ioutils.parser.parse(): %s is callable. Building parameters...", _t_func)
        end

        -- If it's callable, then build a parameter stack out of the current param_stack [] and hand it to the function (if possible).
        downpour_io.parser.pstack = {}
        if fparam_iter > 1 then
            local i = 1
            for i = 1, #param_stack, 1 do
                downpour_io.parser.pstack[i] = tonumber(param_stack[i])
            end
        end

        func(downpour_io.parser.pstack)
        return true
    else
        if safe() then
            log("[!] - downpour.ioutils.parser.parse(): %s is NOT callable. Make sure the table is correctly implemented, and the command you are trying to invoke has an executable .play() method ready.", _t_func)
        end

        return false
    end
end

return downpour_io.parser -- // downpour_io.parser = {}
