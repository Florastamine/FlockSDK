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

/// Variable timestep scene update.
FLOCKSDK_EVENT(E_SCENEUPDATE, SceneUpdate)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Scene subsystem update.
FLOCKSDK_EVENT(E_SCENESUBSYSTEMUPDATE, SceneSubsystemUpdate)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Scene transform smoothing update.
FLOCKSDK_EVENT(E_UPDATESMOOTHING, UpdateSmoothing)
{
    FLOCKSDK_PARAM(P_CONSTANT, Constant);            // float
    FLOCKSDK_PARAM(P_SQUAREDSNAPTHRESHOLD, SquaredSnapThreshold);  // float
}

/// Scene drawable update finished. Custom animation (eg. IK) can be done at this point.
FLOCKSDK_EVENT(E_SCENEDRAWABLEUPDATEFINISHED, SceneDrawableUpdateFinished)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep);            // float
}

/// SmoothedTransform target position changed.
FLOCKSDK_EVENT(E_TARGETPOSITION, TargetPositionChanged)
{
}

/// SmoothedTransform target position changed.
FLOCKSDK_EVENT(E_TARGETROTATION, TargetRotationChanged)
{
}

/// Scene attribute animation update.
FLOCKSDK_EVENT(E_ATTRIBUTEANIMATIONUPDATE, AttributeAnimationUpdate)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Attribute animation added to object animation.
FLOCKSDK_EVENT(E_ATTRIBUTEANIMATIONADDED, AttributeAnimationAdded)
{
    FLOCKSDK_PARAM(P_OBJECTANIMATION, ObjectAnimation);               // Object animation pointer
    FLOCKSDK_PARAM(P_ATTRIBUTEANIMATIONNAME, AttributeAnimationName); // String
}

/// Attribute animation removed from object animation.
FLOCKSDK_EVENT(E_ATTRIBUTEANIMATIONREMOVED, AttributeAnimationRemoved)
{
    FLOCKSDK_PARAM(P_OBJECTANIMATION, ObjectAnimation);               // Object animation pointer
    FLOCKSDK_PARAM(P_ATTRIBUTEANIMATIONNAME, AttributeAnimationName); // String
}

/// Variable timestep scene post-update.
FLOCKSDK_EVENT(E_SCENEPOSTUPDATE, ScenePostUpdate)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Asynchronous scene loading progress.
FLOCKSDK_EVENT(E_ASYNCLOADPROGRESS, AsyncLoadProgress)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_PROGRESS, Progress);            // float
    FLOCKSDK_PARAM(P_LOADEDNODES, LoadedNodes);      // int
    FLOCKSDK_PARAM(P_TOTALNODES, TotalNodes);        // int
    FLOCKSDK_PARAM(P_LOADEDRESOURCES, LoadedResources); // int
    FLOCKSDK_PARAM(P_TOTALRESOURCES, TotalResources);   // int
};

/// Asynchronous scene loading finished.
FLOCKSDK_EVENT(E_ASYNCLOADFINISHED, AsyncLoadFinished)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
};

/// A child node has been added to a parent node.
FLOCKSDK_EVENT(E_NODEADDED, NodeAdded)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_PARENT, Parent);                // Node pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
}

/// A child node is about to be removed from a parent node.
FLOCKSDK_EVENT(E_NODEREMOVED, NodeRemoved)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_PARENT, Parent);                // Node pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
}

/// A component has been created to a node.
FLOCKSDK_EVENT(E_COMPONENTADDED, ComponentAdded)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
    FLOCKSDK_PARAM(P_COMPONENT, Component);          // Component pointer
}

/// A component is about to be removed from a node.
FLOCKSDK_EVENT(E_COMPONENTREMOVED, ComponentRemoved)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
    FLOCKSDK_PARAM(P_COMPONENT, Component);          // Component pointer
}

/// A node's name has changed.
FLOCKSDK_EVENT(E_NODENAMECHANGED, NodeNameChanged)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
}

/// A node's enabled state has changed.
FLOCKSDK_EVENT(E_NODEENABLEDCHANGED, NodeEnabledChanged)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
}

/// A node's tag has been added.
FLOCKSDK_EVENT(E_NODETAGADDED, NodeTagAdded)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
    FLOCKSDK_PARAM(P_TAG, Tag);                      // String tag
}

/// A node's tag has been removed.
FLOCKSDK_EVENT(E_NODETAGREMOVED, NodeTagRemoved)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
    FLOCKSDK_PARAM(P_TAG, Tag);                      // String tag
}

/// A component's enabled state has changed.
FLOCKSDK_EVENT(E_COMPONENTENABLEDCHANGED, ComponentEnabledChanged)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
    FLOCKSDK_PARAM(P_COMPONENT, Component);          // Component pointer
}

/// A serializable's temporary state has changed.
FLOCKSDK_EVENT(E_TEMPORARYCHANGED, TemporaryChanged)
{
    FLOCKSDK_PARAM(P_SERIALIZABLE, Serializable);    // Serializable pointer
}

/// A node (and its children and components) has been cloned.
FLOCKSDK_EVENT(E_NODECLONED, NodeCloned)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_NODE, Node);                    // Node pointer
    FLOCKSDK_PARAM(P_CLONENODE, CloneNode);          // Node pointer
}

/// A component has been cloned.
FLOCKSDK_EVENT(E_COMPONENTCLONED, ComponentCloned)
{
    FLOCKSDK_PARAM(P_SCENE, Scene);                  // Scene pointer
    FLOCKSDK_PARAM(P_COMPONENT, Component);          // Component pointer
    FLOCKSDK_PARAM(P_CLONECOMPONENT, CloneComponent); // Component pointer
}

/// A network attribute update from the server has been intercepted.
FLOCKSDK_EVENT(E_INTERCEPTNETWORKUPDATE, InterceptNetworkUpdate)
{
    FLOCKSDK_PARAM(P_SERIALIZABLE, Serializable);    // Serializable pointer
    FLOCKSDK_PARAM(P_TIMESTAMP, TimeStamp);          // unsigned (0-255)
    FLOCKSDK_PARAM(P_INDEX, Index);                  // unsigned
    FLOCKSDK_PARAM(P_NAME, Name);                    // String
    FLOCKSDK_PARAM(P_VALUE, Value);                  // Variant
}

}
