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

#include "../UI/RichMarkupParser.h"
#include "../Core/StringUtils.h"

struct ColorMap {
    const char *name;
    const FlockSDK::Color color;
};

static FlockSDK::Color ParseHTMLColor(const FlockSDK::String &str)
{
    static const ColorMap color_table[] = {
        { "white", FlockSDK::Color::WHITE },
        { "gray", FlockSDK::Color::GRAY },
        { "black", FlockSDK::Color::BLACK },
        { "red", FlockSDK::Color::RED },
        { "green", FlockSDK::Color::GREEN },
        { "blue", FlockSDK::Color::BLUE },
        { "cyan", FlockSDK::Color::CYAN },
        { "magenta", FlockSDK::Color::MAGENTA },
        { "yellow", FlockSDK::Color::YELLOW },
        { "gray_dark", FlockSDK::Color::GRAY_DARK },
        { "violet", FlockSDK::Color::VIOLET },
        { "lime", FlockSDK::Color::LIME },
        { "lime_arctic", FlockSDK::Color::LIME_ARCTIC },
        { "cherry", FlockSDK::Color::CHERRY },
        { "indigo", FlockSDK::Color::INDIGO },
        { "crimson", FlockSDK::Color::CRIMSON },
        { "lavender", FlockSDK::Color::LAVENDER },
        { "lemon_bitter", FlockSDK::Color::LEMON_BITTER },
        { "lilac_pale", FlockSDK::Color::LILAC_PALE },
        { "teal", FlockSDK::Color::TEAL },
        { "orchid", FlockSDK::Color::ORCHID },
        { "green_apple", FlockSDK::Color::GREEN_APPLE },
        { "aquamarine", FlockSDK::Color::AQUAMARINE },
        { "bleu_de_france", FlockSDK::Color::BLEU_DE_FRANCE },
        { "scarlet", FlockSDK::Color::SCARLET },
        { "sand", FlockSDK::Color::SAND },
        { "peach", FlockSDK::Color::PEACH },
        { "pear", FlockSDK::Color::PEAR },
        { "ivory", FlockSDK::Color::IVORY },
        { NULL, FlockSDK::Color() },
    };

    unsigned char R = 0, G = 0, B = 0, A = 0;
    FlockSDK::String color = str.ToLower();

    for (int i = 0; color_table[i].name; i++)
    {
        const ColorMap* entry = &color_table[i];
        if (color.Compare(entry->name) == 0)
            return entry->color;
    }

    if (color.Length() == 7 || color.Length() == 9)
    {
        if (color.Find("#") == 0)
        {
            R = (unsigned char) std::strtoul(str.Substring(1, 2).CString(), 0, 16);
            G = (unsigned char) std::strtoul(str.Substring(3, 2).CString(), 0, 16);
            B = (unsigned char) std::strtoul(str.Substring(5, 2).CString(), 0, 16);
            A = str.Length() == 7 ? 255 : (unsigned char) std::strtoul(str.Substring(7, 2).CString(), 0, 16);
        }
    }

    return FlockSDK::Color(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
}

namespace FlockSDK {
/// Supported tags:
///  <br> - line break
///  <b></b> - bold
///  <i></i> - italic
///  <color=#FF800000></color> - change text color, supports 24/32 bit hex and color names
///  <size=14></size> - change text size in pixels
///  <font=fonts/SDK/Annonymous Pro.ttf></font> - change text font
///  <img src=image.png width=320 height=240 /> - embed an image
///  TODO: <quad material=material.xml width=10 height=10 x=10 y=10 />
void ParseRichTextHTML(const String &text, Vector<TextBlock>& blocks, const FontState& default_font_state)
{
    unsigned pos = 0, last_tag_end = 0;
    unsigned tag_begin = 0, tag_end = 0;
    bool closing_tag = false;
    Vector<TextBlock> stack;
    TextBlock* current_block = 0;
    String tag;

    // Copy everything up to the first found tag
    pos = text.Find('<', pos, false);
    if (pos != String::NPOS && pos != 0)
    {
        TextBlock block;
        block.font = default_font_state;
        block.text = text.Substring(0, pos);
        blocks.Push(block);
        last_tag_end = pos;
    }

    while ((pos = text.Find('<', pos, false)) != String::NPOS)
    {
        tag_end = text.Find('>', pos + 1, false);
        // skip <>
        if (tag_end == pos + 1)
        {
            pos += 1;
            continue;
        }

        // stop if there's no closing tag or text ends
        if (pos + 1 >= text.Length() || tag_end == String::NPOS)
            break;

        tag_begin = pos;
        closing_tag = text[pos + 1] == '/';
        pos += closing_tag ? 2 : 1;
        tag = text.Substring(pos, tag_end - pos);

        TextBlock block;

        if (!closing_tag)
        {
            block.text = text.Substring(last_tag_end, tag_begin - last_tag_end);
            bool is_br_tag = tag == "br" || tag == "break";
            bool is_img_tag = tag.StartsWith("img") || tag.StartsWith("image");
            bool is_b_tag = tag == "b";
            bool is_i_tag = tag == "i";
            bool is_color_tag = tag.StartsWith("color");
            bool is_size_tag = tag.StartsWith("size");
            bool is_font_tag = tag.StartsWith("font");
            // bool is_quad_tag = tag.StartsWith("quad");

            if (is_br_tag)
            {
                // Append current text as a block
                if (current_block)
                    block.font = current_block->font;
                
                blocks.Push(block);
                // Append line break tag
                block.is_line_brake = true;
                block.text.Clear();
                blocks.Push(block);
            } 
            else if (is_img_tag)
            {
                // Append current text as a block
                if (current_block)
                    block.font = current_block->font;
                
                blocks.Push(block);

                block.is_iconic = true;
                closing_tag = true;
                auto img_tokens = tag.Split(' ');
                for (auto &t : img_tokens) {
                    auto tokens = t.Split('=');
                    if (tokens.Size() != 2)
                      continue;
                    
                    const String &name = tokens[0];
                    const String &value = tokens[1];
                    if (name == "width")
                        block.image_width = ToFloat(value);
                    else if (name == "height")
                        block.image_height = ToFloat(value);
                    else if (name == "src")
                        block.text = value;
                    
                }
                // Append image tag
                blocks.Push(block);
            }
            else if (is_b_tag)
                block.font.bold = true;
            else if (is_i_tag)
                block.font.italic = true;
            else if (is_color_tag || is_size_tag)
            {
                auto tokens = tag.Split('=');
                if (tokens.Size() >= 2)
                {
                    if (is_color_tag)
                        block.font.color = ParseHTMLColor(tokens[1]);
                    else if (is_size_tag)
                        block.font.size = ToInt(tokens[1]);
                }
                  
            }
            else if (is_font_tag)
            {
                auto tokens = tag.Split('=');
                if (tokens.Size() == 2)
                    block.font.face = tokens[1];
            }
            /*
            else if (is_quad_tag)
            {
                // Append current text as a block
                if (current_block)
                    block.font = current_block->font;
                
                blocks.Push(block);

                block.is_iconic = true;
                closing_tag = true;
                auto img_tokens = tag.Split(' ');
                for (auto &t : img_tokens)
                {
                    auto tokens = t.Split('=');
                    if (tokens.Size() != 2)
                        continue;
                    const String &name = tokens[0];
                    const String &value = tokens[1];
                    if (name == "width")
                        block.image_width = ToFloat(value);
                    else if (name == "height")
                        block.image_height = ToFloat(value);
                    else if (name == "material")
                        block.text = value;
                }
            }
            */
        }
        else if (current_block)
            current_block->text = text.Substring(last_tag_end, tag_begin - last_tag_end);

        // On every tag found, push the current block to the list of blocks
        if (current_block && !block.is_line_brake)
        {
            if (!closing_tag)
                current_block->text = block.text;
            if (!current_block->text.Empty())
                blocks.Push(*current_block);
        }

        if (closing_tag)
        {
            // On closing tag, pop one block from the stack
            if (!stack.Empty())
                stack.Pop();
        } 
        else
        {
            // On opening tag, push the current block in the stack
            if (!block.is_line_brake)
                stack.Push(block);
        }

        pos = tag_end + 1;
        last_tag_end = tag_end + 1;
        current_block = !stack.Empty() ? &stack[stack.Size() - 1] : 0;
    }

    // copy anything after the last tag
    if (last_tag_end < text.Length())
    {
        TextBlock block;
        block.font = default_font_state;
        block.text = text.Substring(last_tag_end, text.Length() - last_tag_end);
        blocks.Push(block);
    }

    // append blocks that are not closed properly
    if (!closing_tag && current_block && !current_block->text.Empty())
        blocks.Push(*current_block);
}

} // namespace FlockSDK
