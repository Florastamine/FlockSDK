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
]]-- 

utils = {} 

utils.boolean = {} 

utils.boolean.tostring = function (boolean) 
    if boolean ~= nil and type(boolean) == "boolean" then 
        if boolean == true then return "true" 
        else return "false" end 
    end 

    return nil 
end 

utils.boolean.tonumber = function (boolean) 
    local r = utils.boolean.tostring(boolean) 

    if r == tostring("true") then r = 1 
    elseif r == tostring("false") then r = 0
    else end 

    return r 
end -- // utils.boolean = {} 

utils.path = {} 

utils.path.make_path = function (path)
    local p = tostring(path)  

    if p ~= nil and type(p) == "string" then 
        local b = p:byte(#p) 

        if b ~= 47 and b ~= 92 then 
            p = p .. '/' 
        end 
    end 

    return p 
end 

utils.path.make = utils.path.make_path 

utils.path.get_root = function () 
    return love.filesystem.getSourceBaseDirectory() .. '/' 
end -- // utils.path = {} 

return utils 
