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

#include "SceneEditor.Legacy.h"

static constexpr const char *SDKEditorScriptFile = "pfiles/development/SDK_editor.as";

SceneEditorLegacyBase::SceneEditorLegacyBase(FlockSDK::Context* context) : FlockSDK::Application(context) {}

void SceneEditorLegacyBase::Setup()
{
}

int main(int argc, char **argv) 
{
    auto *DPContext = new FlockSDK::Context();
    auto *DPGame    = new SceneEditorLegacyBase(DPContext);
    DPGame->argc_   = argc;
    DPGame->argv_   = (argc > 1 && argv[1]) ? argv[1] : FlockSDK::String::EMPTY;

    return (FlockSDK::SharedPtr<SceneEditorLegacyBase>(DPGame))->Run();
}

void SceneEditorLegacyBase::Start()
{
    context_->RegisterSubsystem(new FlockSDK::Script(context_));
    context_->RegisterSubsystem(new FlockSDK::LuaScript(context_));
    scriptFile_ = GetSubsystem<FlockSDK::ResourceCache>()->GetResource<FlockSDK::ScriptFile>(SDKEditorScriptFile);

    if (scriptFile_ && scriptFile_->Execute("void Start()"))
    {
        // Subscribe to script's reload event to allow live-reload of the application
        SubscribeToEvent(scriptFile_, FlockSDK::E_RELOADSTARTED, FLOCKSDK_HANDLER(SceneEditorLegacyBase, HandleScriptReloadStarted));
        SubscribeToEvent(scriptFile_, FlockSDK::E_RELOADFINISHED, FLOCKSDK_HANDLER(SceneEditorLegacyBase, HandleScriptReloadFinished));
        SubscribeToEvent(scriptFile_, FlockSDK::E_RELOADFAILED, FLOCKSDK_HANDLER(SceneEditorLegacyBase, HandleScriptReloadFailed));
        return;
    }
    ErrorExit();
}

void SceneEditorLegacyBase::Stop()
{
    if (scriptFile_)
    {
        // Execute the optional stop function
        if (scriptFile_->GetFunction("void Stop()"))
            scriptFile_->Execute("void Stop()");
    }
}

void SceneEditorLegacyBase::HandleScriptReloadStarted(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData)
{
    if (scriptFile_->GetFunction("void Stop()"))
        scriptFile_->Execute("void Stop()");
}

void SceneEditorLegacyBase::HandleScriptReloadFinished(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData)
{
    // Restart the script application after reload
    if (!scriptFile_->Execute("void Start()"))
    {
        scriptFile_.Reset();
        ErrorExit();
    }
}

void SceneEditorLegacyBase::HandleScriptReloadFailed(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData)
{
    scriptFile_.Reset();
    ErrorExit();
}
