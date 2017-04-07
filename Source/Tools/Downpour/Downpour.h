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

#pragma once 

#include <Urho3D/Engine/Application.h> 

namespace Downpour {
    class DownpourBase : public Urho3D::Application {
        URHO3D_OBJECT(DownpourBase, Urho3D::Application);

        public:
            DownpourBase(Urho3D::Context *context);

            virtual void Setup();
            virtual void Start();
            virtual void Stop();

            int argc_;
            Urho3D::String argv_;
            Urho3D::String moduleName_;
            Urho3D::SharedPtr<Urho3D::LuaScript> moduleEditorPtr_;
        private:
            void HandleScriptReloadStarted(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
            void HandleScriptReloadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
            void HandleScriptReloadFailed(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData); 
            void Exit(void);
    };
};

namespace Downpour {
    inline const char *GetEditorBootArg() { return("--run-editor"); }
    inline const char *GetRawScriptLocation() { return("pfiles/core-main.lua"); }
    inline const char *GetCompiledScriptLocation() { return("pfiles/core-main.dcs"); }
    inline const char *GetSDKLocation() { return("pfiles/development/SDK_editor.lua"); } 
    inline const char *GetCompiledScriptExtension() { return (".dcs"); } 
    inline const char *GetScriptExtension() { return(".lua"); } 
};
