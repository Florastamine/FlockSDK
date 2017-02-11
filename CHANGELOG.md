# Release 1 -- 01.06.2017 ([download](https://drive.google.com/drive/folders/0Bx0q4f1kFRaiRmcxNTNnVi1QZXc?usp=sharing))
Initial release. 

# Release 2 -- 01.27.2017 ([download](https://drive.google.com/drive/folders/0Bx0q4f1kFRaidm5UdHh3Rm1tMjQ?usp=sharing)) 
These builds incorporated a few bug-fixes introduced in the main `Urho3D` repository as well as new features and upgrades of existing 3rd party libraries. Most notably: 
* Upgraded [AngelScript](http://www.angelcode.com/angelscript/) from `2.30.2` to `2.31.2`. 
* Completely removed support for touch controls and touch input. (Because `Windows` and `Linux` are the only two target platforms that are officially supported, and touch controls are more popular on mobile devices. Also the code base would be simplified a lot.) 
* Gotten rid of legacy and/or unpopular file formats to reduce the size of the AssetImporter tool. (around 5 MiB was cut due to the dropping of a lot of file formats) 
* New editor features: Show object's origin and paint selection. 

# Release 3 -- 02.11.2017 ([download](https://drive.google.com/open?id=0Bx0q4f1kFRaiOWFaUnAtcTZLYms))
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
