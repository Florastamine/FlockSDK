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

#pragma once 

#include <Flock/Engine/Application.h> 

class SceneEditorLegacyBase : public FlockSDK::Application {
    FLOCKSDK_OBJECT(SceneEditorLegacyBase, FlockSDK::Application);

    public:
        SceneEditorLegacyBase(FlockSDK::Context *context);

        virtual void Setup();
        virtual void Start();
        virtual void Stop();

        void HandleScriptReloadStarted(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData);
        void HandleScriptReloadFinished(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData);
        void HandleScriptReloadFailed(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData);
    private:
        void Exit(void);

        FlockSDK::SharedPtr<FlockSDK::ScriptFile> scriptFile_;
};
