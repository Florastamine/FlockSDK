# Release 1 -- 01.06.2017 ([engine binaries + tool binaries](https://drive.google.com/drive/folders/0Bx0q4f1kFRaiRmcxNTNnVi1QZXc?usp=sharing))
Initial release. 

# Release 2 -- 01.27.2017 ([engine binaries + tool binaries](https://drive.google.com/drive/folders/0Bx0q4f1kFRaidm5UdHh3Rm1tMjQ?usp=sharing)) 
These builds incorporated a few bug-fixes introduced in the main `Urho3D` repository as well as new features and upgrades of existing 3rd party libraries. Most notably: 
* Upgraded [AngelScript](http://www.angelcode.com/angelscript/) from `2.30.2` to `2.31.2`. 
* Completely removed support for touch controls and touch input. (Because `Windows` and `Linux` are the only two target platforms that are officially supported, and touch controls are more popular on mobile devices. Also the code base would be simplified a lot.) 
* Gotten rid of legacy and/or unpopular file formats to reduce the size of the AssetImporter tool. (around 5 MiB was cut due to the dropping of a lot of file formats) 
* New editor features: Show object's origin and paint selection. 

**`Release 2` is the last release which still fully supports `Direct3D11`/`Direct3D9` on `Windows` platform.** 

# Release 3 -- 02.11.2017 ([engine binaries + tool binaries](https://drive.google.com/open?id=0Bx0q4f1kFRaiOWFaUnAtcTZLYms))
As usual, this release brought in various fixes, tweaks, upgrades, modifications as well as removals. 
* General bug-fixes and tweaks. 
* A few critical components have been made built-in and thus can't be disintegrated from the build through switches from now on. 
* New editor features: Compact viewport. 
* Upgraded [FreeType](https://www.freetype.org/) from `2.5.0` to `2.7.1`. 
* Upgraded [SQLite](https://www.sqlite.org) from `3.13.0` to `3.16.2`. 
* Removed most of platform-specific code for all available platforms except `Windows` and `Linux`. 
* Removed [Blender](https://www.blender.org), [Ogre3D](http://www.ogre3d.org)'s `.xml` and [Terragen](http://planetside.co.uk) importer from `AssetImporter`. 
* Removed [Microsoft Visual Studio](https://www.visualstudio.com) minidump support. 
* Removed `nanodbc`/`ODBC` support in favor of `SQLite`. 
* Completely removed `Direct3D11`/`Direct3D9` support, leaving only the `OpenGL` renderer across all platforms (`Windows` and `Linux`), which means that from now on, there would be only four prebuilt binaries for both `32-` and `64-`bit targets for `Windows` and `Linux`, running on OpenGL and not `Direct3D11`/`Direct3D9`, which also means that **([release 2](https://drive.google.com/drive/folders/0Bx0q4f1kFRaidm5UdHh3Rm1tMjQ?usp=sharing)) is the last release to support native `Direct3D` on `Windows`)**. 

**`Release 3` is the last release which still compiles on pre-`C++11` environments, due to the upgrade of [Open Asset Import Library](http://www.assimp.org) which was introduced in `Release 4` broke `C++98` compatibility.**  

# Release 4 -- 02.21.2017 ([engine binaries + tool binaries](https://drive.google.com/drive/folders/0Bx0q4f1kFRaicmFUaldhNnEtczA?usp=sharing)) 
This release does not only moves further into the goal of driving away from the original codebase, but also contains various bugfixes and improvements, outlined below. 
* General bug-fixes and tweaks. 
* New editor feature: Basic terrain editing within the editor, which includes terrain manipulation/smoothing, different brush size/types, undo/redo support, and heightmap saving/loading.
* Dropped the switch to enable C++11 (`-DFLOCKSDK_C++11`). This is an old switch that was left from the `Flock` codebase and is used to enable C++11 support, but it was dropped and now C++11 is mandatorily required. 
* The 2D component is now an integral part of the SDK, which adds an additional ~2.0 `MiB` overhead to the built executable. 
* Upgraded [Open Asset Import Library](http://www.assimp.org) from `3.1.1` to `3.3.1`, **thus effectively rendering pre-C++11 compilers to not be able to compile the SDK**. **(*)** 
* Upgraded [pugixml](http://pugixml.org) from `1.7` to `1.8`. 
* Upgraded [libcpuid](https://github.com/anrieff/libcpuid) from `0.2.2` to `0.4.0`. 
* Upgraded [Box2D](https://github.com/erincatto/Box2D) to commit [`71a6079b5fa717b8b56aad6a1130b74232989183`](https://github.com/erincatto/Box2D/commit/71a6079b5fa717b8b56aad6a1130b74232989183). 
* Dropped networking from the codebase (including [Civetweb](https://github.com/civetweb/civetweb) and [kNet](https://github.com/juj/kNet)). 
* `AssetImporter`, `RampGenerator`, `ScriptCompiler` and `SpritePacker` were taken out of the codebase and thus won't be included during the compilation process. **(..)**

**(.)** The upgrade of [Open Asset Import Library](http://www.assimp.org) from `3.1.1` to `3.3.1` brought to the table the ability to import from [3MF](http://www.3mf.io/what-is-3mf/), [SIB](https://en.wikipedia.org/wiki/Silo_(software)), and [glTF](https://en.wikipedia.org/wiki/GlTF). However, in this release, I've also dropped support for [SMD](https://developer.valvesoftware.com/wiki/Studio_Model_Data), [AC](https://en.wikipedia.org/wiki/AC3D), and Unreal's [.3D](http://paulbourke.net/dataformats/unreal/). 

**(..)** The rationale behind this is, for the parts are not part of the codebase (helper tools, for example) that rarely change, there's no point in keep re-compiling the code that will not (or rarely) change again and again when building the SDK, which would waste a lot of time, and thus these tools were removed from the codebase and are instead provided under the form of 32-bit pre-built binaries. Now the tools will only be provided under binary format, and their separate repository can be found [here](https://github.com/Florastamine/IWBHT_SDK_Tools). 

**`Release 4` is the last major release which still provides partial support for compiling the SDK under MSVC compilers. Starting from the fifth release, users are recommended to switch to [Cygwin](https://www.cygwin.com)/[MinGW-W64](http://mingw-w64.org/doku.php)/[MSYS2](http://www.msys2.org) instead.** 

# Release 5 -- 03.31.2017 ([engine binaries + tool binaries](https://drive.google.com/drive/folders/0Bx0q4f1kFRaieTBhZHJJYzRNRHc?usp=sharing)) 
This release saw a major delay compared to past releases, which usually takes around half a month for each of them, because I've been very busy with the game, and for that development effort was largely shifted to focusing on the game instead. Nevertheless, this is a fairly large release, packed with many brand new features as well as modifications and removals coming from both the Urho3D developers and myself. Enjoy!
* General bug-fixes and tweaks. 
* Physics support is now enforced. 
* Profiling support is now included by default in the build scripts, which can be left out easily. 
* Now the editor configuration file is located at the same folder as the editor executable and is named `SDKConfig.xml`. 
* GCC now uses `-O2` instead of `-Ofast` when building the SDK. 
* The SDK-specific LuaJIT patch is now optional and can be left out to enforce normal LuaJIT behaviour by commenting out `DOWNPOUR_PATCH` inside [`Source/ThirdParty/LuaJIT/src/luaconf.h`](https://github.com/Florastamine/FlockSDK/blob/master/Source/ThirdParty/LuaJIT/src/luaconf.h). 
* The SDK now prefers loading pre-compiled Lua scripts instead of raw scripts. 

* Removed testing support. 
* Removed support for Tundra's `.txml` format. (https://github.com/realXtend/tundra-urho3d)
* Removed the ability to export the scene to Wavefront **.obj**. 
* Removed support for `MSVC` compilers. 
* Removed lots of mobile-specific and `OpenGL ES`-related code and artifacts. 
* Disabled exception handling and threading support on [AngelScript](http://www.angelcode.com/angelscript/). 

* Integration of various post-processing `GLSL`-based shaders: Negative, chromatic aberration, pixelate, infrared, emboss, scanline, sharpen, crosshatch, crossstitch, TV/noise, posterization, edge detection/sobel, oil paint, SSAO. 
* Integrated a new water shader, replacing the default one. 
* Complete [ProcSky](https://github.com/carnalis/ProcSky/) integration into the engine, now with fixes, changes and Lua bindings. 
* Inverse kinematics integration. 
* Added LuaJIT's `string.ends_with()`.
* Added `IntVector2` type. 
* `64`-bit integer variant type support. 
* Multi-monitor and refresh rate options for full-screen/borderless. 

# Release 6 -- ??.??.2017 ([engine binaries + tool binaries](https://www.google.co.uk/?gws_rd=ssl)) 
## Changes/bugfixes
* The SDK's official name was changed to "Flock SDK", and the new GitHub URL pointing to the repository is now https://github.com/Florastamine/FlockSDK.
* Pre-built engine binaries are now Pentium 4 onwards compatible. These binaries were built using an old Pentium 4 machine, which runs Vista SP2 and GCC 7.1. Thus, Vista SP2 or higher is recommended, but it should runs just fine on XP SP3.
* Various bug-fixes and tweaks. 
* The pre-built [AngelScript](http://www.angelcode.com/angelscript/)-based scene editor binary now runs on 32-bit `Windows` and `Linux`. Previous releases were shipped with pre-built 32-bit executable for the engine runtime, and 64-bit executable for the scene editor. This was a long-standing issue as anyone who wants to use the scene editor were forced to switch to 64-bit OSes. This bug is now fixed, which also lowered the minimum requirements for running the SDK to 32-bit platforms.
* Starting from this release, the [AngelScript](http://www.angelcode.com/angelscript/)-based editor will no longer be included by default (it's there though, and can still be built with the `-DFLOCK_SCENE_EDITOR=1` option enabled), and thus has been marked as deprecated.
* Starting from this release, the engine (& scene editor) will be built with `-DCMAKE_BUILD_TYPE=RelWithDebInfo` instead of `-DCMAKE_BUILD_TYPE=Release`. **This is needed for working around a critical bug related to the legacy scene editor, see below.**
* `Vector2` can now initializes from `IntVector2`: `Vector2(const IntVector2 &vector);`.
* Improved build files generation time by removing a few unnecessary SDL2 function checks.
* Shorten main script file name to just `main.lua` instead of `core-main.lua`.
* Improved SSAO's overall quality, and also added a new parameter, `SSAOQuality`, which controls the "spreadness" of the effect.
* `stb_rect_pack.h` was moved to the [tools repository](https://github.com/Florastamine/IWBHT_SDK_Tools).
* Upgraded `stb_image` (`v2.12` -> `v2.15`) and `stb_image_write` (`v1.02` -> `v1.05`). **Note: Importing 16-bit PNGs should be possible now as `stb_image`, as of `v2.15`, provides suppport for 16-bit PNGs.**
* Upgraded [GLEW](http://glew.sourceforge.net/) (`v1.13.0` -> `v2.0.0`).
* Upgraded [Detour/DetourCrowd/DetourTileCache/Recast](https://github.com/recastnavigation/recastnavigation) (`v1.4` -> `v1.5.1`).
* Upgraded [Box2D](https://github.com/erincatto/Box2D) to latest `HEAD`.
* Upgraded [LZ4](https://github.com/lz4/lz4) (`1.7.1` -> `1.7.5`).
* Upgraded [SQLite](https://www.sqlite.org/index.html) (`3.16.2` -> `3.19.3`).
* Added [Cppcheck](https://github.com/danmar/cppcheck) definition file (`FlockSDK.cppcheck`), so users can help with fixing C++-related issues.

## Removals/deprecation of features
* Removed `ScriptCompiler` from the [tools repository](https://github.com/Florastamine/IWBHT_SDK_Tools).
* Dropped [Mir](http://unity.ubuntu.com/mir/index.html) support. The only thing that have been keeping Mir alive was Unity, but now with Canonical dropping Unity from future releases of Ubuntu, there's no reason to keep it around the code base.
* Dropped [libcpuid](https://github.com/anrieff/libcpuid) as a dependency in favor of self-made functions for detecting the number of CPU cores/threads on Windows platform.

## New features
* Added support for first-person view renderable objects. Assign the `bin/pfiles/shaders/techniques/DiffViewRenderables.xml` material to whichever objects you want to mark as unclippable by scene geometry.
* Integration of volumetric lighting for both forward (`ForwardVolumetricLighting*.xml`) & deferred (`DeferredVolumetricLighting*.xml`) rendering path.
* Ready-to-use, newly added post-process effects as well as standard shaders: outline (shader); Commodore 64 (C64) (post-process); glitch (post-process); sine (post-process); sketch (post-process); pencil (post-process); dithering (post-process).
* A large selection of APIs for querying various properties of the currently running machine. (`GetTotalMemory()`, `GetLoginName()`, `GetHostName()`, `GetOSVersion()`, `GetHomePath()`, `GetTemporaryPath()`, `GetCPUBigEndian()`, `GetCPULittleEndian()`, `GetCPUClock()`, `GetCPUArchitecture()`, `GetCPUVendorID()`, `GetCPUBrandName()`, `GetCPUExtensions()`, `GetLocale()`, `GetEnvVar()`, `HasEnvVar()`, `SetClipboard()`, `GetClipboard()`, `GetBatteryPercentage()`, `GetBatteryTimeLeft()`, `OpenProcessHandle()`, `CloseProcessHandle()`, `KillProcess()`).
* APIs for querying OpenGL/GLSL version, GPU name, GPU basic capabilities (memory, maximum texture size, supported OpenGL extensions) (`Graphics::GetAPIVersion()`, `Graphics::GetAdapterName()`, `Graphics::GetGPUMaxTextureSize()`, `Graphics::GetNumSupportedExtensions()`, `Graphics::GetSupportedExtensions()`, `Graphics::HasExtension()`, `Graphics::GetTotalVideoMemory()`)
* APIs for querying time/date data: `Timer::GetSystemTimeUnix()`, `Timer::GetSystemTimeAsString()`, `Timer::GetSystemTimeUnixAsString()`, `Timer::GetTimeStamp()`, `Timer::GetTimeStampAsString()`.
* Per-character coloring is now possible through `Text::SetColorCharacter()`, and partial support for multi-colored texts was added through `Text::SetTextFormatted()`.
* Networking support was added back.
* Added `String::FindFirstOf()`.

## Changes/bugfixes/new features merged from Urho3D:
* Various bug-fixes and tweaks. 
* Font-related functions (`Text::SetFont()`, `Text::SetFontSize()`, `Text::GetFontSize()`,...) now takes `float` as arguments instead of `int`, which allows for fractional font size.
* Upgraded [FreeType](https://www.freetype.org/) (`v2.7.1` -> `v2.8`).
* Upgraded [Bullet](http://bulletphysics.org/wordpress/) (`2.84` -> `2.86`).
* Upgraded [rapidjson](https://github.com/miloyip/rapidjson) (`0.1` -> `1.1.0`).
* Added a few helper functions for `Matrix3`, `Matrix3x4` and `Matrix4`.
* Added `Base64` decoding support when loading `.tmx` files.
* Added `DecodeBase64()`.

* **Input**
* Added `Input::GetInputScale()`.

* **Vector3**
* Added `Vector3::Orthogonalize()`.

* **VertexBuffer**
* Added `VertexBuffer::UpdateOffsets()`.

* **Terrain**
* Added `Terrain::HeightMapToWorld()`.

* **Sphere**
* Added `Sphere::GetLocalPoint()`, `Sphere::GetPoint()`.

* **DebugRenderer**
* Added `DebugRenderer::AddSphereSector()`.

* **Graphics**
* Added `Graphics::GetSize()`.

* **JSONValue**
* Added `JSONValue::SetVariant()`, `JSONValue::SetVariantMap()`, `JSONValue::GetVariant()`, `JSONValue::GetVariantMap()`.


