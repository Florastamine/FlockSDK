//
// Copyright (c) 2008-2016 the Urho3D project.
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

#include "../Precompiled.h"

#include "../Core/Context.h"
#include "../Core/CoreEvents.h"
#include "../Core/Mutex.h"
#include "../Core/ProcessUtils.h"
#include "../Core/Profiler.h"
#include "../Core/StringUtils.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/GraphicsEvents.h"
#include "../Input/Input.h"
#include "../IO/FileSystem.h"
#include "../IO/Log.h"
#include "../IO/RWOpsWrapper.h"
#include "../Resource/ResourceCache.h"
#include "../UI/Text.h"
#include "../UI/UI.h"

#include <SDL/SDL.h>



// Use a "click inside window to focus" mechanism on desktop platforms when the mouse cursor is hidden
// TODO: For now, in this particular case only, treat all the ARM on Linux as "desktop" (e.g. RPI, odroid, etc), revisit this again when we support "mobile" ARM on Linux
#if defined(_WIN32) || defined(__linux__)
#define REQUIRE_CLICK_TO_FOCUS
#endif

namespace Urho3D
{

const StringHash VAR_BUTTON_KEY_BINDING("VAR_BUTTON_KEY_BINDING");
const StringHash VAR_BUTTON_MOUSE_BUTTON_BINDING("VAR_BUTTON_MOUSE_BUTTON_BINDING");
const StringHash VAR_LAST_KEYSYM("VAR_LAST_KEYSYM");

/// Convert SDL keycode if necessary.
int ConvertSDLKeyCode(int keySym, int scanCode)
{
    if (scanCode == SCANCODE_AC_BACK)
        return KEY_ESCAPE;
    else
        return SDL_tolower(keySym);
}

Input::Input(Context* context) :
    Object(context),
    mouseButtonDown_(0),
    mouseButtonPress_(0),
    lastVisibleMousePosition_(MOUSE_POSITION_OFFSCREEN),
    mouseMoveWheel_(0),
    windowID_(0),
    toggleFullscreen_(true),
    mouseVisible_(false),
    lastMouseVisible_(false),
    mouseGrabbed_(false),
    lastMouseGrabbed_(false),
    mouseMode_(MM_ABSOLUTE),
    lastMouseMode_(MM_ABSOLUTE),
    sdlMouseRelative_(false),
    inputFocus_(false),
    minimized_(false),
    focusedThisFrame_(false),
    suppressNextMouseMove_(false),
    mouseMoveScaled_(false),
    initialized_(false)
{
    SubscribeToEvent(E_SCREENMODE, URHO3D_HANDLER(Input, HandleScreenMode));

    // Try to initialize right now, but skip if screen mode is not yet set
    Initialize();
}

Input::~Input()
{
    context_->ReleaseSDL();
}

void Input::Update()
{
    assert(initialized_);

    URHO3D_PROFILE(UpdateInput);

    bool mouseMoved = false;
    if (mouseMove_ != IntVector2::ZERO)
        mouseMoved = true;

    ResetInputAccumulation();

    SDL_Event evt;
    while (SDL_PollEvent(&evt))
        HandleSDLEvent(&evt);

    if (suppressNextMouseMove_ && (mouseMove_ != IntVector2::ZERO || mouseMoved))
        UnsuppressMouseMove();

    // Check for focus change this frame
    SDL_Window* window = graphics_->GetWindow();
    unsigned flags = window ? SDL_GetWindowFlags(window) & (SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS) : 0;
#ifndef __EMSCRIPTEN__
    if (window)
    {
#ifdef REQUIRE_CLICK_TO_FOCUS
        // When using the "click to focus" mechanism, only focus automatically in fullscreen or non-hidden mouse mode
        if (!inputFocus_ && ((mouseVisible_ || mouseMode_ == MM_FREE) || graphics_->GetFullscreen()) && (flags & SDL_WINDOW_INPUT_FOCUS))
#else
        if (!inputFocus_ && (flags & SDL_WINDOW_INPUT_FOCUS))
#endif
            focusedThisFrame_ = true;

        if (focusedThisFrame_)
            GainFocus();

        // Check for losing focus. The window flags are not reliable when using an external window, so prevent losing focus in that case
        if (inputFocus_ && !graphics_->GetExternalWindow() && (flags & SDL_WINDOW_INPUT_FOCUS) == 0)
            LoseFocus();
    }
    else
        return;

    // Handle mouse mode MM_WRAP
    if (mouseVisible_ && mouseMode_ == MM_WRAP)
    {
        IntVector2 windowPos = graphics_->GetWindowPosition();
        IntVector2 mpos;
        SDL_GetGlobalMouseState(&mpos.x_, &mpos.y_);
        mpos -= windowPos;

        const int buffer = 5;
        const int width = graphics_->GetWidth() - buffer * 2;
        const int height = graphics_->GetHeight() - buffer * 2;

        // SetMousePosition utilizes backbuffer coordinate system, scale now from window coordinates
        mpos.x_ = (int)(mpos.x_ * inputScale_.x_);
        mpos.y_ = (int)(mpos.y_ * inputScale_.y_);

        bool warp = false;
        if (mpos.x_ < buffer)
        {
            warp = true;
            mpos.x_ += width;
        }

        if (mpos.x_ > buffer + width)
        {
            warp = true;
            mpos.x_ -= width;
        }

        if (mpos.y_ < buffer)
        {
            warp = true;
            mpos.y_ += height;
        }

        if (mpos.y_ > buffer + height)
        {
            warp = true;
            mpos.y_ -= height;
        }

        if (warp)
        {
            SetMousePosition(mpos);
            SuppressNextMouseMove();
        }
    }
#else
    if (!window)
        return;
#endif

    if ((graphics_->GetExternalWindow() || (!mouseVisible_ && inputFocus_ && (flags & SDL_WINDOW_MOUSE_FOCUS))))
    {
        const IntVector2 mousePosition = GetMousePosition();
        mouseMove_ = mousePosition - lastMousePosition_;
        mouseMoveScaled_ = true; // Already in backbuffer scale, since GetMousePosition() operates in that

        if (graphics_->GetExternalWindow())
            lastMousePosition_ = mousePosition;
        else
        {
            // Recenter the mouse cursor manually after move
            CenterMousePosition();
        }
        // Send mouse move event if necessary
        if (mouseMove_ != IntVector2::ZERO)
        {
            if (!suppressNextMouseMove_)
            {
                using namespace MouseMove;

                VariantMap& eventData = GetEventDataMap();

                eventData[P_X] = mousePosition.x_;
                eventData[P_Y] = mousePosition.y_;
                eventData[P_DX] = mouseMove_.x_;
                eventData[P_DY] = mouseMove_.y_;
                eventData[P_BUTTONS] = mouseButtonDown_;
                eventData[P_QUALIFIERS] = GetQualifiers();
                SendEvent(E_MOUSEMOVE, eventData);
            }
        }
    }
    else if (!mouseVisible_ && sdlMouseRelative_ && inputFocus_ && (flags & SDL_WINDOW_MOUSE_FOCUS))
    {
        // Keep the cursor trapped in window.
        CenterMousePosition();
    }
}

void Input::SetMouseVisible(bool enable, bool suppressEvent)
{
    const bool startMouseVisible = mouseVisible_;

    // In mouse mode relative, the mouse should be invisible
    if (mouseMode_ == MM_RELATIVE)
    {
        if (!suppressEvent)
            lastMouseVisible_ = enable;

        enable = false;
    }

    if (enable != mouseVisible_)
    {
        if (initialized_)
        {
            // External windows can only support visible mouse cursor
            if (graphics_->GetExternalWindow())
            {
                mouseVisible_ = true;
                if (!suppressEvent)
                    lastMouseVisible_ = true;
                return;
            }

            if (!enable && inputFocus_)
            {
                if (mouseVisible_)
                    lastVisibleMousePosition_ = GetMousePosition();

                if (mouseMode_ == MM_ABSOLUTE)
                    SetMouseModeAbsolute(SDL_TRUE);

                SDL_ShowCursor(SDL_FALSE);
                mouseVisible_ = false;
            }
            else if (mouseMode_ != MM_RELATIVE)
            {
                SetMouseGrabbed(false, suppressEvent);

                SDL_ShowCursor(SDL_TRUE);
                mouseVisible_ = true;
                if (mouseMode_ == MM_ABSOLUTE)
                    SetMouseModeAbsolute(SDL_FALSE);

                // Update cursor position
                UI* ui = GetSubsystem<UI>();
                Cursor* cursor = ui->GetCursor();
                // If the UI Cursor was visible, use that position instead of last visible OS cursor position
                if (cursor && cursor->IsVisible())
                {
                    IntVector2 pos = cursor->GetScreenPosition();
                    if (pos != MOUSE_POSITION_OFFSCREEN)
                    {
                        SetMousePosition(pos);
                        lastMousePosition_ = pos;
                    }
                }
                else
                {
                    if (lastVisibleMousePosition_ != MOUSE_POSITION_OFFSCREEN)
                    {
                        SetMousePosition(lastVisibleMousePosition_);
                        lastMousePosition_ = lastVisibleMousePosition_;
                    }
                }
            }
        }
        else
        {
            // Allow to set desired mouse visibility before initialization
            mouseVisible_ = enable;
        }

        if (mouseVisible_ != startMouseVisible)
        {
            SuppressNextMouseMove();
            if (!suppressEvent)
            {
                lastMouseVisible_ = mouseVisible_;
                using namespace MouseVisibleChanged;

                VariantMap& eventData = GetEventDataMap();
                eventData[P_VISIBLE] = mouseVisible_;
                SendEvent(E_MOUSEVISIBLECHANGED, eventData);
            }
        }
    }
}

void Input::ResetMouseVisible()
{
    SetMouseVisible(lastMouseVisible_, false);
}

void Input::SetMouseGrabbed(bool grab, bool suppressEvent)
{
    mouseGrabbed_ = grab;

    if (!suppressEvent)
        lastMouseGrabbed_ = grab;
}

void Input::ResetMouseGrabbed()
{
    SetMouseGrabbed(lastMouseGrabbed_, true);
}

void Input::SetMouseModeAbsolute(SDL_bool enable)
{
    SDL_Window* const window = graphics_->GetWindow();

    SDL_SetWindowGrab(window, enable);
}

void Input::SetMouseModeRelative(SDL_bool enable)
{
    SDL_Window* const window = graphics_->GetWindow();

    int result = SDL_SetRelativeMouseMode(enable);
    sdlMouseRelative_ = enable && (result == 0);

    if (result == -1)
        SDL_SetWindowGrab(window, enable);
}

void Input::SetMouseMode(MouseMode mode, bool suppressEvent)
{
    const MouseMode previousMode = mouseMode_;
    if (mode != mouseMode_)
    {
        if (initialized_)
        {
            SuppressNextMouseMove();

            mouseMode_ = mode;
            SDL_Window* const window = graphics_->GetWindow();

            UI* const ui = GetSubsystem<UI>();
            Cursor* const cursor = ui->GetCursor();

            // Handle changing from previous mode
            if (previousMode == MM_ABSOLUTE)
            {
                if (!mouseVisible_)
                    SetMouseModeAbsolute(SDL_FALSE);
            }
            if (previousMode == MM_RELATIVE)
            {
                SetMouseModeRelative(SDL_FALSE);
                ResetMouseVisible();
            }
            else if (previousMode == MM_WRAP)
                SDL_SetWindowGrab(window, SDL_FALSE);

            // Handle changing to new mode
            if (mode == MM_ABSOLUTE)
            {
                if (!mouseVisible_)
                    SetMouseModeAbsolute(SDL_TRUE);
            }
            else if (mode == MM_RELATIVE)
            {
                SetMouseVisible(false, true);
                SetMouseModeRelative(SDL_TRUE);
            }
            else if (mode == MM_WRAP)
            {
                SetMouseGrabbed(true, suppressEvent);
                SDL_SetWindowGrab(window, SDL_TRUE);
            }

            if (mode != MM_WRAP)
                SetMouseGrabbed(!(mouseVisible_ || (cursor && cursor->IsVisible())), suppressEvent);
        }
        else
        {
            // Allow to set desired mouse mode before initialization
            mouseMode_ = mode;
        }
    }

    if (!suppressEvent)
    {
        lastMouseMode_ = mode;

        if (mouseMode_ != previousMode)
        {
            VariantMap& eventData = GetEventDataMap();
            eventData[MouseModeChanged::P_MODE] = mode;
            eventData[MouseModeChanged::P_MOUSELOCKED] = IsMouseLocked();
            SendEvent(E_MOUSEMODECHANGED, eventData);
        }
    }
}

void Input::ResetMouseMode()
{
    SetMouseMode(lastMouseMode_, false);
}

void Input::SetToggleFullscreen(bool enable)
{
    toggleFullscreen_ = enable;
}

static void PopulateKeyBindingMap(HashMap<String, int>& keyBindingMap)
{
    if (keyBindingMap.Empty())
    {
        keyBindingMap.Insert(MakePair<String, int>("SPACE", KEY_SPACE));
        keyBindingMap.Insert(MakePair<String, int>("LCTRL", KEY_LCTRL));
        keyBindingMap.Insert(MakePair<String, int>("RCTRL", KEY_RCTRL));
        keyBindingMap.Insert(MakePair<String, int>("LSHIFT", KEY_LSHIFT));
        keyBindingMap.Insert(MakePair<String, int>("RSHIFT", KEY_RSHIFT));
        keyBindingMap.Insert(MakePair<String, int>("LALT", KEY_LALT));
        keyBindingMap.Insert(MakePair<String, int>("RALT", KEY_RALT));
        keyBindingMap.Insert(MakePair<String, int>("LGUI", KEY_LGUI));
        keyBindingMap.Insert(MakePair<String, int>("RGUI", KEY_RGUI));
        keyBindingMap.Insert(MakePair<String, int>("TAB", KEY_TAB));
        keyBindingMap.Insert(MakePair<String, int>("RETURN", KEY_RETURN));
        keyBindingMap.Insert(MakePair<String, int>("RETURN2", KEY_RETURN2));
        keyBindingMap.Insert(MakePair<String, int>("ENTER", KEY_KP_ENTER));
        keyBindingMap.Insert(MakePair<String, int>("SELECT", KEY_SELECT));
        keyBindingMap.Insert(MakePair<String, int>("LEFT", KEY_LEFT));
        keyBindingMap.Insert(MakePair<String, int>("RIGHT", KEY_RIGHT));
        keyBindingMap.Insert(MakePair<String, int>("UP", KEY_UP));
        keyBindingMap.Insert(MakePair<String, int>("DOWN", KEY_DOWN));
        keyBindingMap.Insert(MakePair<String, int>("PAGEUP", KEY_PAGEUP));
        keyBindingMap.Insert(MakePair<String, int>("PAGEDOWN", KEY_PAGEDOWN));
        keyBindingMap.Insert(MakePair<String, int>("F1", KEY_F1));
        keyBindingMap.Insert(MakePair<String, int>("F2", KEY_F2));
        keyBindingMap.Insert(MakePair<String, int>("F3", KEY_F3));
        keyBindingMap.Insert(MakePair<String, int>("F4", KEY_F4));
        keyBindingMap.Insert(MakePair<String, int>("F5", KEY_F5));
        keyBindingMap.Insert(MakePair<String, int>("F6", KEY_F6));
        keyBindingMap.Insert(MakePair<String, int>("F7", KEY_F7));
        keyBindingMap.Insert(MakePair<String, int>("F8", KEY_F8));
        keyBindingMap.Insert(MakePair<String, int>("F9", KEY_F9));
        keyBindingMap.Insert(MakePair<String, int>("F10", KEY_F10));
        keyBindingMap.Insert(MakePair<String, int>("F11", KEY_F11));
        keyBindingMap.Insert(MakePair<String, int>("F12", KEY_F12));
    }
}

static void PopulateMouseButtonBindingMap(HashMap<String, int>& mouseButtonBindingMap)
{
    if (mouseButtonBindingMap.Empty())
    {
        mouseButtonBindingMap.Insert(MakePair<String, int>("LEFT", SDL_BUTTON_LEFT));
        mouseButtonBindingMap.Insert(MakePair<String, int>("MIDDLE", SDL_BUTTON_MIDDLE));
        mouseButtonBindingMap.Insert(MakePair<String, int>("RIGHT", SDL_BUTTON_RIGHT));
        mouseButtonBindingMap.Insert(MakePair<String, int>("X1", SDL_BUTTON_X1));
        mouseButtonBindingMap.Insert(MakePair<String, int>("X2", SDL_BUTTON_X2));
    }
}

void Input::SetScreenKeyboardVisible(bool enable)
{
    if (enable != SDL_IsTextInputActive())
    {
        if (enable)
            SDL_StartTextInput();
        else
            SDL_StopTextInput();
    }
}

int Input::GetKeyFromName(const String& name) const
{
    return SDL_GetKeyFromName(name.CString());
}

int Input::GetKeyFromScancode(int scancode) const
{
    return SDL_GetKeyFromScancode((SDL_Scancode)scancode);
}

String Input::GetKeyName(int key) const
{
    return String(SDL_GetKeyName(key));
}

int Input::GetScancodeFromKey(int key) const
{
    return SDL_GetScancodeFromKey(key);
}

int Input::GetScancodeFromName(const String& name) const
{
    return SDL_GetScancodeFromName(name.CString());
}

String Input::GetScancodeName(int scancode) const
{
    return SDL_GetScancodeName((SDL_Scancode)scancode);
}

bool Input::GetKeyDown(int key) const
{
    return keyDown_.Contains(SDL_tolower(key));
}

bool Input::GetKeyPress(int key) const
{
    return keyPress_.Contains(SDL_tolower(key));
}

bool Input::GetScancodeDown(int scancode) const
{
    return scancodeDown_.Contains(scancode);
}

bool Input::GetScancodePress(int scancode) const
{
    return scancodePress_.Contains(scancode);
}

bool Input::GetMouseButtonDown(int button) const
{
    return (mouseButtonDown_ & button) != 0;
}

bool Input::GetMouseButtonPress(int button) const
{
    return (mouseButtonPress_ & button) != 0;
}

bool Input::GetQualifierDown(int qualifier) const
{
    if (qualifier == QUAL_SHIFT)
        return GetKeyDown(KEY_LSHIFT) || GetKeyDown(KEY_RSHIFT);
    if (qualifier == QUAL_CTRL)
        return GetKeyDown(KEY_LCTRL) || GetKeyDown(KEY_RCTRL);
    if (qualifier == QUAL_ALT)
        return GetKeyDown(KEY_LALT) || GetKeyDown(KEY_RALT);

    return false;
}

bool Input::GetQualifierPress(int qualifier) const
{
    if (qualifier == QUAL_SHIFT)
        return GetKeyPress(KEY_LSHIFT) || GetKeyPress(KEY_RSHIFT);
    if (qualifier == QUAL_CTRL)
        return GetKeyPress(KEY_LCTRL) || GetKeyPress(KEY_RCTRL);
    if (qualifier == QUAL_ALT)
        return GetKeyPress(KEY_LALT) || GetKeyPress(KEY_RALT);

    return false;
}

int Input::GetQualifiers() const
{
    int ret = 0;
    if (GetQualifierDown(QUAL_SHIFT))
        ret |= QUAL_SHIFT;
    if (GetQualifierDown(QUAL_CTRL))
        ret |= QUAL_CTRL;
    if (GetQualifierDown(QUAL_ALT))
        ret |= QUAL_ALT;

    return ret;
}

IntVector2 Input::GetMousePosition() const
{
    IntVector2 ret = IntVector2::ZERO;

    if (!initialized_)
        return ret;

    SDL_GetMouseState(&ret.x_, &ret.y_);
    ret.x_ = (int)(ret.x_ * inputScale_.x_);
    ret.y_ = (int)(ret.y_ * inputScale_.y_);

    return ret;
}

IntVector2 Input::GetMouseMove() const
{
    if (!suppressNextMouseMove_)
        return mouseMoveScaled_ ? mouseMove_ : IntVector2((int)(mouseMove_.x_ * inputScale_.x_), (int)(mouseMove_.y_ * inputScale_.y_));
    else
        return IntVector2::ZERO;
}

int Input::GetMouseMoveX() const
{
    if (!suppressNextMouseMove_)
        return mouseMoveScaled_ ? mouseMove_.x_ : (int)(mouseMove_.x_ * inputScale_.x_);
    else
        return 0;
}

int Input::GetMouseMoveY() const
{
    if (!suppressNextMouseMove_)
        return mouseMoveScaled_ ? mouseMove_.y_ : mouseMove_.y_ * inputScale_.y_;
    else
        return 0;
}

bool Input::GetScreenKeyboardSupport() const
{
    return SDL_HasScreenKeyboardSupport();
}

bool Input::IsScreenKeyboardVisible() const
{
    return SDL_IsTextInputActive();
}

bool Input::IsMouseLocked() const
{
    return !((mouseMode_ == MM_ABSOLUTE && mouseVisible_) || mouseMode_ == MM_FREE);
}

bool Input::IsMinimized() const
{
    // Return minimized state also when unfocused in fullscreen
    if (!inputFocus_ && graphics_ && graphics_->GetFullscreen())
        return true;
    else
        return minimized_;
}

void Input::Initialize()
{
    Graphics* graphics = GetSubsystem<Graphics>();
    if (!graphics || !graphics->IsInitialized())
        return;

    graphics_ = graphics;

    // In external window mode only visible mouse is supported
    if (graphics_->GetExternalWindow())
        mouseVisible_ = true;

    // Set the initial activation
    initialized_ = true;
    GainFocus();

    ResetState();

    SubscribeToEvent(E_BEGINFRAME, URHO3D_HANDLER(Input, HandleBeginFrame));

    URHO3D_LOGINFO("Initialized input");
}

void Input::ResetInputAccumulation()
{
    // Reset input accumulation for this frame
    keyPress_.Clear();
    scancodePress_.Clear();
    mouseButtonPress_ = 0;
    mouseMove_ = IntVector2::ZERO;
    mouseMoveWheel_ = 0;
}

void Input::GainFocus()
{
    ResetState();

    inputFocus_ = true;
    focusedThisFrame_ = false;

    // Restore mouse mode
    const MouseMode mm = mouseMode_;
    mouseMode_ = MM_FREE;
    SetMouseMode(mm, true);

    SuppressNextMouseMove();

    // Re-establish mouse cursor hiding as necessary
    if (!mouseVisible_)
        SDL_ShowCursor(SDL_FALSE);

    SendInputFocusEvent();
}

void Input::LoseFocus()
{
    ResetState();

    inputFocus_ = false;
    focusedThisFrame_ = false;

    // Show the mouse cursor when inactive
    SDL_ShowCursor(SDL_TRUE);

    // Change mouse mode -- removing any cursor grabs, etc.
    const MouseMode mm = mouseMode_;
    SetMouseMode(MM_FREE, true);
    // Restore flags to reflect correct mouse state.
    mouseMode_ = mm;

    SendInputFocusEvent();
}

void Input::ResetState()
{
    keyDown_.Clear();
    keyPress_.Clear();
    scancodeDown_.Clear();
    scancodePress_.Clear();

    // Use SetMouseButton() to reset the state so that mouse events will be sent properly
    SetMouseButton(MOUSEB_LEFT, false);
    SetMouseButton(MOUSEB_RIGHT, false);
    SetMouseButton(MOUSEB_MIDDLE, false);

    mouseMove_ = IntVector2::ZERO;
    mouseMoveWheel_ = 0;
    mouseButtonPress_ = 0;
}

void Input::SendInputFocusEvent()
{
    using namespace InputFocus;

    VariantMap& eventData = GetEventDataMap();
    eventData[P_FOCUS] = HasFocus();
    eventData[P_MINIMIZED] = IsMinimized();
    SendEvent(E_INPUTFOCUS, eventData);
}

void Input::SetMouseButton(int button, bool newState)
{
    if (newState)
    {
        if (!(mouseButtonDown_ & button))
            mouseButtonPress_ |= button;

        mouseButtonDown_ |= button;
    }
    else
    {
        if (!(mouseButtonDown_ & button))
            return;

        mouseButtonDown_ &= ~button;
    }

    using namespace MouseButtonDown;

    VariantMap& eventData = GetEventDataMap();
    eventData[P_BUTTON] = button;
    eventData[P_BUTTONS] = mouseButtonDown_;
    eventData[P_QUALIFIERS] = GetQualifiers();
    SendEvent(newState ? E_MOUSEBUTTONDOWN : E_MOUSEBUTTONUP, eventData);
}

void Input::SetKey(int key, int scancode, bool newState)
{
    bool repeat = false;

    if (newState)
    {
        scancodeDown_.Insert(scancode);
        scancodePress_.Insert(scancode);

        if (!keyDown_.Contains(key))
        {
            keyDown_.Insert(key);
            keyPress_.Insert(key);
        }
        else
            repeat = true;
    }
    else
    {
        scancodeDown_.Erase(scancode);

        if (!keyDown_.Erase(key))
            return;
    }

    using namespace KeyDown;

    VariantMap& eventData = GetEventDataMap();
    eventData[P_KEY] = key;
    eventData[P_SCANCODE] = scancode;
    eventData[P_BUTTONS] = mouseButtonDown_;
    eventData[P_QUALIFIERS] = GetQualifiers();
    if (newState)
        eventData[P_REPEAT] = repeat;
    SendEvent(newState ? E_KEYDOWN : E_KEYUP, eventData);

    if ((key == KEY_RETURN || key == KEY_RETURN2 || key == KEY_KP_ENTER) && newState && !repeat && toggleFullscreen_ &&
        (GetKeyDown(KEY_LALT) || GetKeyDown(KEY_RALT)))
        graphics_->ToggleFullscreen();
}

void Input::SetMouseWheel(int delta)
{
    if (delta)
    {
        mouseMoveWheel_ += delta;

        using namespace MouseWheel;

        VariantMap& eventData = GetEventDataMap();
        eventData[P_WHEEL] = delta;
        eventData[P_BUTTONS] = mouseButtonDown_;
        eventData[P_QUALIFIERS] = GetQualifiers();
        SendEvent(E_MOUSEWHEEL, eventData);
    }
}

void Input::SetMousePosition(const IntVector2& position)
{
    if (!graphics_)
        return;

    SDL_WarpMouseInWindow(graphics_->GetWindow(), (int)(position.x_ / inputScale_.x_), (int)(position.y_ / inputScale_.y_));
}

void Input::CenterMousePosition()
{
    const IntVector2 center(graphics_->GetWidth() / 2, graphics_->GetHeight() / 2);
    if (GetMousePosition() != center)
    {
        SetMousePosition(center);
        lastMousePosition_ = center;
    }
}

void Input::SuppressNextMouseMove()
{
    suppressNextMouseMove_ = true;
    mouseMove_ = IntVector2::ZERO;
}

void Input::UnsuppressMouseMove()
{
    suppressNextMouseMove_ = false;
    mouseMove_ = IntVector2::ZERO;
    lastMousePosition_ = GetMousePosition();
}

void Input::HandleSDLEvent(void* sdlEvent)
{
    SDL_Event& evt = *static_cast<SDL_Event*>(sdlEvent);

    // While not having input focus, skip key/mouse/touch/joystick events, except for the "click to focus" mechanism
    if (!inputFocus_ && evt.type >= SDL_KEYDOWN)
    {
#ifdef REQUIRE_CLICK_TO_FOCUS
        // Require the click to be at least 1 pixel inside the window to disregard clicks in the title bar
        if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.x > 0 && evt.button.y > 0 && evt.button.x < graphics_->GetWidth() - 1 &&
            evt.button.y < graphics_->GetHeight() - 1)
        {
            focusedThisFrame_ = true;
            // Do not cause the click to actually go throughfin
            return;
        }
        else
#endif
            return;
    }

    // Possibility for custom handling or suppression of default handling for the SDL event
    {
        using namespace SDLRawInput;

        VariantMap eventData = GetEventDataMap();
        eventData[P_SDLEVENT] = &evt;
        eventData[P_CONSUMED] = false;
        SendEvent(E_SDLRAWINPUT, eventData);

        if (eventData[P_CONSUMED].GetBool())
            return;
    }

    switch (evt.type)
    {
    case SDL_KEYDOWN:
        SetKey(ConvertSDLKeyCode(evt.key.keysym.sym, evt.key.keysym.scancode), evt.key.keysym.scancode, true);
        break;

    case SDL_KEYUP:
        SetKey(ConvertSDLKeyCode(evt.key.keysym.sym, evt.key.keysym.scancode), evt.key.keysym.scancode, false);
        break;

    case SDL_TEXTINPUT:
        {
            using namespace TextInput;

            VariantMap textInputEventData;
            textInputEventData[P_TEXT] = textInput_ = &evt.text.text[0];
            SendEvent(E_TEXTINPUT, textInputEventData);
        }
        break;

    case SDL_TEXTEDITING:
        {
            using namespace TextEditing;

            VariantMap textEditingEventData;
            textEditingEventData[P_COMPOSITION] = &evt.edit.text[0];
            textEditingEventData[P_CURSOR] = evt.edit.start;
            textEditingEventData[P_SELECTION_LENGTH] = evt.edit.length;
            SendEvent(E_TEXTEDITING, textEditingEventData);
        }
        break;

    case SDL_MOUSEBUTTONDOWN:
        SetMouseButton(1 << (evt.button.button - 1), true);
        break;

    case SDL_MOUSEBUTTONUP:
        SetMouseButton(1 << (evt.button.button - 1), false);
        break;

    case SDL_MOUSEMOTION:
        if ((sdlMouseRelative_ || mouseVisible_ || mouseMode_ == MM_FREE))
        {
            // Accumulate without scaling for accuracy, needs to be scaled to backbuffer coordinates when asked
            mouseMove_.x_ += evt.motion.xrel;
            mouseMove_.y_ += evt.motion.yrel;
            mouseMoveScaled_ = false;

            if (!suppressNextMouseMove_)
            {
                using namespace MouseMove;

                VariantMap& eventData = GetEventDataMap();
                eventData[P_X] = (int)(evt.motion.x * inputScale_.x_);
                eventData[P_Y] = (int)(evt.motion.y * inputScale_.y_);
                // The "on-the-fly" motion data needs to be scaled now, though this may reduce accuracy
                eventData[P_DX] = (int)(evt.motion.xrel * inputScale_.x_);
                eventData[P_DY] = (int)(evt.motion.yrel * inputScale_.y_);
                eventData[P_BUTTONS] = mouseButtonDown_;
                eventData[P_QUALIFIERS] = GetQualifiers();
                SendEvent(E_MOUSEMOVE, eventData);
            }
        }
        break;

    case SDL_MOUSEWHEEL:
        SetMouseWheel(evt.wheel.y);
        break;

    case SDL_WINDOWEVENT:
        {
            switch (evt.window.event)
            {
            case SDL_WINDOWEVENT_MINIMIZED:
                minimized_ = true;
                SendInputFocusEvent();
                break;

            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
                minimized_ = false;
                SendInputFocusEvent();
                break;

            case SDL_WINDOWEVENT_RESIZED:
                graphics_->OnWindowResized();
                break;
            case SDL_WINDOWEVENT_MOVED:
                graphics_->OnWindowMoved();
                break;

            default: break;
            }
        }
        break;

    case SDL_DROPFILE:
        {
            using namespace DropFile;

            VariantMap& eventData = GetEventDataMap();
            eventData[P_FILENAME] = GetInternalPath(String(evt.drop.file));
            SDL_free(evt.drop.file);

            SendEvent(E_DROPFILE, eventData);
        }
        break;

    case SDL_QUIT:
        SendEvent(E_EXITREQUESTED);
        break;

    default: break;
    }
}

void Input::HandleScreenMode(StringHash eventType, VariantMap& eventData)
{
    if (!initialized_)
        Initialize();

    // Re-enable cursor clipping, and re-center the cursor (if needed) to the new screen size, so that there is no erroneous
    // mouse move event. Also get new window ID if it changed
    SDL_Window* window = graphics_->GetWindow();
    windowID_ = SDL_GetWindowID(window);

    if (graphics_->GetFullscreen() || !mouseVisible_)
        focusedThisFrame_ = true;

    // After setting a new screen mode we should not be minimized
    minimized_ = (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) != 0;

    // Calculate input coordinate scaling from SDL window to backbuffer ratio
    int winWidth, winHeight;
    int gfxWidth = graphics_->GetWidth();
    int gfxHeight = graphics_->GetHeight();
    SDL_GetWindowSize(window, &winWidth, &winHeight);
    if (winWidth > 0 && winHeight > 0 && gfxWidth > 0 && gfxHeight > 0)
    {
        inputScale_.x_ = (float)gfxWidth / (float)winWidth;
        inputScale_.y_ = (float)gfxHeight / (float)winHeight;
    }
    else
        inputScale_ = Vector2::ONE;
}

void Input::HandleBeginFrame(StringHash eventType, VariantMap& eventData)
{
    // Update input right at the beginning of the frame
    SendEvent(E_INPUTBEGIN);
    Update();
    SendEvent(E_INPUTEND);
}

}
