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

#include <Flock/Engine/Engine.h>
#include <Flock/IO/FileSystem.h>
#include <Flock/IO/Log.h>
#include <Flock/LuaScript/LuaScript.h>
#include <Flock/Resource/ResourceCache.h>
#include <Flock/Resource/ResourceEvents.h>

#include "Downpour.h"

static inline constexpr const char *GetRawScriptLocation() { return("pfiles/main.lua"); }
static inline constexpr const char *GetCompiledScriptLocation() { return("pfiles/main.dcs"); }

DownpourBase::DownpourBase(FlockSDK::Context* context) : FlockSDK::Application(context) {}

void DownpourBase::Setup()
{
    auto *fsObject = GetSubsystem<FlockSDK::FileSystem>();

    if (fsObject)
    {
        if (fsObject->FileExists(GetCompiledScriptLocation()))
            moduleName_ = GetCompiledScriptLocation();
        else if (fsObject->FileExists(GetRawScriptLocation())) 
            moduleName_ = GetRawScriptLocation();
    }
} 

void DownpourBase::Start()
{
    auto *luaScript = new FlockSDK::LuaScript(context_);
    context_->RegisterSubsystem(luaScript);

    if (luaScript->ExecuteFile(moduleName_))
    {
        luaScript->ExecuteFunction("Start");
        return;
    }
    ErrorExit();
}

void DownpourBase::Stop()
{
    auto *luaScript = GetSubsystem<FlockSDK::LuaScript>();
    if (luaScript && luaScript->GetFunction("Stop", true))
        luaScript->ExecuteFunction("Stop");
}

void DownpourBase::Exit(void) 
{
    engine_->Exit();
    exitCode_ = EXIT_FAILURE;
}

int main(int argc, char **argv) 
{
    auto *DPContext = new FlockSDK::Context();
    auto *DPGame    = new DownpourBase(DPContext);
    DPGame->argc_   = argc;
    DPGame->argv_   = (argc > 1 && argv[1]) ? argv[1] : FlockSDK::String::EMPTY;

    return (FlockSDK::SharedPtr<DownpourBase>(DPGame))->Run();
} 
