//
// Copyright (c) 2008-2017 Flock SDK developers & contributors. 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../Precompiled.h"

#include "../Core/StringUtils.h"
#include "../Core/APIVersionQuery.h"

#include <LuaJIT/luajit.h>
#include <Bullet/LinearMath/btScalar.h>
#include <GLEW/glew.h>
#include <ft2build.h>
#include <FreeType/freetype.h>
#include <SDL/SDL_version.h>
#include <SQLite/sqlite3.h>
#include <PugiXml/pugixml.hpp>
#include <LZ4/lz4.h>
#include <rapidjson/rapidjson.h>
#include <toluapp/tolua++.h>

// Cache fixed strings so they don't have to be re-calculated each time the function is called.
static const FlockSDK::String BulletVersion(FlockSDK::ToString("Bullet %i", BT_BULLET_VERSION));
static const FlockSDK::String GLEWVersion(FlockSDK::ToString("GLEW %i.%i.%i.%i", GLEW_VERSION, GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO));
static const FlockSDK::String FreeTypeVersion(FlockSDK::ToString("FreeType %i.%i.%i", FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH));
static const FlockSDK::String SDLVersion(FlockSDK::ToString("SDL %i.%i.%i", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL));
static const FlockSDK::String SQLiteVersion(FlockSDK::String("SQLite ") + SQLITE_VERSION);
static const FlockSDK::String PugiXMLVersion(FlockSDK::ToString("PugiXml %i", PUGIXML_VERSION));
static const FlockSDK::String LZ4Version(FlockSDK::String("LZ4 ") + LZ4_VERSION_STRING);
static const FlockSDK::String RapidJSONVersion(FlockSDK::String("RapidJSON ") + RAPIDJSON_VERSION_STRING);
static const FlockSDK::String JOJPEGVersion("jo_jpeg 1.52"); // Have to rely on the release note.

namespace FlockSDK
{

String GetLuaJITVersion()
{
    return LUAJIT_VERSION;
}

String GetBulletVersion()
{
    return BulletVersion;
}

String GetGLEWVersion()
{
    return GLEWVersion;
}

String GetFreeTypeVersion()
{
    return FreeTypeVersion;
}

String GetSDLVersion()
{
    return SDLVersion;
}

String GetSQLiteVersion()
{
    return SQLiteVersion;
}

String GetPugiXmlVersion()
{
    return PugiXMLVersion;
}

String GetLZ4Version()
{
    return LZ4Version;
}

String GetRapidJSONVersion()
{
    return RapidJSONVersion;
}

String GetJOJPEGVersion()
{
    return JOJPEGVersion;
}

String GettoluappVersion()
{
    return TOLUA_VERSION;
}

}
