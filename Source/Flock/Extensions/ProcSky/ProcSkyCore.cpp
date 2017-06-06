//  @class ProcSky
//  @brief Procedural Sky component for Flock
//  @author carnalis <carnalis.j@gmail.com>
//  @license MIT License
//  @copyright
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

// Contains modifications by Florastamine (git@github.com:Florastamine):
// -- Changed GetProjection(false) to GetProjection(). 
// -- Created a separate component group for ProcSky in the editor, and also exposed various properties to the ProcSky component inside the editor. 
// -- Removed default camera creation. 
// -- Removed PROCSKY_UI and PROCSKY_TEXTURE_DUMPING as hard-coding debugging utilities into the codebase along with fixed keysets are not necessarily a good idea.
// -- Fixed coding convention. 

#include "ProcSkyCore.h"  
#include "../../Core/Context.h"
#include "../../Core/CoreEvents.h"
#include "../../Core/Variant.h"
#include "../../Graphics/Camera.h"
#include "../../Graphics/Graphics.h"
#include "../../Graphics/GraphicsDefs.h"
#include "../../Graphics/Material.h"
#include "../../Graphics/Model.h"
#include "../../Graphics/Renderer.h"
#include "../../Graphics/RenderPath.h"
#include "../../Graphics/Skybox.h"
#include "../../Graphics/Technique.h"
#include "../../Graphics/Texture.h"
#include "../../Graphics/TextureCube.h"
#include "../../Graphics/View.h"
#include "../../Graphics/Viewport.h"
#include "../../Input/Input.h"
#include "../../Input/InputEvents.h"
#include "../../Math/Matrix4.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "../../Resource/ResourceCache.h"
#include "../../IO/Log.h"
#include "../../Scene/Scene.h"

namespace FlockSDK { 

ProcSky::ProcSky(Context* context):
  Component(context)
  , renderSize_(256)
  , renderFOV_(89.5f)
  , updateAuto_(true)
  , updateInterval_(0.0f)
  , updateWait_(0)
  , renderQueued_(true)
  , cam_(NULL)
  , Kr_(Vector3(0.18867780436772762, 0.4978442963618773, 0.6616065586417131)) 
  , rayleighBrightness_(3.3f)
  , mieBrightness_(0.1f)
  , spotBrightness_(50.0f)
  , scatterStrength_(0.028f)
  , rayleighStrength_(0.139f)
  , mieStrength_(0.264f)
  , rayleighCollectionPower_(0.81f)
  , mieCollectionPower_(0.39f)
  , mieDistribution_(0.63f)
{
  faceRotations_[FACE_POSITIVE_X] = Matrix3(0,0,1,  0,1,0, -1,0,0);
  faceRotations_[FACE_NEGATIVE_X] = Matrix3(0,0,-1, 0,1,0,  1,0,0);
  faceRotations_[FACE_POSITIVE_Y] = Matrix3(1,0,0,  0,0,1,  0,-1,0);
  faceRotations_[FACE_NEGATIVE_Y] = Matrix3(1,0,0,  0,0,-1, 0,1,0);
  faceRotations_[FACE_POSITIVE_Z] = Matrix3(1,0,0,  0,1,0,  0,0,1);
  faceRotations_[FACE_NEGATIVE_Z] = Matrix3(-1,0,0, 0,1,0,  0,0,-1);
}

ProcSky::~ProcSky() {}

void ProcSky::RegisterObject(Context* context) {
  context->RegisterFactory<ProcSky>("ProcSky");

  FLOCKSDK_ACCESSOR_ATTRIBUTE("Render size", GetRenderSize, SetRenderSize2, unsigned, 256, AM_DEFAULT);

  FLOCKSDK_ACCESSOR_ATTRIBUTE("Enable auto-update", GetUpdateAuto, SetUpdateAuto, bool, true, AM_DEFAULT);
  FLOCKSDK_ACCESSOR_ATTRIBUTE("Auto-update wait time", GetUpdateWait, SetUpdateWait, float, 0.0f, AM_DEFAULT);
  FLOCKSDK_ACCESSOR_ATTRIBUTE("Auto-update interval", GetUpdateInterval, SetUpdateInterval, float, 0.0f, AM_DEFAULT); 

  FLOCKSDK_ACCESSOR_ATTRIBUTE("Scattering strength", GetScatteringStrength, SetScatteringStrength, float, 0.028f, AM_DEFAULT); 
  FLOCKSDK_MIXED_ACCESSOR_ATTRIBUTE("Air absorption profile", GetAirAbsorptionProfile, SetAirAbsorptionProfile, Vector3, Vector3(0.18867780436772762, 0.4978442963618773, 0.6616065586417131), AM_DEFAULT); 

  FLOCKSDK_ACCESSOR_ATTRIBUTE("Rayleigh strength", GetRayleighStrength, SetRayleighStrength, float, 0.139f, AM_DEFAULT); 
  FLOCKSDK_ACCESSOR_ATTRIBUTE("Rayleigh brightness", GetRayleighBrightness, SetRayleighBrightness, float, 3.3f, AM_DEFAULT); 
  FLOCKSDK_ACCESSOR_ATTRIBUTE("Rayleigh collection power", GetRayleighCollectionPower, SetRayleighCollectionPower, float, 0.81f, AM_DEFAULT);

  FLOCKSDK_ACCESSOR_ATTRIBUTE("Mie strength", GetMieStrength, SetMieStrength, float, 0.264f, AM_DEFAULT);
  FLOCKSDK_ACCESSOR_ATTRIBUTE("Mie brightness", GetMieBrightness, SetMieBrightness, float, 0.1f, AM_DEFAULT); 
  FLOCKSDK_ACCESSOR_ATTRIBUTE("Mie collection power", GetMieCollectionPower, SetMieCollectionPower, float, 0.39f, AM_DEFAULT); 
  FLOCKSDK_ACCESSOR_ATTRIBUTE("Mie distribution", GetMieDistribution, SetMieDistribution, float, 0.63f, AM_DEFAULT);

  FLOCKSDK_ACCESSOR_ATTRIBUTE("Spot brightness", GetSpotBrightness, SetSpotBrightness, float, 50.0f, AM_DEFAULT);
}

void ProcSky::OnNodeSet(Node* node) {
  if (!node) return;
}

bool ProcSky::Initialize() {
  FLOCKSDK_LOGDEBUG("ProcSky::Initialize()");
  ResourceCache* cache = GetSubsystem<ResourceCache>();
  Renderer* renderer = GetSubsystem<Renderer>();
  rPath_ = renderer->GetViewport(0)->GetRenderPath(); 

  if (!cam_) { 
    cam_ = node_->CreateComponent<Camera>();
    cam_->SetFov(renderFOV_);
    cam_->SetAspectRatio(1.0f);
    cam_->SetNearClip(1.0f);
    cam_->SetFarClip(100.0f);
  }

  // Use first child as light node if it exists; otherwise, create it.
  if (!lightNode_) {
    const Vector<SharedPtr<Node> >& children = node_->GetChildren();
    if (children.Size()) {
      lightNode_ = children[0];
    }
    if (!lightNode_) {
      FLOCKSDK_LOGDEBUG("ProcSky::Initialize: Creating node 'ProcSkyLight' with directional light.");
      lightNode_ = node_->CreateChild("ProcSkyLight");
      Light* light(lightNode_->CreateComponent<Light>());
      light->SetLightType(LIGHT_DIRECTIONAL);
      Color lightColor;
      lightColor.FromHSV(57.0f, 9.9f, 75.3f);
      light->SetColor(lightColor);
    }
  }

  // Create a Skybox to draw to. Set its Material, Technique, and render size.
  skybox_ = node_->CreateComponent<Skybox>();
  Model* model(cache->GetResource<Model>("objects/Box.mdl"));
  skybox_->SetModel(model);
  SharedPtr<Material> skyboxMat(new Material(context_));
  skyboxMat->SetTechnique(0, cache->GetResource<Technique>("shaders/techniques/DiffSkybox.xml"));
  skyboxMat->SetCullMode(CULL_NONE);
  skybox_->SetMaterial(skyboxMat);
  SetRenderSize(renderSize_);

  // Shove some of the shader parameters into a VariantMap.
  VariantMap atmoParams;
  atmoParams["Kr"] = Kr_;
  atmoParams["RayleighBrightness"] = rayleighBrightness_;
  atmoParams["MieBrightness"] = mieBrightness_;
  atmoParams["SpotBrightness"] = spotBrightness_;
  atmoParams["ScatterStrength"] = scatterStrength_;
  atmoParams["RayleighStrength"] = rayleighStrength_;
  atmoParams["MieStrength"] = mieStrength_;
  atmoParams["RayleighCollectionPower"] = rayleighCollectionPower_;
  atmoParams["MieCollectionPower"] = mieCollectionPower_;
  atmoParams["MieDistribution"] = mieDistribution_;
  atmoParams["LightDir"] = Vector3::DOWN;
  atmoParams["InvProj"] = cam_->GetProjection().Inverse();

  // Add custom quad commands to render path.
  for (auto i = 0u; i < MAX_CUBEMAP_FACES; ++i) {
    RenderPathCommand cmd;
    cmd.tag_ = "ProcSky";
    cmd.type_ = CMD_QUAD;
    cmd.sortMode_ = SORT_BACKTOFRONT;
    cmd.pass_ = "base";
    cmd.outputs_.Push(MakePair(String("DiffProcSky"), (CubeMapFace)i));
    cmd.textureNames_[0] = "";
    cmd.vertexShaderName_ = "ProcSky";
    cmd.vertexShaderDefines_ = "";
    cmd.pixelShaderName_ = "ProcSky";
    cmd.pixelShaderDefines_ = "";
    cmd.shaderParameters_ = atmoParams;
    cmd.shaderParameters_["InvViewRot"] = faceRotations_[i];
    cmd.enabled_ = true;
    rPath_->AddCommand(cmd);
  }

  // Perform at least one render to avoid empty sky.
  Update();

  SubscribeToEvent(E_UPDATE, FLOCKSDK_HANDLER(ProcSky, HandleUpdate));
  return true;
}

void ProcSky::HandleUpdate(StringHash eventType, VariantMap& eventData) {
  if (updateAuto_) {
    float dt(eventData[Update::P_TIMESTEP].GetFloat());
    // If using an interval, queue update when done waiting.
    if (updateInterval_ > 0) {
      updateWait_ -= dt;
      if (updateWait_ <= 0) {
        updateWait_ = updateInterval_;
        Update();
      }
    } else { // No interval; just update.
      Update();
    }
  }
}

// Update shader parameters and queue a render.
void ProcSky::Update() {
  if (lightNode_) {
    // In the shader code, LightDir is the direction TO the object casting light, not the direction OF the light, so invert the direction.
    Vector3 lightDir(-lightNode_->GetWorldDirection());
    rPath_->SetShaderParameter("LightDir", lightDir);
  }
  ///@TODO Need only send changed parameters.
  rPath_->SetShaderParameter("Kr", Kr_);
  rPath_->SetShaderParameter("RayleighBrightness", rayleighBrightness_);
  rPath_->SetShaderParameter("MieBrightness", mieBrightness_);
  rPath_->SetShaderParameter("SpotBrightness", spotBrightness_);
  rPath_->SetShaderParameter("ScatterStrength", scatterStrength_);
  rPath_->SetShaderParameter("RayleighStrength", rayleighStrength_);
  rPath_->SetShaderParameter("MieStrength", mieStrength_);
  rPath_->SetShaderParameter("RayleighCollectionPower", rayleighCollectionPower_);
  rPath_->SetShaderParameter("MieCollectionPower", mieCollectionPower_);
  rPath_->SetShaderParameter("MieDistribution", mieDistribution_);
  rPath_->SetShaderParameter("InvProj", cam_->GetProjection().Inverse());
  SetRenderQueued(true);
} 

void ProcSky::SetRenderSize2(unsigned size) { SetRenderSize(size); }

bool ProcSky::SetRenderSize(unsigned size) {
  if (size >= 1) {
    // Create a TextureCube and assign to the ProcSky material.
    SharedPtr<TextureCube> skyboxTexCube(new TextureCube(context_));
    skyboxTexCube->SetName("DiffProcSky");
    skyboxTexCube->SetSize(size, Graphics::GetRGBAFormat(), TEXTURE_RENDERTARGET);
    skyboxTexCube->SetFilterMode(FILTER_BILINEAR);
    skyboxTexCube->SetAddressMode(COORD_U, ADDRESS_CLAMP);
    skyboxTexCube->SetAddressMode(COORD_V, ADDRESS_CLAMP);
    skyboxTexCube->SetAddressMode(COORD_W, ADDRESS_CLAMP);
    GetSubsystem<ResourceCache>()->AddManualResource(skyboxTexCube);

    skybox_->GetMaterial()->SetTexture(TU_DIFFUSE, skyboxTexCube);
    renderSize_ = size;
    return true;
  } else {
    FLOCKSDK_LOGWARNING("ProcSky::SetSize (" + String(size) + ") ignored; requires size >= 1.");
  }
  return false;
}

void ProcSky::SetUpdateAuto(bool updateAuto) {
  if (updateAuto_ == updateAuto) return;
  updateAuto_ = updateAuto;
}

void ProcSky::SetRenderQueued(bool queued) {
  if (renderQueued_ == queued) return;
  // When using manual update, be notified after rendering.
  if (!updateAuto_)
    SubscribeToEvent(E_POSTRENDERUPDATE, FLOCKSDK_HANDLER(ProcSky, HandlePostRenderUpdate));
  rPath_->SetEnabled("ProcSky", queued);
  renderQueued_ = queued;
}

void ProcSky::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData) {
  if (!updateAuto_)
    SetRenderQueued(false);
}

}
