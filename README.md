## Welcome! 

**Q: What is this mess?** 
A: It's a general-purpose game engine written in a mixutre of C and C++, and provides scripting support through Lua/LuaJIT. Originally derived from the [Urho3D game engine](https://github.com/urho3d/Urho3D) when the engine is `322 commits` ahead of its `1.6` release, and since then the fork has been received patches, modifications, cuts and updates introduced by both the [Urho3D](https://github.com/urho3d/Urho3D) developers and myself. It's now being used for my upcoming video game project, and as such, it contains changes and additions that are very specific to meet my goals and thus probably aren't very useful for everyone else. 

**Q: What are the exact differences between Urho3D and Downpour/IWBHT_SDK?** 
A: In short: Heavy focus on Windows and Linux as both development and target platforms; Integration of various third-party libraries/tools, natively; Upgrades of several dependencies to their newer versions; A growing collection of pre-built, ready-to-use shaders with customizable parameters; C++11 with std-based containers instead of Urho3D-based containers; Bugfixes and upgrades to the core API, as well as a Lua-based standard library to help you in getting started in writing applications and games in Downpour. 

**Q: Why did you remove support for macOS?** 
A: Because I don't have a Mac myself for proper testing. If there's enough interest, I will add some sort of CI testing for Mac builds. 

**Q: Why did you remove AngelScript?** 
A: Because in my game projects, I use only Lua (for the most parts) and C++ (for performance-critical parts). Besides, Lua is widely adopted and used in the video game industry, and thus support and/or the number of available libraries and tools is very large, giving me a lot of options when writing game logic in Lua. 

**Q: Wait, what about the editor then? Isn't it written in AngelScript?** 
A: The editor will go through a complete Lua rewrite. 

**Q: Is there some sort of pre-built binaries for a quick look at the engine?** 
A: Yes ([here!](https://drive.google.com/drive/folders/0Bx0q4f1kFRaieTBhZHJJYzRNRHc?usp=sharing), along with [release notes](https://github.com/Florastamine/IWBHT_SDK/blob/master/CHANGELOG.md)). Pre-built binaries and static libraries are both provided. For shared libraries, though, you will have to build it yourself. 

One thing though, these binaries are provided with the intention of providing a quick way of testing and playing around with the SDK, and thus they are not guaranteed to be stable enough for development purposes. These binaries include both executable files which can be run off straight away or static libraries which can be linked alongside your code. 

Latest **unstable** binaries were built and released on `03.31.2017`, and can be downloaded [here](https://drive.google.com/drive/folders/0Bx0q4f1kFRaieTBhZHJJYzRNRHc?usp=sharing). Older versions can still be downloaded [on the release notes page](https://github.com/Florastamine/IWBHT_SDK/blob/master/CHANGELOG.md). 

**Q: Any instructions on how to build the engine?** 
A: Instructions are provided [here](https://github.com/Florastamine/IWBHT_SDK/blob/master/BUILDING.md). In short, you will need `cmake` and `g++`. `msvc` isn't supported. A `C++11`-conformant compiler is required for the building process. If you're building on Windows, you can use [MinGW-W64](https://sourceforge.net/projects/mingw-w64/) instead. 

**Q: License?** 
A: The fork follows the same license as [Urho3D](https://github.com/urho3d/Urho3D), and there is no intention of keeping it compatible with the original code. 