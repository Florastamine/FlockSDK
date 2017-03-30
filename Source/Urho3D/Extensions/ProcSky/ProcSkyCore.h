//  @class ProcSky
//  @brief Procedural Sky component for Urho3D
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
// -- Added a bunch of getters/setters for exposing customizable parameters to the editor. 
// -- Fixed coding convention. 

#pragma once

#include "../../Precompiled.h"

#include "../../Scene/Component.h"
#include "../../Graphics/Camera.h" 
#include "../../Scene/Node.h"

#include "../../Graphics/RenderPath.h" 
#include "../../Graphics/Viewport.h" 
#include "../../Container/Vector.h" 
#include "../../Math/Matrix3.h" 
#include "../../Math/Vector3.h"

namespace Urho3D { 
class Skybox;
class StringHash;

class URHO3D_API ProcSky : public Component {
  URHO3D_OBJECT(ProcSky, Component);

public:
  ProcSky(Context* context);
  virtual ~ProcSky();

  static void RegisterObject(Context* context);

  void OnNodeSet(Node *node);
  
  /// Initialize objects and subscribe to update events.
  bool Initialize();
  /// Queue render of next frame.
  void Update();

  inline bool GetUpdateAuto() const { return updateAuto_; }
  /// Automatic update renders according to update interval. If Manual, user calls Update() to render.
  void SetUpdateAuto(bool updateAuto);

  inline float GetUpdateInterval() const { return updateInterval_; }
  /// Set the rendering interval (default 0).
  void SetUpdateInterval(float interval) { updateInterval_ = interval; }

  inline float GetUpdateWait() const { return updateWait_; }
  inline void SetUpdateWait(float f) { updateWait_ = f; }

  inline unsigned GetRenderSize() const { return renderSize_; } 
  bool SetRenderSize(unsigned size);
  void SetRenderSize2(unsigned size); 

  inline float GetRayleighBrightness() const { return rayleighBrightness_; }
  inline void SetRayleighBrightness(float f) { rayleighBrightness_ = f; } 

  inline Vector3 GetAirAbsorptionProfile() const { return Kr_; }
  inline void SetAirAbsorptionProfile(const Vector3 &v) { Kr_.x_ = v.x_; Kr_.y_ = v.y_; Kr_.z_ = v.z_; } 

  inline float GetMieBrightness() const  { return mieBrightness_; }
  inline void SetMieBrightness(float f) { mieBrightness_ = f; } 

  inline float GetSpotBrightness() const { return spotBrightness_; }
  inline void SetSpotBrightness(float f) { spotBrightness_ = f; } 

  inline float GetScatteringStrength() const  { return scatterStrength_; }
  inline void SetScatteringStrength(float f) { scatterStrength_ = f; } 

  inline float GetRayleighStrength() const { return rayleighStrength_; }
  inline void SetRayleighStrength(float f) { rayleighStrength_ = f; } 

  inline float GetMieStrength() const { return mieStrength_; }
  inline void SetMieStrength(float f) { mieStrength_ = f; } 

  inline float GetRayleighCollectionPower() const { return rayleighCollectionPower_; }
  inline void SetRayleighCollectionPower(float f) { rayleighCollectionPower_ = f; } 

  inline float GetMieCollectionPower() const { return mieCollectionPower_; }
  inline void SetMieCollectionPower(float f) { mieCollectionPower_ = f; } 

  inline float GetMieDistribution() const { return mieDistribution_; }
  inline void SetMieDistribution(float f) { mieDistribution_ = f; } 

protected:
  void HandleUpdate(StringHash eventType, VariantMap& eventData);
  /// Set rendering of next frame active/inactive.
  void SetRenderQueued(bool enable);
  void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);

protected:
  /// Urho3D Skybox with geometry and main TextureCube.
  SharedPtr<Skybox> skybox_;
  /// Node used for light direction.
  WeakPtr<Node> lightNode_;
  SharedPtr<RenderPath> rPath_;
  /// Render size of each face.
  unsigned renderSize_;
  /// Fixed rotations for each cube face.
  Matrix3 faceRotations_[MAX_CUBEMAP_FACES];

  bool updateAuto_;
  float updateInterval_;
  float updateWait_;
  bool renderQueued_;
public:
  /// Atmospheric parameters.
  Vector3 Kr_; // Absorption profile of air.
  float rayleighBrightness_;
  float mieBrightness_;
  float spotBrightness_;
  float scatterStrength_;
  float rayleighStrength_;
  float mieStrength_;
  float rayleighCollectionPower_;
  float mieCollectionPower_;
  float mieDistribution_;
}; 

}
