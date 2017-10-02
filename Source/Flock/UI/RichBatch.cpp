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

#include "../UI/RichBatch.h"
#include "../UI/RichWidget.h"

static void QuadToUIBatch(FlockSDK::UIBatch *batch, const FlockSDK::Quad &q) 
{
    unsigned color = q.color_.ToUInt();
    unsigned begin = batch->vertexData_->Size();
    batch->vertexData_->Resize(begin + 6 * FlockSDK::UI_VERTEX_SIZE);
    float* dest = &(batch->vertexData_->At(begin));
    batch->vertexEnd_ = batch->vertexData_->Size();

    dest[0] = q.vertices_.min_.x_;
    dest[1] = q.vertices_.min_.y_;
    dest[2] = q.z_;
    ((unsigned&)dest[3]) = color;
    dest[4] = q.tex_coords_.min_.x_;
    dest[5] = q.tex_coords_.min_.y_;

    dest[6] = q.vertices_.max_.x_;
    dest[7] = q.vertices_.min_.y_;
    dest[8] = q.z_;
    ((unsigned&)dest[9]) = color;
    dest[10] = q.tex_coords_.max_.x_;
    dest[11] = q.tex_coords_.min_.y_;

    dest[12] = q.vertices_.min_.x_;
    dest[13] = q.vertices_.max_.y_;
    dest[14] = q.z_;
    ((unsigned&)dest[15]) = color;
    dest[16] = q.tex_coords_.min_.x_;
    dest[17] = q.tex_coords_.max_.y_;

    dest[18] = q.vertices_.max_.x_;
    dest[19] = q.vertices_.min_.y_;
    dest[20] = q.z_;
    ((unsigned&)dest[21]) = color;
    dest[22] = q.tex_coords_.max_.x_;
    dest[23] = q.tex_coords_.min_.y_;

    dest[24] = q.vertices_.max_.x_;
    dest[25] = q.vertices_.max_.y_;
    dest[26] = q.z_;
    ((unsigned&)dest[27]) = color;
    dest[28] = q.tex_coords_.max_.x_;
    dest[29] = q.tex_coords_.max_.y_;

    dest[30] = q.vertices_.min_.x_;
    dest[31] = q.vertices_.max_.y_;
    dest[32] = q.z_;
    ((unsigned&)dest[33]) = color;
    dest[34] = q.tex_coords_.min_.x_;
    dest[35] = q.tex_coords_.max_.y_;
}

namespace FlockSDK {

RichWidgetBatch::RichWidgetBatch(Context* context)
 : is_dirty_(false)
 , use_count_(0)
 , parent_widget_(0)
 , Object(context)
 , texture_(0)
 , num_batches_(0) {}

RichWidgetBatch::~RichWidgetBatch()
{
    ClearQuads();
}

void RichWidgetBatch::SetParentWidget(RichWidget* parent)
{
    parent_widget_ = parent;
}

void RichWidgetBatch::AddQuad(const Rect &vertices, float z, const Rect &texcoords, const FlockSDK::Color &color)
{
    quads_.Push(Quad(vertices, z, texcoords, color));
    SetDirty();
}

void RichWidgetBatch::ClearQuads()
{
    quads_.Clear();
    SetDirty();
}

void RichWidgetBatch::GetBatches(PODVector<UIBatch>& batches, PODVector<float>& vertexData, const IntRect &currentScissor)
{
    // Draw all the quads to the UIBatch list
    UIBatch batch(0, BLEND_ALPHA, currentScissor, texture_ ? texture_ : 0, &vertexData);

    float scalex = parent_widget_ ? parent_widget_->GetInternalScale().x_ : 1.0f;
    float scaley = parent_widget_ ? parent_widget_->GetInternalScale().y_ : 1.0f;

    const Rect &cliprect = parent_widget_ ? parent_widget_->GetActualDrawArea(false) : Rect();
    const Rect &cliprect_with_padding = parent_widget_ ? parent_widget_->GetActualDrawArea(true) : Rect();
    Vector3 scaled_draw_origin = parent_widget_ ? parent_widget_->GetDrawOrigin() * Vector3(parent_widget_->GetInternalScale().x_, parent_widget_->GetInternalScale().y_, 1.0f) : Vector3::ZERO;
    IntRect padding = parent_widget_ ? parent_widget_->GetPadding() : IntRect::ZERO;
    padding.left_ *= scalex;
    padding.right_ *= scalex;
    padding.top_ *= scaley;
    padding.bottom_ *= scaley;
    Vector3 scale_vector(scalex, scaley, 1);

    auto MoveQuad = [] (const FlockSDK::Vector3 &vector, FlockSDK::Rect &vertices) -> void {
        vertices.min_.x_ += vector.x_;
        vertices.max_.x_ += vector.x_;
        vertices.min_.y_ += vector.y_;
        vertices.max_.y_ += vector.y_;
    };

    auto ScaleQuad = [] (const FlockSDK::Vector3 &scale, FlockSDK::Rect &vertices) -> void {
        vertices.min_.x_ *= scale.x_;
        vertices.max_.x_ *= scale.x_;
        vertices.min_.y_ *= scale.y_;
        vertices.max_.y_ *= scale.y_;
    };

    // Clips a quad to the specified Rect. Returns false if the quad is outside the rect.
    auto ClipQuad = [] (FlockSDK::Rect &vertices, FlockSDK::Rect &texcoords, const FlockSDK::Rect &clip_region) -> bool {
        float leftClip = (float) clip_region.min_.x_ - vertices.min_.x_;
        if (leftClip > 0.0f) 
        {
            if ((float) clip_region.min_.x_ < vertices.max_.x_) 
            {
                texcoords.min_.x_ += (texcoords.max_.x_ - texcoords.min_.x_) * leftClip / (vertices.max_.x_ - vertices.min_.x_);
                vertices.min_.x_ += leftClip;
            } 
            else
                return false;
        }

        float rightClip = vertices.max_.x_ - (float) clip_region.max_.x_;
        if (rightClip > 0.0f)
        {
            if (vertices.min_.x_ < (float) clip_region.max_.x_)
            {
                texcoords.max_.x_ -= (texcoords.max_.x_ - texcoords.min_.x_) * rightClip / (vertices.max_.x_ - vertices.min_.x_);
                vertices.max_.x_ -= rightClip;
            } 
            else
                return false;
        }

        float topClip = (float) clip_region.min_.y_ - vertices.min_.y_;
        if (topClip > 0.0f)
        {
            if ((float) clip_region.min_.y_ < vertices.max_.y_) 
            {
                texcoords.min_.y_ += (texcoords.max_.y_ - texcoords.min_.y_) * topClip / (vertices.max_.y_ - vertices.min_.y_);
                vertices.min_.y_ += topClip;
            }
            else
                return false;
        }

        float bottomClip = vertices.max_.y_ - (float) clip_region.max_.y_;
        if (bottomClip > 0.0f)
        {
            if (vertices.min_.y_ < (float)clip_region.max_.y_)
            {
                texcoords.max_.y_ -= (texcoords.max_.y_ - texcoords.min_.y_) * bottomClip / (vertices.max_.y_ - vertices.min_.y_);
                vertices.max_.y_ -= bottomClip;
            }
            else
                return false;
        }

        return true;
    };

    for (auto &quad : quads_)
    {
        Quad q = quad; // NOTE: uses copy constructor
        ScaleQuad(scale_vector, q.vertices_);
        MoveQuad(scaled_draw_origin, q.vertices_);
        q.vertices_.min_.x_ += padding.left_;
        q.vertices_.max_.x_ += padding.left_;
        q.vertices_.min_.y_ += padding.top_;
        q.vertices_.max_.y_ += padding.top_;
        if (!cliprect.Equals(Rect::ZERO) && !ClipQuad(q.vertices_, q.tex_coords_, cliprect_with_padding))
            continue;

        QuadToUIBatch(&batch, q);
    }

    batch.texture_ = texture_;

    int batch_count_before = batches.Size();
    UIBatch::AddOrMerge(batch, batches);

    num_batches_ = batches.Size() - batch_count_before;

    is_dirty_ = false;
}

} // namespace FlockSDK
