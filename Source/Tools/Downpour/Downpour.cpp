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

#ifdef    URHO3D_ANGELSCRIPT 
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/AngelScript/Script.h>
#endif 

#include <Urho3D/Core/Main.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>

#ifdef    URHO3D_LUA 
#include <Urho3D/LuaScript/LuaScript.h> 
#endif 

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>

#include "Downpour.h"

#include <Urho3D/DebugNew.h> 

namespace Downpour {

DownpourBase::DownpourBase(Urho3D::Context* context) :
Urho3D::Application(context)
{
} 

void DownpourBase::Setup()
{
    Urho3D::FileSystem *fsObject = GetSubsystem<Urho3D::FileSystem>();

    if(fsObject)
    {
        if(! (argc_ > 1 && argv_ == GetEditorBootArg()))
        {
            if(fsObject->FileExists(GetCompiledScriptLocation()))
                moduleName_ = Urho3D::String(GetCompiledScriptLocation());
            else if(fsObject->FileExists(GetRawScriptLocation())) 
                moduleName_ = Urho3D::String(GetRawScriptLocation());
        }
        else 
        {
            moduleName_ = Urho3D::String(GetSDKLocation());
        }
    }
}

void DownpourBase::Start()
{
    Urho3D::String extension = Urho3D::GetExtension(moduleName_);
    if (extension != ".lua" && extension != ".luc")
    {
#ifdef URHO3D_ANGELSCRIPT
        // Instantiate and register the AngelScript subsystem
        context_->RegisterSubsystem(new Urho3D::Script(context_));

        // Hold a shared pointer to the script file to make sure it is not unloaded during runtime
        moduleEditorPtr_ = GetSubsystem<Urho3D::ResourceCache>()->GetResource<Urho3D::ScriptFile>(moduleName_);

        ///hack If we are running the editor, also instantiate Lua subsystem to enable editing Lua ScriptInstances
#ifdef URHO3D_LUA
        if (moduleName_.Contains("SDK"))
            context_->RegisterSubsystem(new Urho3D::LuaScript(context_));
#endif
        // If script loading is successful, proceed to main loop
        if (moduleEditorPtr_ && moduleEditorPtr_->Execute("void Start()"))
        {
            // Subscribe to script's reload event to allow live-reload of the application
            SubscribeToEvent(moduleEditorPtr_, Urho3D::E_RELOADSTARTED, URHO3D_HANDLER(Downpour::DownpourBase, HandleScriptReloadStarted));
            SubscribeToEvent(moduleEditorPtr_, Urho3D::E_RELOADFINISHED, URHO3D_HANDLER(Downpour::DownpourBase, HandleScriptReloadFinished));
            SubscribeToEvent(moduleEditorPtr_, Urho3D::E_RELOADFAILED, URHO3D_HANDLER(Downpour::DownpourBase, HandleScriptReloadFailed));
            return;
        }
#else
        ErrorExit("AngelScript is not enabled!");
        return;
#endif
    }
    else
    {
#ifdef URHO3D_LUA
        // Instantiate and register the Lua script subsystem
        Urho3D::LuaScript* luaScript = new Urho3D::LuaScript(context_);
        context_->RegisterSubsystem(luaScript);

        // If script loading is successful, proceed to main loop
        if (luaScript->ExecuteFile(moduleName_))
        {
            luaScript->ExecuteFunction("Start");
            return;
        }
#else
        ErrorExit("Lua is not enabled!");
        return;
#endif
    }

    // The script was not successfully loaded. Show the last error message and do not run the main loop
    ErrorExit();
}

void DownpourBase::Stop()
{
#ifdef URHO3D_ANGELSCRIPT
    if (moduleEditorPtr_)
    {
        // Execute the optional stop function
        if (moduleEditorPtr_->GetFunction("void Stop()"))
            moduleEditorPtr_->Execute("void Stop()");
    }
#else
    if (false)
    {
    }
#endif

#ifdef URHO3D_LUA
    else
    {
        Urho3D::LuaScript* luaScript = GetSubsystem<Urho3D::LuaScript>();
        if (luaScript && luaScript->GetFunction("Stop", true))
            luaScript->ExecuteFunction("Stop");
    }
#endif
}

void DownpourBase::HandleScriptReloadStarted(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
#ifdef URHO3D_ANGELSCRIPT
    if (moduleEditorPtr_->GetFunction("void Stop()"))
        moduleEditorPtr_->Execute("void Stop()");
#endif
}

void DownpourBase::HandleScriptReloadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
#ifdef URHO3D_ANGELSCRIPT
    // Restart the script application after reload
    if (!moduleEditorPtr_->Execute("void Start()"))
    {
        moduleEditorPtr_.Reset();
        ErrorExit();
    }
#endif
}

void DownpourBase::HandleScriptReloadFailed(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
#ifdef URHO3D_ANGELSCRIPT
    moduleEditorPtr_.Reset();
    ErrorExit();
#endif
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
    DPGame->argv_ = argc > 1 && argv[1] ? Urho3D::String(argv[1]) : Urho3D::String();

    return (Urho3D::SharedPtr<Downpour::DownpourBase>(DPGame))->Run();
} 

