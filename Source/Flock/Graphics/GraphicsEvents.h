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

/// New screen mode set.
FLOCKSDK_EVENT(E_SCREENMODE, ScreenMode)
{
    FLOCKSDK_PARAM(P_WIDTH, Width);                  // int
    FLOCKSDK_PARAM(P_HEIGHT, Height);                // int
    FLOCKSDK_PARAM(P_FULLSCREEN, Fullscreen);        // bool
    FLOCKSDK_PARAM(P_BORDERLESS, Borderless);        // bool
    FLOCKSDK_PARAM(P_RESIZABLE, Resizable);          // bool
    FLOCKSDK_PARAM(P_HIGHDPI, HighDPI);              // bool
    FLOCKSDK_PARAM(P_MONITOR, Monitor);              // int
    FLOCKSDK_PARAM(P_REFRESHRATE, RefreshRate);      // int
}

/// Window position changed.
FLOCKSDK_EVENT(E_WINDOWPOS, WindowPos)
{
    FLOCKSDK_PARAM(P_X, X);                          // int
    FLOCKSDK_PARAM(P_Y, Y);                          // int
}

/// Request for queuing rendersurfaces either in manual or always-update mode.
FLOCKSDK_EVENT(E_RENDERSURFACEUPDATE, RenderSurfaceUpdate)
{
}

/// Frame rendering started.
FLOCKSDK_EVENT(E_BEGINRENDERING, BeginRendering)
{
}

/// Frame rendering ended.
FLOCKSDK_EVENT(E_ENDRENDERING, EndRendering)
{
}

/// Update of a view started.
FLOCKSDK_EVENT(E_BEGINVIEWUPDATE, BeginViewUpdate)
{
    FLOCKSDK_PARAM(P_VIEW, View);                    // View pointer
    FLOCKSDK_PARAM(P_TEXTURE, Texture);              // Texture pointer
    FLOCKSDK_PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// Update of a view ended.
FLOCKSDK_EVENT(E_ENDVIEWUPDATE, EndViewUpdate)
{
    FLOCKSDK_PARAM(P_VIEW, View);                    // View pointer
    FLOCKSDK_PARAM(P_TEXTURE, Texture);              // Texture pointer
    FLOCKSDK_PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// Render of a view started.
FLOCKSDK_EVENT(E_BEGINVIEWRENDER, BeginViewRender)
{
    FLOCKSDK_PARAM(P_VIEW, View);                    // View pointer
    FLOCKSDK_PARAM(P_TEXTURE, Texture);              // Texture pointer
    FLOCKSDK_PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// A view has allocated its screen buffers for rendering. They can be accessed now with View::FindNamedTexture().
FLOCKSDK_EVENT(E_VIEWBUFFERSREADY, ViewBuffersReady)
{
    FLOCKSDK_PARAM(P_VIEW, View);                    // View pointer
    FLOCKSDK_PARAM(P_TEXTURE, Texture);              // Texture pointer
    FLOCKSDK_PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// A view has set global shader parameters for a new combination of vertex/pixel shaders. Custom global parameters can now be set.
FLOCKSDK_EVENT(E_VIEWGLOBALSHADERPARAMETERS, ViewGlobalShaderParameters)
{
    FLOCKSDK_PARAM(P_VIEW, View);                    // View pointer
    FLOCKSDK_PARAM(P_TEXTURE, Texture);              // Texture pointer
    FLOCKSDK_PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// Render of a view ended. Its screen buffers are still accessible if needed.
FLOCKSDK_EVENT(E_ENDVIEWRENDER, EndViewRender)
{
    FLOCKSDK_PARAM(P_VIEW, View);                    // View pointer
    FLOCKSDK_PARAM(P_TEXTURE, Texture);              // Texture pointer
    FLOCKSDK_PARAM(P_SURFACE, Surface);              // RenderSurface pointer
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_CAMERA, Camera);                // Camera pointer
}

/// Render of all views is finished for the frame.
FLOCKSDK_EVENT(E_ENDALLVIEWSRENDER, EndAllViewsRender)
{
}

/// A render path event has occurred.
FLOCKSDK_EVENT(E_RENDERPATHEVENT, RenderPathEvent)
{
    FLOCKSDK_PARAM(P_NAME, Name);                    // String
}

/// Graphics context has been lost. Some or all (depending on the API) GPU objects have lost their contents.
FLOCKSDK_EVENT(E_DEVICELOST, DeviceLost)
{
}

/// Graphics context has been recreated after being lost. GPU objects in the "data lost" state can be restored now.
FLOCKSDK_EVENT(E_DEVICERESET, DeviceReset)
{
}

}
