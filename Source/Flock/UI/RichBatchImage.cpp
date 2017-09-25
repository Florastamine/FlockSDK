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

#include "../UI/RichBatchImage.h"
#include "../UI/RichWidget.h"
#include "../Graphics/Drawable.h"
#include "../Graphics/Texture2D.h"
#include "../Core/Context.h"

namespace FlockSDK {

// Register object factory. Drawable must be registered first.
void RichWidgetImage::RegisterObject(Context* context)
{
    context->RegisterFactory<RichWidgetImage>();
}

RichWidgetImage::RichWidgetImage(Context* context) : RichWidgetBatch(context)
{
    material_ = GetSubsystem<ResourceCache>()->GetResource<Material>("materials/SDK/RichTextImage.xml");
}

RichWidgetImage::~RichWidgetImage() {}

void RichWidgetImage::SetImageSource(const String& sourceUrl)
{
    if (!texture_ || sourceUrl != texture_->GetName())
    {
        if (context_->GetSubsystem<ResourceCache>()->Exists(sourceUrl))
            texture_ = context_->GetSubsystem<ResourceCache>()->GetResource<Texture2D>(sourceUrl);
        if (texture_ && material_)
        {
            material_->SetTexture(TU_DIFFUSE, texture_);
            if (parent_widget_)
                parent_widget_->SetFlags(WidgetFlags_All);
        }
    }
}

void RichWidgetImage::AddImage(const Vector3 pos, float width, float height)
{
    if (parent_widget_ && parent_widget_->GetShadowEnabled())
    {
        AddQuad(
          Rect(pos.x_ + parent_widget_->GetShadowOffset().x_, 
              pos.y_ + parent_widget_->GetShadowOffset().y_, 
              pos.x_ + parent_widget_->GetShadowOffset().x_ + width,
              pos.y_ + parent_widget_->GetShadowOffset().y_ + height), 
            pos.z_ + 0.01f,
            Rect(0, 0, 0, 0), // NOTE: UV is empty
            parent_widget_->GetShadowColor());
    }
    AddQuad(Rect(pos.x_, pos.y_, pos.x_ + width, pos.y_ + height), pos.z_, Rect(0.0f, 0.0f, 1.0f, 1.0f), Color::WHITE);
}

} // namespace FlockSDK

