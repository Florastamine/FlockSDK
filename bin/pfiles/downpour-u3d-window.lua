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
This module contains definitions for the Downpour's Urho3D Lua API wrapper, which
wraps around functionalities for customizing the rendering window (title bar, icon, and so on).
------------------------------------------
Dependencies:
[X] downpour
[X] downpour-u3d
]]--

local downpour_u3d = require("./downpour-u3d")

downpour_u3d.window = {}

downpour_u3d.window["get_cursor_x"] = function ()
    return IntVector2(input:GetMousePosition())["x"]
end

downpour_u3d.window["get_cursor_y"] = function ()
    return IntVector2(input:GetMousePosition())["y"]
end

downpour_u3d.window["get_title"] = function ()
    return graphics:GetWindowTitle()
end

downpour_u3d.window["set_title"] = function (title)
    if title ~= nil then
        graphics:SetWindowTitle(title)
    end
end

downpour_u3d.window["set_resolution"] = function (width, height)
    graphics:SetMode(math.abs(width) or 0, math.abs(height) or 0)
end

downpour_u3d.window["get_resolution"] = function ()
    return graphics:GetWidth(), graphics:GetHeight()
end

downpour_u3d.window["set_fullscreen"] = function (bool)
    if bool ~= graphics:GetFullscreen() then
        graphics:ToggleFullscreen()
    end
end

downpour_u3d.window["set_icon"] = function (file)
    if file ~= nil then
        local p = cache:GetResource("Image", file)
        if p ~= nil then
            graphics:SetWindowIcon(p)
            return p
        end
    end

    return nil
end

downpour_u3d.window["get_fullscreen"] = function ()
    return graphics:ToggleFullscreen()
end

return  downpour_u3d.window
