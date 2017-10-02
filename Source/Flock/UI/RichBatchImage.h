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

#include "../UI/RichBatch.h"
#include "../Graphics/Material.h"

namespace FlockSDK {
/// A mesh that displays image quads.
class RichWidgetImage : public RichWidgetBatch
{
    FLOCKSDK_OBJECT(RichWidgetImage, RichWidgetBatch)
public:
    /// Register object factory.
    static void RegisterObject(Context* context);

    RichWidgetImage(Context* context);
    virtual ~RichWidgetImage();

    /// Set image source url.
    void SetImageSource(const String &sourceUrl);
    /// Get image source url.
    String GetImageSource() const { return source_url_; }
    /// Add an image quad.
    void AddImage(const Vector3 pos, float width, float height);
    /// Get image width.
    int GetImageWidth() const { return texture_ != nullptr ? texture_->GetWidth() : 0; }
    /// Get image height.
    int GetImageHeight() const { return texture_ != nullptr ? texture_->GetHeight() : 0; }
    /// Get aspect ratio (width/height).
    float GetImageAspect() const { return (texture_ != nullptr) && (texture_->GetHeight() > 0) ? (float) texture_->GetWidth() / texture_->GetHeight() : 0.0f; }
private:
    /// Stored url of the image.
    String source_url_;
};

} // namespace FlockSDK
