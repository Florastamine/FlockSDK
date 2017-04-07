//
// Copyright (c) 2008-2016 the Urho3D project.
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

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/LuaScript/LuaScript.h>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>

#include "Downpour.h"

namespace Downpour {

DownpourBase::DownpourBase(Urho3D::Context* context) : Urho3D::Application(context) {}

void DownpourBase::Setup()
{
    auto fsObject = GetSubsystem<Urho3D::FileSystem>();

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
    context_->RegisterSubsystem(new Urho3D::LuaScript(context_)); 
    auto scriptHandle = GetSubsystem<Urho3D::LuaScript>();

    if (scriptHandle->ExecuteFile(moduleName_)) 
    {
        scriptHandle->ExecuteFunction("Start"); 
        if (moduleName_.Contains("SDK")) 
        {
            SubscribeToEvent(moduleEditorPtr_, Urho3D::E_RELOADSTARTED, URHO3D_HANDLER(Downpour::DownpourBase, HandleScriptReloadStarted));
            SubscribeToEvent(moduleEditorPtr_, Urho3D::E_RELOADFINISHED, URHO3D_HANDLER(Downpour::DownpourBase, HandleScriptReloadFinished));
            SubscribeToEvent(moduleEditorPtr_, Urho3D::E_RELOADFAILED, URHO3D_HANDLER(Downpour::DownpourBase, HandleScriptReloadFailed));
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
        Urho3D::LuaScript* luaScript = GetSubsystem<Urho3D::LuaScript>();
        if (luaScript && luaScript->GetFunction("Stop", true))
            luaScript->ExecuteFunction("Stop");
    }
}

void DownpourBase::HandleScriptReloadStarted(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    if (moduleEditorPtr_->GetFunction("Stop"))
        moduleEditorPtr_->ExecuteFunction("Stop");
}

void DownpourBase::HandleScriptReloadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    // Restart the script application after reload
    if (!moduleEditorPtr_->ExecuteFunction("Start"))
    {
        moduleEditorPtr_.Reset();
        ErrorExit();
    }
}

void DownpourBase::HandleScriptReloadFailed(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
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
    Urho3D::Context         *DPContext = new Urho3D::Context();
    Downpour::DownpourBase  *DPGame    = new Downpour::DownpourBase(DPContext);
    DPGame->argc_ = argc;
    DPGame->argv_ = argc > 1 && argv[1] ? argv[1] : Urho3D::String::EMPTY;

    return (Urho3D::SharedPtr<Downpour::DownpourBase>(DPGame))->Run();
} 
