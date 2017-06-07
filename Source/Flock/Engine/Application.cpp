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

#include "../Precompiled.h"

#include "../Engine/Application.h"
#include "../Core/Platform.h" 
#include "../IO/IOEvents.h"
#include "../IO/Log.h"

namespace FlockSDK
{
Application::Application(Context* context) :
    Object(context),
    exitCode_(EXIT_SUCCESS)
{
    engineParameters_ = Engine::ParseParameters(FlockSDK::GetArguments());

    // Create the Engine, but do not initialize it yet. Subsystems except Graphics & Renderer are registered at this point
    engine_ = new Engine(context);

    // Subscribe to log messages so that can show errors if ErrorExit() is called with empty message
    SubscribeToEvent(E_LOGMESSAGE, FLOCKSDK_HANDLER(Application, HandleLogMessage));
}

int Application::Run()
{
    // Emscripten-specific: C++ exceptions are turned off by default in -O1 (and above), unless '-s DISABLE_EXCEPTION_CATCHING=0' flag is set
    // Flock build configuration uses -O3 (Release), -O2 (RelWithDebInfo), and -O0 (Debug)
    // Thus, the try-catch block below should be optimised out except in Debug build configuration
    try
    {
        Setup();
        if (exitCode_)
            return exitCode_;

        if (!engine_->Initialize(engineParameters_))
        {
            ErrorExit();
            return exitCode_;
        }

        Start();
        if (exitCode_)
            return exitCode_;
        
        while (!engine_->IsExiting())
            engine_->RunFrame();

        Stop();

        return exitCode_;
    }
    catch (std::bad_alloc&)
    {
        ErrorDialog(GetTypeName(), "An out-of-memory error occurred. The application will now exit.");
        return EXIT_FAILURE;
    }
}

void Application::ErrorExit(const String &message)
{
    engine_->Exit(); // Close the rendering window
    exitCode_ = EXIT_FAILURE;

    if (!message.Length())
    {
        ErrorDialog(GetTypeName(), startupErrors_.Length() ? startupErrors_ :
            "Application has been terminated due to unexpected error.");
    }
    else
        ErrorDialog(GetTypeName(), message);
}

void Application::HandleLogMessage(StringHash eventType, VariantMap& eventData)
{
    using namespace LogMessage;

    if (eventData[P_LEVEL].GetInt() == LOG_ERROR)
    {
        // Strip the timestamp if necessary
        String error = eventData[P_MESSAGE].GetString();
        unsigned bracketPos = error.Find(']');
        if (bracketPos != String::NPOS)
            error = error.Substring(bracketPos + 2);

        startupErrors_ += error + "\n";
    }
}


}
