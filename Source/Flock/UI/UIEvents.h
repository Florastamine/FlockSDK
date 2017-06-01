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

#include "../Core/Object.h"

namespace FlockSDK
{

/// Global mouse click in the UI. Sent by the UI subsystem.
FLOCKSDK_EVENT(E_UIMOUSECLICK, UIMouseClick)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_BUTTON, Button);                // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Global mouse click end in the UI. Sent by the UI subsystem.
FLOCKSDK_EVENT(E_UIMOUSECLICKEND, UIMouseClickEnd)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_BEGINELEMENT, BeginElement);    // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_BUTTON, Button);                // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Global mouse double click in the UI. Sent by the UI subsystem.
FLOCKSDK_EVENT(E_UIMOUSEDOUBLECLICK, UIMouseDoubleClick)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_BUTTON, Button);                // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Mouse click on a UI element. Parameters are same as in UIMouseClick event, but is sent by the element.
FLOCKSDK_EVENT(E_CLICK, Click)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_BUTTON, Button);                // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Mouse click end on a UI element. Parameters are same as in UIMouseClickEnd event, but is sent by the element.
FLOCKSDK_EVENT(E_CLICKEND, ClickEnd)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_BEGINELEMENT, BeginElement);    // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_BUTTON, Button);                // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Mouse double click on a UI element. Parameters are same as in UIMouseDoubleClick event, but is sent by the element.
FLOCKSDK_EVENT(E_DOUBLECLICK, DoubleClick)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_BUTTON, Button);                // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Drag and drop test.
FLOCKSDK_EVENT(E_DRAGDROPTEST, DragDropTest)
{
    FLOCKSDK_PARAM(P_SOURCE, Source);                // UIElement pointer
    FLOCKSDK_PARAM(P_TARGET, Target);                // UIElement pointer
    FLOCKSDK_PARAM(P_ACCEPT, Accept);                // bool
};

/// Drag and drop finish.
FLOCKSDK_EVENT(E_DRAGDROPFINISH, DragDropFinish)
{
    FLOCKSDK_PARAM(P_SOURCE, Source);                // UIElement pointer
    FLOCKSDK_PARAM(P_TARGET, Target);                // UIElement pointer
    FLOCKSDK_PARAM(P_ACCEPT, Accept);                // bool
};

/// Focus element changed.
FLOCKSDK_EVENT(E_FOCUSCHANGED, FocusChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_CLICKEDELEMENT, ClickedElement); // UIElement pointer
}

/// UI element name changed.
FLOCKSDK_EVENT(E_NAMECHANGED, NameChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// UI element resized.
FLOCKSDK_EVENT(E_RESIZED, Resized)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_WIDTH, Width);                  // int
    FLOCKSDK_PARAM(P_HEIGHT, Height);                // int
    FLOCKSDK_PARAM(P_DX, DX);                        // int
    FLOCKSDK_PARAM(P_DY, DY);                        // int
}

/// UI element positioned.
FLOCKSDK_EVENT(E_POSITIONED, Positioned)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
}

/// UI element visibility changed.
FLOCKSDK_EVENT(E_VISIBLECHANGED, VisibleChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_VISIBLE, Visible);              // bool
}

/// UI element focused.
FLOCKSDK_EVENT(E_FOCUSED, Focused)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_BYKEY, ByKey);                  // bool
}

/// UI element defocused.
FLOCKSDK_EVENT(E_DEFOCUSED, Defocused)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// UI element layout updated.
FLOCKSDK_EVENT(E_LAYOUTUPDATED, LayoutUpdated)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// UI button pressed.
FLOCKSDK_EVENT(E_PRESSED, Pressed)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// UI button was pressed, then released.
FLOCKSDK_EVENT(E_RELEASED, Released)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// UI checkbox toggled.
FLOCKSDK_EVENT(E_TOGGLED, Toggled)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_STATE, State);                  // bool
}

/// UI slider value changed
FLOCKSDK_EVENT(E_SLIDERCHANGED, SliderChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_VALUE, Value);                  // float
}

/// UI slider being paged.
FLOCKSDK_EVENT(E_SLIDERPAGED, SliderPaged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_OFFSET, Offset);                // int
    FLOCKSDK_PARAM(P_PRESSED, Pressed);              // bool
}

/// UI progressbar value changed
FLOCKSDK_EVENT(E_PROGRESSBARCHANGED, ProgressBarChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_VALUE, Value);                  // float
}

/// UI scrollbar value changed.
FLOCKSDK_EVENT(E_SCROLLBARCHANGED, ScrollBarChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_VALUE, Value);                  // float
}

/// UI scrollview position changed.
FLOCKSDK_EVENT(E_VIEWCHANGED, ViewChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
}

/// UI modal changed (currently only Window has modal flag).
FLOCKSDK_EVENT(E_MODALCHANGED, ModalChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_MODAL, Modal);                  // bool
}

/// Text entry into a LineEdit. The text can be modified in the event data.
FLOCKSDK_EVENT(E_TEXTENTRY, TextEntry)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_TEXT, Text);                    // String [in/out]
}

/// Editable text changed
FLOCKSDK_EVENT(E_TEXTCHANGED, TextChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_TEXT, Text);                    // String
}

/// Text editing finished (enter pressed on a LineEdit)
FLOCKSDK_EVENT(E_TEXTFINISHED, TextFinished)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_TEXT, Text);                    // String
    FLOCKSDK_PARAM(P_VALUE, Value);                 // Float
}

/// Menu selected.
FLOCKSDK_EVENT(E_MENUSELECTED, MenuSelected)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// Listview or DropDownList item selected.
FLOCKSDK_EVENT(E_ITEMSELECTED, ItemSelected)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_SELECTION, Selection);          // int
}

/// Listview item deselected.
FLOCKSDK_EVENT(E_ITEMDESELECTED, ItemDeselected)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_SELECTION, Selection);          // int
}

/// Listview selection change finished.
FLOCKSDK_EVENT(E_SELECTIONCHANGED, SelectionChanged)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// Listview item clicked. If this is a left-click, also ItemSelected event will be sent. If this is a right-click, only this event is sent.
FLOCKSDK_EVENT(E_ITEMCLICKED, ItemClicked)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_ITEM, Item);                    // UIElement pointer
    FLOCKSDK_PARAM(P_SELECTION, Selection);          // int
    FLOCKSDK_PARAM(P_BUTTON, Button);                // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Listview item double clicked.
FLOCKSDK_EVENT(E_ITEMDOUBLECLICKED, ItemDoubleClicked)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_ITEM, Item);                    // UIElement pointer
    FLOCKSDK_PARAM(P_SELECTION, Selection);          // int
    FLOCKSDK_PARAM(P_BUTTON, Button);                // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// LineEdit or ListView unhandled key pressed.
FLOCKSDK_EVENT(E_UNHANDLEDKEY, UnhandledKey)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_KEY, Key);                      // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Fileselector choice.
FLOCKSDK_EVENT(E_FILESELECTED, FileSelected)
{
    FLOCKSDK_PARAM(P_FILENAME, FileName);            // String
    FLOCKSDK_PARAM(P_FILTER, Filter);                // String
    FLOCKSDK_PARAM(P_OK, Ok);                        // bool
}

/// MessageBox acknowlegement.
FLOCKSDK_EVENT(E_MESSAGEACK, MessageACK)
{
    FLOCKSDK_PARAM(P_OK, Ok);                        // bool
}

/// A child element has been added to an element. Sent by the UI root element, or element-event-sender if set.
FLOCKSDK_EVENT(E_ELEMENTADDED, ElementAdded)
{
    FLOCKSDK_PARAM(P_ROOT, Root);                    // UIElement pointer
    FLOCKSDK_PARAM(P_PARENT, Parent);                // UIElement pointer
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// A child element is about to be removed from an element. Sent by the UI root element, or element-event-sender if set.
FLOCKSDK_EVENT(E_ELEMENTREMOVED, ElementRemoved)
{
    FLOCKSDK_PARAM(P_ROOT, Root);                    // UIElement pointer
    FLOCKSDK_PARAM(P_PARENT, Parent);                // UIElement pointer
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// Hovering on an UI element has started
FLOCKSDK_EVENT(E_HOVERBEGIN, HoverBegin)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_ELEMENTX, ElementX);            // int
    FLOCKSDK_PARAM(P_ELEMENTY, ElementY);            // int
}

/// Hovering on an UI element has ended
FLOCKSDK_EVENT(E_HOVEREND, HoverEnd)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
}

/// Drag behavior of a UI Element has started
FLOCKSDK_EVENT(E_DRAGBEGIN, DragBegin)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_ELEMENTX, ElementX);            // int
    FLOCKSDK_PARAM(P_ELEMENTY, ElementY);            // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_NUMBUTTONS, NumButtons);        // int
}

/// Drag behavior of a UI Element when the input device has moved
FLOCKSDK_EVENT(E_DRAGMOVE, DragMove)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_DX, DX);                        // int
    FLOCKSDK_PARAM(P_DY, DY);                        // int
    FLOCKSDK_PARAM(P_ELEMENTX, ElementX);            // int
    FLOCKSDK_PARAM(P_ELEMENTY, ElementY);            // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_NUMBUTTONS, NumButtons);        // int
}

/// Drag behavior of a UI Element has finished
FLOCKSDK_EVENT(E_DRAGEND, DragEnd)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_ELEMENTX, ElementX);            // int
    FLOCKSDK_PARAM(P_ELEMENTY, ElementY);            // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_NUMBUTTONS, NumButtons);        // int
}

/// Drag of a UI Element was canceled by pressing ESC
FLOCKSDK_EVENT(E_DRAGCANCEL, DragCancel)
{
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_ELEMENTX, ElementX);            // int
    FLOCKSDK_PARAM(P_ELEMENTY, ElementY);            // int
    FLOCKSDK_PARAM(P_BUTTONS, Buttons);              // int
    FLOCKSDK_PARAM(P_NUMBUTTONS, NumButtons);        // int
}

/// A file was drag-dropped into the application window. Includes also coordinates and UI element if applicable
FLOCKSDK_EVENT(E_UIDROPFILE, UIDropFile)
{
    FLOCKSDK_PARAM(P_FILENAME, FileName);            // String
    FLOCKSDK_PARAM(P_ELEMENT, Element);              // UIElement pointer
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
    FLOCKSDK_PARAM(P_ELEMENTX, ElementX);            // int (only if element is non-null)
    FLOCKSDK_PARAM(P_ELEMENTY, ElementY);            // int (only if element is non-null)
}

}
