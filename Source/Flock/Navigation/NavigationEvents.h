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

/// Complete rebuild of navigation mesh.
FLOCKSDK_EVENT(E_NAVIGATION_MESH_REBUILT, NavigationMeshRebuilt)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_MESH, Mesh); // NavigationMesh pointer
}

/// Partial bounding box rebuild of navigation mesh.
FLOCKSDK_EVENT(E_NAVIGATION_AREA_REBUILT, NavigationAreaRebuilt)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_MESH, Mesh); // NavigationMesh pointer
    FLOCKSDK_PARAM(P_BOUNDSMIN, BoundsMin); // Vector3
    FLOCKSDK_PARAM(P_BOUNDSMAX, BoundsMax); // Vector3
}

/// Crowd agent formation.
FLOCKSDK_EVENT(E_CROWD_AGENT_FORMATION, CrowdAgentFormation)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    FLOCKSDK_PARAM(P_INDEX, Index); // unsigned
    FLOCKSDK_PARAM(P_SIZE, Size); // unsigned
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3 [in/out]
}

/// Crowd agent formation specific to a node.
FLOCKSDK_EVENT(E_CROWD_AGENT_NODE_FORMATION, CrowdAgentNodeFormation)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    FLOCKSDK_PARAM(P_INDEX, Index); // unsigned
    FLOCKSDK_PARAM(P_SIZE, Size); // unsigned
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3 [in/out]
}

/// Crowd agent has been repositioned.
FLOCKSDK_EVENT(E_CROWD_AGENT_REPOSITION, CrowdAgentReposition)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3
    FLOCKSDK_PARAM(P_VELOCITY, Velocity); // Vector3
    FLOCKSDK_PARAM(P_ARRIVED, Arrived); // bool
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep); // float
}

/// Crowd agent has been repositioned, specific to a node
FLOCKSDK_EVENT(E_CROWD_AGENT_NODE_REPOSITION, CrowdAgentNodeReposition)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3
    FLOCKSDK_PARAM(P_VELOCITY, Velocity); // Vector3
    FLOCKSDK_PARAM(P_ARRIVED, Arrived); // bool
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep); // float
}

/// Crowd agent's internal state has become invalidated. This is a special case of CrowdAgentStateChanged event.
FLOCKSDK_EVENT(E_CROWD_AGENT_FAILURE, CrowdAgentFailure)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3
    FLOCKSDK_PARAM(P_VELOCITY, Velocity); // Vector3
    FLOCKSDK_PARAM(P_CROWD_AGENT_STATE, CrowdAgentState); // int
    FLOCKSDK_PARAM(P_CROWD_TARGET_STATE, CrowdTargetState); // int
}

/// Crowd agent's internal state has become invalidated. This is a special case of CrowdAgentStateChanged event.
FLOCKSDK_EVENT(E_CROWD_AGENT_NODE_FAILURE, CrowdAgentNodeFailure)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3
    FLOCKSDK_PARAM(P_VELOCITY, Velocity); // Vector3
    FLOCKSDK_PARAM(P_CROWD_AGENT_STATE, CrowdAgentState); // int
    FLOCKSDK_PARAM(P_CROWD_TARGET_STATE, CrowdTargetState); // int
}

/// Crowd agent's state has been changed.
FLOCKSDK_EVENT(E_CROWD_AGENT_STATE_CHANGED, CrowdAgentStateChanged)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3
    FLOCKSDK_PARAM(P_VELOCITY, Velocity); // Vector3
    FLOCKSDK_PARAM(P_CROWD_AGENT_STATE, CrowdAgentState); // int
    FLOCKSDK_PARAM(P_CROWD_TARGET_STATE, CrowdTargetState); // int
}

/// Crowd agent's state has been changed.
FLOCKSDK_EVENT(E_CROWD_AGENT_NODE_STATE_CHANGED, CrowdAgentNodeStateChanged)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_CROWD_AGENT, CrowdAgent); // CrowdAgent pointer
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3
    FLOCKSDK_PARAM(P_VELOCITY, Velocity); // Vector3
    FLOCKSDK_PARAM(P_CROWD_AGENT_STATE, CrowdAgentState); // int
    FLOCKSDK_PARAM(P_CROWD_TARGET_STATE, CrowdTargetState); // int
}

/// Addition of obstacle to dynamic navigation mesh.
FLOCKSDK_EVENT(E_NAVIGATION_OBSTACLE_ADDED, NavigationObstacleAdded)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_OBSTACLE, Obstacle); // Obstacle pointer
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3
    FLOCKSDK_PARAM(P_RADIUS, Radius); // float
    FLOCKSDK_PARAM(P_HEIGHT, Height); // float
}

/// Removal of obstacle from dynamic navigation mesh.
FLOCKSDK_EVENT(E_NAVIGATION_OBSTACLE_REMOVED, NavigationObstacleRemoved)
{
    FLOCKSDK_PARAM(P_NODE, Node); // Node pointer
    FLOCKSDK_PARAM(P_OBSTACLE, Obstacle); // Obstacle pointer
    FLOCKSDK_PARAM(P_POSITION, Position); // Vector3
    FLOCKSDK_PARAM(P_RADIUS, Radius); // float
    FLOCKSDK_PARAM(P_HEIGHT, Height); // float
}

}
