This is an incomplete list of things that need to be done before the SDK can be able to reach `v1.0.0`, in no particular order. However, without even reaching `v1.0.0`, the SDK is already in a fairly stable state (except the experimental feature(s), which can be enabled through `FLOCK_EXPERIMENTAL`), and can be downloaded, compiled and used straight away.

## Pre-`v1.0.0`
* Re-work the build tools, which still wraps around CMake scripts, but also provides an optional GUI shell, allowing the user to generate a custom .bat or .sh script with the configuration specified in the GUI for building the SDK.
* Write examples for fork-specific new features/changes, and also port various examples from the original engine for completeness and to provide a way of studying the engine API.
* Implement basic game script encryption/decryption, with a private key provided by the user during the compilation process.
those who looking into writing games and/or applications with the SDK.* Integrate Auburns/FastNoiseSIMD into the SDK, with functionalities provided for SSE2 2D/3D Perlin/Simplex/White noise. 
* Add different PRNGs besides the default one, for example, Mersenne Twister, multiply-with-carry, and so on.
* Upgrade LuaJIT to `LuaJIT 2.1.0-beta3`.
* Utility program which does compiling the release version of the engine/game, packaging assets, wrapping up game scripts, cleaning up, doing post-compilation steps (UPX compression, etc.).
* Add several more math functions: numerical integration with different methods, noise generation,...
* Mechanism for random human name generation.
* Steamworks SDK integration.
* `std`-based containers (`std::string`, `std::vector<>`, to name a few) will have bigger influence, and will slowly replace the SDK's self-made containers.
* Rewrite the legacy, AngelScript-based editor in full C++11, which should provide a performance boost, and so I can be able to completely remove AngelScript as a dependency.
* `RichText3D` component integration (with example code, of course).
* Integration of Downpour, which is a set of Lua standard libraries.
* Proper Linux support, and, possibly, macOS.
* Vietnamese translation for the scene editor.

## Post-`v1.0.0`
* A complete, working, first-person game demo, written in both Lua and C++11, with the purpose of showcasing various features of the engine and provides a starting point for 
