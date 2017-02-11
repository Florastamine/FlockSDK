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

#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>

#ifdef URHO3D_LUA
#include <Urho3D/LuaScript/LuaScript.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <Urho3D/DebugNew.h>

using namespace Urho3D;

static void CompileScript(Context* context, const String& fileName)
{
    PrintLine("Compiling script file " + fileName);

    File inFile(context, fileName, FILE_READ);
    if (!inFile.IsOpen())
        ErrorExit("Failed to open script file " + fileName);

    ScriptFile script(context);
    if (!script.Load(inFile))
        ErrorExit();

    String outFileName = ReplaceExtension(fileName, ".asc");
    File outFile(context, outFileName, FILE_WRITE);
    if (!outFile.IsOpen())
        ErrorExit("Failed to open output file " + fileName);

    script.SaveByteCode(outFile);
}

int main(int argc, char** argv)
{
    #ifdef WIN32
    const Vector<String>& arguments = ParseArguments(GetCommandLineW());
    #else
    const Vector<String>& arguments = ParseArguments(argc, argv);
    #endif

    String sourceTree;
    String outputFile;

    if (arguments.Size() < 1)
        ErrorExit("Usage: ScriptCompiler <input file> [resource path for includes]\n");
    else 
        outputFile = arguments[0];

    SharedPtr<Context> context(new Context());
    context->RegisterSubsystem(new Script(context));

    Log* log = context->GetSubsystem<Log>();
    // Register Log subsystem manually if compiled without logging support
    if (!log)
    {
        context->RegisterSubsystem(new Log(context));
        log = context->GetSubsystem<Log>();
    }

    log->SetLevel(LOG_WARNING);
    log->SetTimeStamp(false);

    String path, file, extension;
    SplitPath(outputFile, path, file, extension);

    ResourceCache* cache = context->GetSubsystem<ResourceCache>();

    // Add resource path to be able to resolve includes
    if (arguments.Size() > 1)
        cache->AddResourceDir(arguments[1]);
    else
        cache->AddResourceDir(cache->GetPreferredResourceDir(path));

    if (!file.StartsWith("*"))
        CompileScript(context, outputFile);
    else
    {
        Vector<String> scriptFiles;
        context->GetSubsystem<FileSystem>()->ScanDir(scriptFiles, path, file + extension, SCAN_FILES, false);
        for (unsigned i = 0; i < scriptFiles.Size(); ++i)
            CompileScript(context, path + scriptFiles[i]);
    }

    return EXIT_SUCCESS;
}

