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

#include "../Graphics/Drawable.h"
#include "../UI/UIBatch.h"
#include "../UI/UIElement.h"

namespace FlockSDK
{
class RichWidget;
class RichWidgetBatch;

// Quad storage.
struct Quad
{
    Quad(const Rect& vertices,
      float z,
      const Rect& texcoords,
      const Color& color)
      : vertices_(vertices)
      , z_(z)
      , tex_coords_(texcoords)
      , color_(color) {}

    Quad() : z_(0.0f) {}

    Rect vertices_;
    Rect tex_coords_;
    Color color_;
    float z_;
};

/// A batch for rendering inside a widget.
class RichWidgetBatch : public Object
{
    FLOCKSDK_OBJECT(RichWidgetBatch, Object);
public:
    explicit RichWidgetBatch(Context* context);
    virtual ~RichWidgetBatch();

    /// The texture used on the quads.
    Texture* texture_;
    /// The material used to render.
    SharedPtr<Material> material_;
    /// The output batch.
    UIBatch* batch_;

    /// Set parent widget.
    void SetParentWidget(RichWidget* parent);
    /// Get parent widget.
    RichWidget* GetParentWidget() const { return parent_widget_; }
    /// Does the WidgetBatch need redrawing ?
    bool IsDirty() const { return is_dirty_; }
    /// Force the WidgetBatch to redraw.
    void SetDirty() { is_dirty_ = true; }
    /// Add a quad.
    void AddQuad(const Rect& vertices, float z, const Rect& texcoords, const Color& color);
    /// Remove all quads.
    void ClearQuads();
    /// Is the render item empty (has no quads)?
    bool IsEmpty() const { return quads_.Empty(); }
    /// Get UI batches from this widget.
    virtual void GetBatches(PODVector<UIBatch>& batches, PODVector<float>& vertexData, const IntRect& currentScissor);
protected:
    friend class RichWidget;
    /// Used in RichWidget for caching.
    StringHash id_;
    // Flags if the batch has changed since last draw/
    bool is_dirty_;
    /// List of quads.
    PODVector<Quad> quads_;
    /// The parent widget (if any).
    RichWidget* parent_widget_;
    /// Use count in the last draw call.
    int use_count_;
    /// number of batches in the last GetBatches call.
    int num_batches_;
};

} // namespace FlockSDK
