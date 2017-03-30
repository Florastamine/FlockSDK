/**
  @class ProcSky
  @brief Procedural Sky component for Urho3D
  @author carnalis <carnalis.j@gmail.com>
  @license MIT License
  @copyright
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/ 

// Contains modifications by Florastamine (git@github.com:Florastamine):
// -- Removed GetProcSky() as it is useless (ProcSky is registered as a component and not a subsystem). 
// -- Fixed coding convention. 

#include "../AngelScript/APITemplates.h"
#include "../../Extensions/ProcSky/ProcSkyCore.h" 

namespace Urho3D
{
void RegisterProcSkyAPI(asIScriptEngine* engine)
{
  RegisterComponent<ProcSky>(engine, "ProcSky");

  engine->RegisterObjectMethod("ProcSky", "void SetUpdateAuto(const bool)", asMETHOD(ProcSky, SetUpdateAuto), asCALL_THISCALL);
  engine->RegisterObjectMethod("ProcSky", "bool GetUpdateAuto() const", asMETHOD(ProcSky, GetUpdateAuto), asCALL_THISCALL);
  engine->RegisterObjectMethod("ProcSky", "void SetUpdateInterval(const bool)", asMETHOD(ProcSky, SetUpdateInterval), asCALL_THISCALL);
  engine->RegisterObjectMethod("ProcSky", "float GetUpdateInterval() const", asMETHOD(ProcSky, GetUpdateInterval), asCALL_THISCALL);
  engine->RegisterObjectMethod("ProcSky", "bool SetRenderSize(uint)", asMETHOD(ProcSky, SetRenderSize), asCALL_THISCALL);
  engine->RegisterObjectMethod("ProcSky", "uint GetRenderSize() const", asMETHOD(ProcSky, GetRenderSize), asCALL_THISCALL);
  engine->RegisterObjectMethod("ProcSky", "bool Initialize()", asMETHOD(ProcSky, Initialize), asCALL_THISCALL);
  engine->RegisterObjectMethod("ProcSky", "void Update()", asMETHOD(ProcSky, Update), asCALL_THISCALL);
}
}
