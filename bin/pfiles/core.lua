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
This module implements the core/kernel-level functionalities for preceding sub-core modules. It offers:
* Tight integration with inherited submodules: Because we are using LuaJIT, a tracing JIT compiler to parse Lua code,
++ the whole codebase is not fully compiled at once during execution. Instead, a program's portions (so-called chunks in Lua),
++ are parsed and interpreted when encountered. This way, we can include references to submodules even when they haven't been
++ compiled (yet), and call the method which contains the reference later when all references are resolved.

* Arguments parsing: We can pass custom flags to our game and then parse it to adjust the subsystem's behaviour accordingly,
++ like, unlocking development/mod tools or passing an additional configuration file to the game.

* Calls, initializes and destructs subsystems: You should never call subsystem's initialization/destruction methods directly,
++ instead you can just simply pass a flag defining which modules you want to load, and the core will take care of the nitty-gritty
++ details of loading that module. (SDL anyone?)
------------------------------------------
]]--

local debug = require("./development/core-debug")

core = {}

core = {
    ["__safe__"] = false
}

core["safe"] = function (switch)
    core["__safe__"] = switch
end

core["is_safe"] = function ()
    return core["__safe__"]
end -- // core = {}

core.args = {}

core.args["get"] = function ()
    return GetArguments()
end

core.args["exists"] = function (arg)
    if arg and arg ~= "" then
        local t = GetArguments()
        for _, v in ipairs(t) do
            if v == arg then
                return true
            end
        end
    end

    return false
end

core.args["get_param"] = function (arg_val)
    if arg_val and arg_val ~= "" then
        local t = GetArguments()
        for k, v in ipairs(t) do
            if v == arg_val and t[k + 1] ~= nil then
                return t[k + 1]
            end
        end
    end

    return nil
end -- // core.args = {}

scene_ = nil -- Scene
cameraNode = nil -- Camera scene node
yaw = 0 -- Camera yaw angle
pitch = 0 -- Camera pitch angle
useMouseMode_ = MM_ABSOLUTE

function SampleStart()
    -- Subscribe key down event
    SubscribeToEvent("KeyDown", "HandleKeyDown")

    -- Subscribe key up event
    SubscribeToEvent("KeyUp", "HandleKeyUp")

    -- Subscribe scene update event
    SubscribeToEvent("SceneUpdate", "HandleSceneUpdate")
end

function SampleInitMouseMode(mode)
    useMouseMode_ = mode

    input.mouseVisible = true
    SubscribeToEvent("MouseButtonDown", "HandleMouseModeRequest")
    SubscribeToEvent("MouseModeChanged", "HandleMouseModeChange")
end

function HandleKeyUp(evt, evd)
    local key = evd["Key"]:GetInt()
    -- Close console (if open) or exit when ESC is pressed
    if key == KEY_ESCAPE then
        if console:IsVisible() then
            console:SetVisible(false)
        else
            engine:Exit()
        end
    end
end

function HandleKeyDown(evt, evd)
    debug.register_keys(evt, evd)
end

function HandleSceneUpdate(evt, evd)

end

function HandleMouseModeRequest(evt, evd)
    if console ~= nil and console.visible then
        return
    end

    if input.mouseMode == MM_ABSOLUTE then
        input.mouseVisible = false
    elseif useMouseMode_ == MM_FREE then
        input.mouseVisible = true
    end

    input.mouseMode = useMouseMode_
end

function HandleMouseModeChange(evt, evd)
    mouseLocked = evd["MouseLocked"]:GetBool()
    input.mouseVisible = not mouseLocked;
end

return core
