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

package.path = package.path .. ";./BuildTool"
package.cpath = package.cpath .. ";./" 

local imgui = require("imgui") 
local vars = require("vars") 
local utils = require("utils") 
local debug = require("debux") 

local debug_io_handle = nil 

function love.load(args) 
    debug.debug = true 

    debug.do_if(
        function () 
            debug_io_handle = io.open(utils.path.get_root() .. debug.name, "w+") 
            debug_io_handle:write("Logging module for Herbal opened. Will be streaming events into ", debug.name, " of ", utils.path.get_root(), "\n")  
        end 
    )

    love.window.setTitle(vars.window_title)
    love.window.setMode(0, 0, {})
    vars.window_max_width, vars.window_max_height = love.graphics.getDimensions()

    love.window.setMode(vars.window_width, vars.window_height, {
        fullscreen = false,
        vsync = false,
        msaa = 0,
        resizable = false,
        x = (vars.window_max_width - vars.window_width) / 2,
        y = (vars.window_max_height - vars.window_height) / 2
    }) 

    debug.do_if(
        function () 
            local _, _, t = love.window.getMode() 

            debug_io_handle:write(string.format(
                [[
window_title: %s  
window_width/window_max_width: %i/%i 
window_height/window_max_height: %i/%i 
x/y/fullscreen/vsync: %i/%i/%i/%i 
                ]], 
               vars.window_title, vars.window_width, vars.window_max_width, vars.window_height, vars.window_max_height, t.x, t.y, utils.boolean.tonumber(t.fullscreen), utils.boolean.tonumber(t.vsync) 
            )) 
        end 
    )
end 

local ParseToolchainType = function (function_table) 
    if function_table ~= nil and type(function_table) == "table" then 
        function_table[vars.toolchain_type]() 
    end 
end  

local ActionReadConfigurationFile = function (name) 
    if name and type(name) == "string" then 
        local f = io.open(utils.path.get_root() .. name, 'r') 

        debug.do_if(
            function () 
                debug_io_handle:write("Attempting to open ", name, " for reading configuration data.\n") 
            end 
        )

        if f ~= nil then 
            local rls = function () return (f:read("*l")):match("^%s*(.-)%s*$") end 
            local rln = function () return tonumber(rls()) end 
            local rlb = function () if rls() == tostring("true") then return true else return false end end 

            vars.built_target = rls()
            vars.cmake_root = rls()

            vars.toolchain_type = rln() 
            vars.target_type = rln()  
            vars.lib_type = rln() 
            vars.database_type = rln()

            vars.is_build_64bit = rlb() 
            vars.is_build_network = rlb() 
            vars.is_build_physics = rlb() 
            vars.is_build_nav = rlb() 
            vars.is_build_2d = rlb() 
            vars.is_build_mt = rlb() 
            vars.is_build_profiling = rlb() 
            vars.is_build_precompiled_h = rlb() 
            vars.is_build_gnu11 = rlb() 

            vars.is_build_angelscript = rlb() 

            vars.is_build_lua = rlb() 
            vars.is_build_luajit = rlb() 
            vars.is_build_luajit_alm = rlb() 
            vars.is_build_luajit_safe = rlb() 
            vars.is_build_lua_raw_loader = rlb() 

            vars.is_build_gl = rlb() 
            vars.is_build_d3d11 = rlb() 

            vars.is_build_static_vcpplib = rlb() 
            vars.is_build_w32_konsole = rlb()

            vars.is_build_samples = rlb() 
            vars.is_build_devtools = rlb() 
            vars.is_build_extras = rlb() 
            vars.is_build_packaging = rlb() 
            vars.is_build_docs = rlb() 

            vars.is_strip_bin = rlb()  

            f:close() 
        end 
    end 
end 

local ActionSaveConfigurationFile = function (name) 
    if name and type(name) == "string" then 
        -- 
    end 
end  

local ActionConfigureAndGenerate = function () 
end 

local ActionReparseConfiguration = function () 
    ActionReadConfigurationFile(vars.config_name)
end 

local ActionSaveConfiguration = function () 
end 

function love.update(dt) 
    if vars.begin_configuring_pressed == true then 
        do 
            ActionConfigureAndGenerate() 
            vars.begin_configuring_pressed = false 
        end  
    elseif vars.begin_reparse_configuration == true then 
        do 
            ActionReparseConfiguration() 
            vars.begin_reparse_configuration = false 
        end  
    elseif vars.begin_save_configuration == true then 
        do 
            ActionSaveConfiguration() 
            vars.begin_save_configuration = false 
        end 
    else 
    end 
end 

function love.textinput(t)
    imgui.TextInput(t)
end

function love.keypressed(key)
    imgui.KeyPressed(key)
end

function love.keyreleased(key)
    imgui.KeyReleased(key)
end

function love.mousemoved(x, y)
    imgui.MouseMoved(x, y)
end

function love.mousepressed(x, y, button)
    imgui.MousePressed(button)
end

function love.mousereleased(x, y, button)
    imgui.MouseReleased(button)
end

function love.wheelmoved(x, y)
    imgui.WheelMoved(y)
end 

local DrawToolchainUI = function () 
    ParseToolchainType({
        function () 
            --  
        end, 

        function () 
            --  
        end, 

        function () 
            -- 
        end, 

        function () 
            _, vars.msys2_root = imgui.InputText("MSYS2 root", vars.msys2_root, vars.msys2_root_length)

            imgui.Text("MinGW target to use: ") 
            _, vars.msys2_mingw_type = imgui.Combo("5", vars.msys2_mingw_type, vars.msys2_mingw_enum, #vars.msys2_mingw_enum) 

            _, vars.is_auto_detect_make = imgui.Checkbox("Auto-detect make/mingw32-make", vars.is_auto_detect_make) 
            if vars.is_auto_detect_make ~= true then  
                _, vars.msys2_mingw_make = imgui.InputText("Manual specification of make/mingw32-make", vars.msys2_mingw_make, vars.msys2_mingw_make_length) 
            end 

            _, vars.is_auto_detect_strip = imgui.Checkbox("Auto-detect strip", vars.is_auto_detect_strip) 
            if vars.is_auto_detect_strip ~= true then 
                _, vars.msys2_mingw_strip = imgui.InputText("Manual specification of strip", vars.msys2_mingw_strip, vars.msys2_mingw_strip_length) 
            end 
        end  
    }) 
end  

function love.draw()
    imgui.NewFrame() 

    imgui.SetNextWindowSize(vars.window_width, vars.window_height)
    imgui.SetNextWindowPos(0, 0)

    imgui.Begin(vars.window_title)
        if imgui.CollapsingHeader("General configuration") then 
            _, vars.built_target = imgui.InputText("Built target folder", vars.built_target, vars.built_target_length)  
            _, vars.cmake_root = imgui.InputText("CMake location/path", vars.cmake_root, vars.cmake_root_length)
        end 

        if imgui.CollapsingHeader("Toolchain") then 
            imgui.Text("Toolchain type:")
            _, vars.toolchain_type = imgui.Combo("1", vars.toolchain_type, vars.toolchain_type_enum, #vars.toolchain_type_enum)

            DrawToolchainUI()
        end 

        if imgui.CollapsingHeader("Target configuration") then 
            _, vars.target_type = imgui.Combo("2", vars.target_type, vars.target_type_enum, #vars.target_type_enum)
        end 

        if imgui.CollapsingHeader("Build options") then 
            _, vars.is_build_64bit = imgui.Checkbox("Build 64-bit targets (URHO3D_64BIT)", vars.is_build_64bit) 

            _, vars.is_build_network = imgui.Checkbox("Networking support (URHO3D_NETWORK)", vars.is_build_network) 

            _, vars.is_build_physics = imgui.Checkbox("Physics(*) support (URHO3D_PHYSICS)", vars.is_build_physics)
            imgui.Text("(*) Physics support through Bullet.")

            _, vars.is_build_nav = imgui.Checkbox("Navigation support (URHO3D_NAVIGATION)", vars.is_build_nav)

            _, vars.is_build_2d = imgui.Checkbox("2D(*) support (URHO3D_URHO2D)", vars.is_build_2d) 
            imgui.Text("(*) 2D support includes both rendering and physics.")

            _, vars.is_build_mt = imgui.Checkbox("MT support (URHO3D_THREADING)", vars.is_build_mt) 

            _, vars.is_build_profiling = imgui.Checkbox("Profiling support (URHO3D_PROFILING)", vars.is_build_profiling) 

            _, vars.is_build_precompiled_h = imgui.Checkbox("Enable PCH(*) support (URHO3D_PCH)", vars.is_build_precompiled_h)
            imgui.Text("(*) PCH: Precompiled header.")

            _, vars.is_build_gnu11 = imgui.Checkbox("Enable C++11(*) support (URHO3D_C++11)", vars.is_build_gnu11) 
            imgui.Text("C++11 (URHO3D_C++11) would also be enabled if one of the following configurations were selected: ")
            imgui.Text("- URHO3D_ANGELSCRIPT on web and Android/ARM platforms with aarch64, or")
            imgui.Text("- URHO3D_DATABASE_ODBC on all platforms.") 

            imgui.Text("Library type")
            _, vars.lib_type = imgui.Combo("3", vars.lib_type, vars.lib_type_enum, #vars.lib_type_enum)

            if imgui.CollapsingHeader("AngelScript") then  
                _, vars.is_build_angelscript = imgui.Checkbox("AngelScript(*) support (URHO3D_ANGELSCRIPT), ", vars.is_build_angelscript)
                imgui.Text("(*) AngelScript is required by the built-in scene editor.")
            end 

            if imgui.CollapsingHeader("Lua/LuaJIT") then 
                _, vars.is_build_lua = imgui.Checkbox("Lua support (URHO3D_LUA)", vars.is_build_lua)
                _, vars.is_build_luajit = imgui.Checkbox("LuaJIT support (URHO3D_LUAJIT)", vars.is_build_luajit)
                _, vars.is_build_luajit_alm = imgui.Checkbox("LuaJIT amalgamated build(*) (URHO3D_LUAJIT_AMALG)", vars.is_build_luajit_alm)
                imgui.Text("(*) Quoting from http://luajit.org/install.html:") 
                imgui.Text([[
The build system has a special target for an amalgamated build, i.e. make amalg. 
This compiles the LuaJIT core as one huge C file and allows GCC to generate faster 
and shorter code. Alas, this requires lots of memory during the build. This may 
be a problem for some users, that's why it's not enabled by default. But it shouldn't 
be a problem for most build farms. 
It's recommended that binary distributions use this target for their LuaJIT builds.
                ]])

                _, vars.is_build_luajit_safe = imgui.Checkbox("C++ wrapper safety checks(*) for Lua/LuaJIT (URHO3D_SAFE_LUA)", vars.is_build_luajit_safe) 
                imgui.Text("(*) This can be slow.")   

                _, vars.is_build_lua_raw_loader = imgui.Checkbox("Prefer loading raw script files over the ones in Urho3D's cache. (URHO3D_LUA_RAW_SCRIPT_LOADER)", vars.is_build_lua_raw_loader) 
                imgui.Text("(*) This can be useful for debugging purposes, but has less performance than just loading from the resource cache.")
            end 

            if imgui.CollapsingHeader("Rendering") then 
                _, vars.is_build_gl = imgui.Checkbox("OpenGL(*) support (URHO3D_OPENGL)", vars.is_build_gl) 
                _, vars.is_build_d3d11 = imgui.Checkbox("Direct3D11(**) support (URHO3D_D3D11)", vars.is_build_d3d11)
                imgui.Text("(*) Selecting OpenGL on any platform will override Direct3D, and vice versa.")  
                imgui.Text("(**) Building on Windows with both OpenGL and Direct3D11 unselected will default to Direct3D9.")
                imgui.Text("     Selecting both OpenGL and Direct3D11 will default to OpenGL on non-Windows platforms, and ")
                imgui.Text("     Direct3D11/Direct3D9 on Windows platform.")
            end 

            if imgui.CollapsingHeader("Database") then 
                _, vars.database_type = imgui.Combo("4", vars.database_type, vars.database_type_enum, #vars.database_type_enum)

                if vars.database_type == 1 then 
                    imgui.Text("OBDC requires vendor-specific ODBC driver.") 
                end 
            end 

            if imgui.CollapsingHeader("Windows-specific settings") then 
                _, vars.is_build_static_vcpplib = imgui.Checkbox("Prefer static C/C++ runtime libraries(*) (URHO3D_STATIC_RUNTIME)", vars.is_build_static_vcpplib)
                imgui.Text("(*) Use static C/C++ runtime libraries and eliminate the need for runtime DLLs installation. (VS only)")

                _, vars.is_build_w32_konsole = imgui.Checkbox("Prefer main() over WinMain() to set up a console for logging purposes. (URHO3D_WIN32_CONSOLE)", vars.is_build_w32_konsole) 
            end 

            if imgui.CollapsingHeader("Extras") then 
                _, vars.is_build_samples = imgui.Checkbox("Build samples (URHO3D_SAMPLES)", vars.is_build_samples)

                _, vars.is_build_devtools = imgui.Checkbox("Build development tools(*) (URHO3D_TOOLS)", vars.is_build_devtools)
                imgui.Text("(*) Include everything in Source/Tools/.")

                _, vars.is_build_extras = imgui.Checkbox("Build extra tools(*) (URHO3D_EXTRAS)", vars.is_build_extras)
                imgui.Text("(*) OgreBatchConverter")

                _, vars.is_build_packaging = imgui.Checkbox("Enable packaging of resources (URHO3D_PACKAGING)", vars.is_build_packaging)

                _, vars.is_build_docs = imgui.Checkbox("Build documentation (URHO3D_DOCS)", vars.is_build_docs)
            end  
        end 

        if imgui.CollapsingHeader("Compiler configuration") then 
            if vars.toolchain_type ~= 2 then -- Which means we won't be compiling with the Microsoft toolchain. 
                imgui.Text("gcc/make toolchain selected.") 

                if imgui.CollapsingHeader("Warnings") then 
                    _, vars.gcc.is_warning = imgui.Checkbox("Disable all warnings. (-w)", vars.gcc.is_warning)

                    if vars.gcc.is_warning ~= true then 
                        _, vars.gcc.is_warning_wall = imgui.Checkbox("Enable (almost) all warnings (-Wall)", vars.gcc.is_warning_wall) 
                        _, vars.gcc.is_warning_wextra = imgui.Checkbox("Enable extra warnings (-Wextra)", vars.gcc.is_warning_wextra) 
                        _, vars.gcc.is_warning_stricter = imgui.Checkbox("Stricter conformation to ISO C/ISO C++ (-Wpedantic)", vars.gcc.is_warning_wextra)
                        _, vars.gcc.is_warning_inline = imgui.Checkbox("Warning if the specified function marked as \"inline\" cannot be inlined. (-Winline)", vars.gcc.is_warning_inline) 
                    end 
                end 

                if imgui.CollapsingHeader("Optimizations") then 
                    _, vars.gcc.optimization_level_type = imgui.Combo("6", vars.gcc.optimization_level_type, vars.gcc.optimization_level_enum, #vars.gcc.optimization_level_enum) 
                end 
            else 
                imgui.Text("msvc/nmake toolchain selected.") 
            end 
        end 

        if imgui.CollapsingHeader("Post-build") then 
            _, vars.is_strip_bin = imgui.Checkbox("Strip symbols from binaries(*) (gcc only)", vars.is_strip_bin)
            imgui.Text("(*) Only binaries produced by GCC (gcc/g++/...) can have their symbols stripped. On ")
            imgui.Text("Windows platforms and binaries are built using MSVC, symbols are stored in (a) separate")
            imgui.Text(".pdb (files).") 
        end 

        vars.begin_reparse_configuration = imgui.Button("Reparse configuration file (" .. vars.config_name .. ")") 
        vars.begin_save_configuration = imgui.Button("Save current configuration to " .. vars.config_name)
        vars.begin_configuring_pressed = imgui.Button("Begin configuring and generating " .. vars.script_name)
    imgui.End()

    imgui.Render()
end 
