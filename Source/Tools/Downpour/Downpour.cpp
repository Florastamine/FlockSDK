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

#include <Flock/AngelScript/ScriptFile.h>
#include <Flock/AngelScript/Script.h>
#include <Flock/Engine/Engine.h>
#include <Flock/IO/FileSystem.h>
#include <Flock/IO/Log.h>
#include <Flock/LuaScript/LuaScript.h>
#include <Flock/Resource/ResourceCache.h>
#include <Flock/Resource/ResourceEvents.h>

#include "Downpour.h"

static inline constexpr const char *GetSDKLocation() { return("pfiles/development/SDK_editor.as"); } 

DownpourBase::DownpourBase(FlockSDK::Context* context) : FlockSDK::Application(context) {}

void DownpourBase::Setup()
{
    moduleName_ = GetSDKLocation();
}

void DownpourBase::Start()
{
    // Instantiate and register the AngelScript subsystem
    context_->RegisterSubsystem(new FlockSDK::Script(context_));

    // Hold a shared pointer to the script file to make sure it is not unloaded during runtime
    moduleEditorPtr_ = GetSubsystem<FlockSDK::ResourceCache>()->GetResource<FlockSDK::ScriptFile>(moduleName_);

    if (moduleName_.Contains("SDK", false))
        context_->RegisterSubsystem(new FlockSDK::LuaScript(context_)); 

    if (moduleEditorPtr_ && moduleEditorPtr_->Execute("void Start()"))
    {
        // Subscribe to script's reload event to allow live-reload of the application
        SubscribeToEvent(moduleEditorPtr_, FlockSDK::E_RELOADSTARTED, FLOCKSDK_HANDLER(DownpourBase, HandleScriptReloadStarted));
        SubscribeToEvent(moduleEditorPtr_, FlockSDK::E_RELOADFINISHED, FLOCKSDK_HANDLER(DownpourBase, HandleScriptReloadFinished));
        SubscribeToEvent(moduleEditorPtr_, FlockSDK::E_RELOADFAILED, FLOCKSDK_HANDLER(DownpourBase, HandleScriptReloadFailed));
        return;
    }

    ErrorExit();
}

void DownpourBase::Stop()
{
    if (moduleEditorPtr_->GetFunction("Stop"))
        moduleEditorPtr_->Execute("Stop");
}

void DownpourBase::HandleScriptReloadStarted(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData)
{
    if (moduleEditorPtr_->GetFunction("Stop"))
        moduleEditorPtr_->Execute("Stop");
}

void DownpourBase::HandleScriptReloadFinished(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData)
{
    // Restart the script application after reload
    if (!moduleEditorPtr_->Execute("Start"))
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

int main(int argc, char **argv) 
{
    auto *DPContext = new FlockSDK::Context();
    auto *DPGame    = new DownpourBase(DPContext);
    DPGame->argc_   = argc;
    DPGame->argv_   = (argc > 1 && argv[1]) ? argv[1] : FlockSDK::String::EMPTY;

    return (FlockSDK::SharedPtr<DownpourBase>(DPGame))->Run();
} 
