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

vars = {} 

local _API_version_major, _API_version_minor, _API_version_rev, _API_version_name = love.getVersion() 
local _version = "Alpha" 

vars = {
    version = _version, 

    window_width = 640, 
    window_height = 600,
    window_max_width = 0, 
    window_max_height = 0,
    window_title = string.format("Herbal %s (%s, Love2D %d.%d.%d %s)", vars.version, _VERSION, _API_version_major, _API_version_minor, _API_version_rev, _API_version_name), 

    built_target = "./", 
    built_target_length = 257,

    msys2_root = "./",
    msys2_root_length = 257, 

        msys2_mingw_make = "./", 
        msys2_mingw_make_length = 257, 

        msys2_mingw_strip = "./", 
        msys2_mingw_strip_length = 257, 

    mingw_root = "./", 
    mingw_root_length = 257, 

    cmake_root = "./", 
    cmake_root_length = 257, 

    is_build_64bit_target   = false, 
    is_build_angelscript    = false, 
    is_build_lua            = false, 
    is_build_luajit         = false, 
    is_build_luajit_alm     = false, 
    is_build_luajit_safe    = false, 
    is_build_lua_raw_loader = false, 
    is_build_network        = false, 
    is_build_physics        = false, 
    is_build_nav            = false, 
    is_build_mt             = false, 
    is_build_2d             = false, 
    is_build_devtools       = false, 
    is_build_extras         = false, 
    is_build_samples        = false, 
    is_build_packaging      = false, 
    is_build_profiling      = false, 
    is_build_static_vcpplib = false, 
    is_build_w32_konsole    = false, 
    is_build_docs           = false, 
    is_build_precompiled_h  = false, 
    is_build_gnu11          = false, 
    is_build_gl             = false, 
    is_build_d3d11          = false, 
    is_strip_bin            = false, 

    is_auto_detect_make     = false, 
    is_auto_detect_strip    = false, 

    database_type    = 1,
    target_type      = 1,
    lib_type         = 1,
    toolchain_type   = 1, 
    msys2_mingw_type = 1, 

    config_name = "herbal.conf", 
    script_name = "build",

    toolchain_type_enum = {
        "Linux (native)", 
        "Windows (with Microsoft Visual Studio)", 
        "Windows (with MinGW/MinGW-W64)", 
        "Windows (with MSYS2+MinGW/MinGW-W64)"
    }, 

    database_type_enum = {
        "OBDC (*)",
        "SQLite"
    }, 

    target_type_enum = {
        "Linux (I'll generate a bash (.sh) script)", 
        "Windows (I'll generate a batch (.bat) script)"
    }, 

    lib_type_enum = {
        "Static (.a on Linux, and .lib on Windows)", 
        "Dynamic (.so on Linux, and .dll on Windows)"
    }, 

    msys2_mingw_enum = {
        "32-bit MinGW (/mingw32)", 
        "64-bit MinGW (/mingw64)", 
    }, 

    gcc = {
        optimization_level_type = 1, 

        optimization_level_enum = { -- TODO: -Og, -Os, -O0 
            "A little (-O/-O1)", 
            "Slight (-O2)", 
            "Heavy (-O3)", 
            "Aggressive (-Ofast)"
        }, 

        is_warning = false, 
        is_warning_wall = false, 
        is_warning_wextra = false, 
        is_warning_wextra = false, 
        is_warning_inline = false 
    }
}

return vars 
