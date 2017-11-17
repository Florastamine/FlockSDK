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

#include <Flock/Core/Platform.h>
#include <Flock/Engine/Engine.h>
#include <Flock/IO/FileSystem.h>
#include <Flock/LuaScript/LuaScript.h>
#include <Flock/Resource/ResourceCache.h>
#include <Flock/Resource/ResourceEvents.h>

#include "Downpour.h"

namespace SDK = FlockSDK;

static inline constexpr const char *GetRawScriptLocation() { return("pfiles/main.lua"); }
static inline constexpr const char *GetCompiledScriptLocation() { return("pfiles/main.dcs"); }

DownpourBase::DownpourBase(SDK::Context* context) : SDK::Application(context) {}

void DownpourBase::Setup()
{
    auto *fsObject = GetSubsystem<SDK::FileSystem>();

    if (fsObject)
    {
        // Search order: checks for arguments -> loads the script specified in the arguments, or tries to
        // load the compiled version of the script (which has the .s) extension, finally tries to switch
        // to the plain text version of the script file.
        if (argv_ != SDK::String::EMPTY && argv_[0] != '-')
            moduleName_ = argv_.Replaced('\\', '/');
        else if (fsObject->FileExists(GetCompiledScriptLocation()))
            moduleName_ = GetCompiledScriptLocation();
        else
            moduleName_ = GetRawScriptLocation();
    }
}

void DownpourBase::Start()
{
    auto *luaScript = new SDK::LuaScript(context_);
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
    auto *luaScript = GetSubsystem<SDK::LuaScript>();
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
    auto *DPContext = new SDK::Context();
    auto *DPGame = new DownpourBase(DPContext);
    DPGame->argc_ = argc;
    DPGame->argv_ = (argc > 1 && argv[1]) ? argv[1] : SDK::String::EMPTY;

    SDK::ParseArguments(argc, argv);

    return (SDK::SharedPtr<DownpourBase>(DPGame))->Run();
}
