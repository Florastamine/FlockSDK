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

#include "../UI/RichWidget.h"

namespace FlockSDK {

/// Ticker type
enum TickerType
{
    TickerType_None,
    TickerType_Horizontal,
    TickerType_Vertical,
};

/// Ticker direction
enum TickerDirection
{
    TickerDirection_Negative,
    TickerDirection_Positive,
};

/// Text wrapping
enum TextWrapping
{
    WRAP_NONE,
    WRAP_WORD
};

/// Font description of a text block
struct FontState
{
    String face;
    unsigned size;
    Color color;
    bool bold;
    bool italic;
    bool underlined;
    bool striked;
    bool superscript;
    bool subscript;

    FontState()
     : size(0),
       bold(false),
       italic(false),
       underlined(false),
       striked(false),
       superscript(false),
       subscript(false) {};
};

/// A block of text or an image
struct TextBlock
{
    /// Text or image/material source
    String text; 
    /// Font description of this block (if this is a text block)
    FontState font;

    float image_width;
    float image_height;
    bool is_iconic;
    bool is_visible;
    bool is_line_brake;

    void Reset() {
      image_width = 0.0f;
      image_height = 0.0f;
      is_iconic = false;
      is_visible = true;
      is_line_brake = false;
    }

    TextBlock()
     : image_width(0.0f)
     , image_height(0.0f)
     , is_iconic(false)
     , is_visible(true)
     , is_line_brake(false) {}
};

/// A line inside the text layout
struct TextLine
{
    TextLine()
     : width(0)
     , height(0)
      , offset_x(0)
      , offset_y(0)
      , align(HA_LEFT) {}

    int width;
    int height;

    int offset_x;
    int offset_y;

    HorizontalAlignment	align;
    Vector<TextBlock> blocks;
};

/// RichText3D
class RichText3D : public RichWidget
{
    FLOCKSDK_OBJECT(RichText3D, RichWidget);
public:
    /// Register object factory. Drawable must be registered first.
    static void RegisterObject(Context* context);

    /// Contructor.
    RichText3D(Context* context);
    /// Destructor.
    virtual ~RichText3D();

    /// Set display text inside the view.
    void SetText(const String &text);
    /// Get currently displayed text (as markup).
    const String &GetText() const;
    /// Set the default font for blocks without formatting.
    void SetDefaultFont(const String &face, unsigned size);
    /// Get default font name
    String GetDefaultFontName() const { return defaultFontState_.face; }
    /// Get default font size
    unsigned GetDefaultFontSize() const { return defaultFontState_.size; }
    /// Set text color.
    void SetTextColor(const Color &color);
    /// Get text color.
    Color GetTextColor() const { return defaultFontState_.color; }
    /// Set text alignment.
    void SetAlignment(HorizontalAlignment align);
    /// Get text alignment.
    HorizontalAlignment GetAlignment() const { return alignment_; }
    /// Set additional line spacing (can be negative).
    void SetLineSpacing(int lineSpacing);
    /// Get additional line spacing.
    int GetLineSpacing() const { return lineSpacing_; }
    /// Set word wrapping.
    void SetWrapping(bool wrapping);
    /// Get wrapping.
    bool GetWrapping() const { return wrapping_ == WRAP_WORD; }
    /// Set ticker type.
    void SetTickerType(TickerType type);
    /// Get ticker type.
    TickerType GetTickerType() const;
    /// Set ticker scroll direction.
    void SetTickerDirection(TickerDirection direction);
    /// Get ticker scroll direction.
    TickerDirection GetTickerDirection() const;
    /// Set ticker scroll speed.
    void SetTickerSpeed(float pixelspersecond);
    /// Get ticker scroll speed.
    float GetTickerSpeed() const;
    /// Set single line.
    void SetSingleLine(bool singleLine);
    /// Get single line.
    bool GetSingleLine() const { return singleLine_; }
    /// Reset the ticker to the beginning.
    void ResetTicker();
    /// Set ticker position (0-1 range).
    void SetTickerPosition(float tickerPosition);
    /// Get ticker position (0-1 range).
    float GetTickerPosition() const;
    /// Get refresh count - number of times the view is recompiled.
    unsigned GetRefreshCount() const { return refreshCount_; }
    /// Get font ResourceRef.
    ResourceRef GetFontAttr() const;
    /// Set font ResourceRef.
    void SetFontAttr(const ResourceRef& value);
    /// Get font size.
    int GetFontSizeAttr() const;
    /// Set font size.
    void SetFontSizeAttr(int size);
protected:
    /// The caption text.
    String text_;
    /// Additional line spacing (can be negative).
    int lineSpacing_;
    /// Ticker type.
    TickerType tickerType_;
    /// Ticker direction.
    TickerDirection tickerDirection_;
    /// Ticker speed.
    float tickerSpeed_;
    /// Default font state for unformatted text.
    FontState defaultFontState_;
    /// The lines of text.
    Vector<TextLine> lines_; // TODO: could be removed in the future.
    /// The scroll origin of the text (in ticker mode).
    Vector3 scrollOrigin_;
    /// Is the text single line.
    bool singleLine_;
    /// Ticker position (0-1).
    float tickerPosition_;
    /// Wrapping
    TextWrapping wrapping_;
    /// Alignment
    HorizontalAlignment alignment_;
    /// How many times this text has recompiled itself
    unsigned refreshCount_;

    /// Compile the text to render items.
    void CompileTextLayout();
    /// Arrange text blocks into the textview layout as lines.
    void ArrangeTextBlocks(Vector<TextBlock>& markup_blocks);
    /// Draw text lines to the widget.
    void DrawTextLines();

    /// Per-frame text animation.
    void UpdateTickerAnimation(FlockSDK::StringHash eventType, FlockSDK::VariantMap& eventData);
};

} // namespace FlockSDK
