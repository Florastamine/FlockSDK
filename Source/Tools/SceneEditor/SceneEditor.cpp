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

#include <Flock/Core/CoreEvents.h>
#include <Flock/Engine/Engine.h>
#include <Flock/IO/FileSystem.h>
#include <Flock/IO/Log.h>
#include <Flock/LuaScript/LuaScript.h>
#include <Flock/Resource/ResourceCache.h>
#include <Flock/Resource/ResourceEvents.h>
#include <Flock/Resource/Localization.h>
#include <Flock/Input/Input.h>

#include "SceneEditor.h" 

#include <cassert>

SceneEditorBase::SceneEditorBase(FlockSDK::Context* context) : FlockSDK::Application(context) {}

void SceneEditorBase::Setup()
{
}

void SceneEditorBase::Start()
{
    auto *i18n = GetSubsystem<FlockSDK::Localization>();
    auto *engine = GetSubsystem<FlockSDK::Engine>();
    auto *input = GetSubsystem<FlockSDK::Input>(); 

    assert(i18n && engine && input); 

    i18n->LoadJSONFile("development/SDK_editor/EditorStrings.json"); 

    if (engine->IsHeadless())
        ErrorExit();
    
    input->SetMouseVisible(true); 

    SubscribeToEvent(FlockSDK::E_UPDATE, FLOCKSDK_HANDLER(SceneEditorBase, HandleUpdate));
}

void SceneEditorBase::Stop()
{
}

void SceneEditorBase::Exit(void) 
{
}

void SceneEditorBase::HandleUpdate(FlockSDK::StringHash eventType, FlockSDK::VariantMap &eventData)
{
}

int main(int argc, char **argv) 
{
    auto *DPContext = new FlockSDK::Context();
    auto *DPGame    = new SceneEditorBase(DPContext);
    DPGame->argc_   = argc;
    DPGame->argv_   = (argc > 1 && argv[1]) ? argv[1] : FlockSDK::String::EMPTY;

    return (FlockSDK::SharedPtr<SceneEditorBase>(DPGame))->Run();
} 
