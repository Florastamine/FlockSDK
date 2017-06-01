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

namespace Downpour {

DownpourBase::DownpourBase(FlockSDK::Context* context) : FlockSDK::Application(context) {}

void DownpourBase::Setup()
{
    auto fsObject = GetSubsystem<FlockSDK::FileSystem>();

    if(fsObject)
    {
        if (!(argc_ > 1 && argv_ == GetEditorBootArg()))
        {
            if(fsObject->FileExists(GetCompiledScriptLocation()))
                moduleName_ = GetCompiledScriptLocation();
            else if(fsObject->FileExists(GetRawScriptLocation())) 
                moduleName_ = GetRawScriptLocation();
        }
        else 
        {
            moduleName_ = GetSDKLocation();
        }
    }
} 

void DownpourBase::Start()
{
    context_->RegisterSubsystem(new FlockSDK::LuaScript(context_)); 
    auto scriptHandle = GetSubsystem<FlockSDK::LuaScript>();

    if (scriptHandle->ExecuteFile(moduleName_)) 
    {
        scriptHandle->ExecuteFunction("Start"); 
        if (moduleName_.Contains("SDK")) 
        {
            SubscribeToEvent(moduleEditorPtr_, FlockSDK::E_RELOADSTARTED, FLOCKSDK_HANDLER(Downpour::DownpourBase, HandleScriptReloadStarted));
            SubscribeToEvent(moduleEditorPtr_, FlockSDK::E_RELOADFINISHED, FLOCKSDK_HANDLER(Downpour::DownpourBase, HandleScriptReloadFinished));
            SubscribeToEvent(moduleEditorPtr_, FlockSDK::E_RELOADFAILED, FLOCKSDK_HANDLER(Downpour::DownpourBase, HandleScriptReloadFailed));
        }
        return; 
    }

    // The script was not successfully loaded. Show the last error message and do not run the main loop
    ErrorExit();
}

void DownpourBase::Stop()
{
    if (moduleEditorPtr_)
    {
        // Execute the optional stop function
        if (moduleEditorPtr_->GetFunction("Stop"))
            moduleEditorPtr_->ExecuteFunction("Stop");
    }
    else
    {
        FlockSDK::LuaScript* luaScript = GetSubsystem<FlockSDK::LuaScript>();
        if (luaScript && luaScript->GetFunction("Stop", true))
            luaScript->ExecuteFunction("Stop");
    }
}

void DownpourBase::HandleScriptReloadStarted(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData)
{
    if (moduleEditorPtr_->GetFunction("Stop"))
        moduleEditorPtr_->ExecuteFunction("Stop");
}

void DownpourBase::HandleScriptReloadFinished(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData)
{
    // Restart the script application after reload
    if (!moduleEditorPtr_->ExecuteFunction("Start"))
    {
        moduleEditorPtr_.Reset();
        ErrorExit();
    }
}

void DownpourBase::HandleScriptReloadFailed(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData)
{
    moduleEditorPtr_.Reset();
    ErrorExit();
}

void DownpourBase::Exit(void) 
{
    engine_->Exit();
    exitCode_ = EXIT_FAILURE;
}

};

int main(int argc, char **argv) 
{
    auto *DPContext = new FlockSDK::Context();
    auto *DPGame    = new Downpour::DownpourBase(DPContext);
    DPGame->argc_   = argc;
    DPGame->argv_   = (argc > 1 && argv[1]) ? argv[1] : FlockSDK::String::EMPTY;

    return (FlockSDK::SharedPtr<Downpour::DownpourBase>(DPGame))->Run();
} 
