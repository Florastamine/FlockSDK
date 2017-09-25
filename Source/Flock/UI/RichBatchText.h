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
#include "../UI/Font.h"

namespace FlockSDK
{

/// A mesh that displays text quads with a single font/size.
class RichWidgetText : public RichWidgetBatch
{
    FLOCKSDK_OBJECT(RichWidgetText, RichWidgetBatch);
public:
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Contructor.
    RichWidgetText(Context* context);
    /// Destructor.
    virtual ~RichWidgetText();

    /// Draw a quad.
    void DrawQuad(const Rect& vertices, float z, const Rect& texCoords, const Color& color);
    /// Draw a glyph.
    void DrawGlyph(const Rect& texCoords, float x, float y, float z, float width, float height, const Color& color);
    /// Draw a glyph, scaled depending on the bitmap font and pointsize.
    void DrawGlyphScaled(const Rect& texCoords, float x, float y, float z, float width, float height, const Vector2& scale, const Color& color);
    /// Add text.
    void AddText(const String& text, const Vector3& pos, const Color& color);
    /// Set the font.
    void SetFont(const String& fontname, int pointsize);
    /// Get the font face (only valid after SetFont).
    FontFace* GetFontFace() const { return font_face_; }
    /// Calculate text extents with the current font
    Vector2 CalculateTextExtents(const String& text);
    /// Row height
    float GetRowHeight() const;
private:
    Font* font_;
    FontFace* font_face_;
    int pointsize_;
    Vector2 bitmap_font_rescale_;
};

} // namespace FlockSDK
