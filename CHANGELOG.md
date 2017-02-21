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
* Dropped the switch to enable C++11 (`-DURHO3D_C++11`). This is an old switch that was left from the `Urho3D` codebase and is used to enable C++11 support, but it was dropped and now C++11 is mandatorily required. 
* The 2D component is now an integral part of the SDK, which adds an additional ~2.0 `MiB` overhead to the built executable. 
* Upgraded [Open Asset Import Library](http://www.assimp.org) from `3.1.1` to `3.3.1`, **thus effectively rendering pre-C++11 compilers to not be able to compile the SDK**. **(*)** 
* Upgraded [pugixml](http://pugixml.org) from `1.7` to `1.8`. 
* Upgraded [libcpuid](https://github.com/anrieff/libcpuid) from `0.2.2` to `0.4.0`. 
* Upgraded [Box2D](https://github.com/erincatto/Box2D) to commit [`71a6079b5fa717b8b56aad6a1130b74232989183`](https://github.com/erincatto/Box2D/commit/71a6079b5fa717b8b56aad6a1130b74232989183). 
* Dropped networking from the codebase (including [Civetweb](https://github.com/civetweb/civetweb) and [kNet](https://github.com/juj/kNet)). 
* `AssetImporter`, `RampGenerator`, `ScriptCompiler` and `SpritePacker` were taken out of the codebase and thus won't be included during the compilation process. **(..)**

**(.)** The upgrade of [Open Asset Import Library](http://www.assimp.org) from `3.1.1` to `3.3.1` brought to the table the ability to import from [3MF](http://www.3mf.io/what-is-3mf/), [SIB](https://en.wikipedia.org/wiki/Silo_(software)), and [glTF](https://en.wikipedia.org/wiki/GlTF). However, in this release, I've also dropped support for [SMD](https://developer.valvesoftware.com/wiki/Studio_Model_Data), [AC](https://en.wikipedia.org/wiki/AC3D), and Unreal's [.3D](http://paulbourke.net/dataformats/unreal/). 

**(..)** The rationale behind this is, for the parts are not part of the codebase (helper tools, for example) that rarely change, there's no point in keep re-compiling the code that will not (or rarely) change again and again when building the SDK, which would waste a lot of time, and thus these tools were removed from the codebase and are instead provided under the form of 32-bit pre-built binaries. Now the tools will only be provided under binary format, and their separate repository can be found [here](https://github.com/Florastamine/IWBHT_SDK_Tools). 
