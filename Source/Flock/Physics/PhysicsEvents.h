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

/// Physics world is about to be stepped.
FLOCKSDK_EVENT(E_PHYSICSPRESTEP, PhysicsPreStep)
{
    FLOCKSDK_PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Physics world has been stepped.
FLOCKSDK_EVENT(E_PHYSICSPOSTSTEP, PhysicsPostStep)
{
    FLOCKSDK_PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    FLOCKSDK_PARAM(P_TIMESTEP, TimeStep);            // float
}

/// Physics collision started. Global event sent by the PhysicsWorld.
FLOCKSDK_EVENT(E_PHYSICSCOLLISIONSTART, PhysicsCollisionStart)
{
    FLOCKSDK_PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    FLOCKSDK_PARAM(P_NODEA, NodeA);                  // Node pointer
    FLOCKSDK_PARAM(P_NODEB, NodeB);                  // Node pointer
    FLOCKSDK_PARAM(P_BODYA, BodyA);                  // RigidBody pointer
    FLOCKSDK_PARAM(P_BODYB, BodyB);                  // RigidBody pointer
    FLOCKSDK_PARAM(P_TRIGGER, Trigger);              // bool
    FLOCKSDK_PARAM(P_CONTACTS, Contacts);            // Buffer containing position (Vector3), normal (Vector3), distance (float), impulse (float) for each contact
}

/// Physics collision ongoing. Global event sent by the PhysicsWorld.
FLOCKSDK_EVENT(E_PHYSICSCOLLISION, PhysicsCollision)
{
    FLOCKSDK_PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    FLOCKSDK_PARAM(P_NODEA, NodeA);                  // Node pointer
    FLOCKSDK_PARAM(P_NODEB, NodeB);                  // Node pointer
    FLOCKSDK_PARAM(P_BODYA, BodyA);                  // RigidBody pointer
    FLOCKSDK_PARAM(P_BODYB, BodyB);                  // RigidBody pointer
    FLOCKSDK_PARAM(P_TRIGGER, Trigger);              // bool
    FLOCKSDK_PARAM(P_CONTACTS, Contacts);            // Buffer containing position (Vector3), normal (Vector3), distance (float), impulse (float) for each contact
}

/// Physics collision ended. Global event sent by the PhysicsWorld.
FLOCKSDK_EVENT(E_PHYSICSCOLLISIONEND, PhysicsCollisionEnd)
{
    FLOCKSDK_PARAM(P_WORLD, World);                  // PhysicsWorld pointer
    FLOCKSDK_PARAM(P_NODEA, NodeA);                  // Node pointer
    FLOCKSDK_PARAM(P_NODEB, NodeB);                  // Node pointer
    FLOCKSDK_PARAM(P_BODYA, BodyA);                  // RigidBody pointer
    FLOCKSDK_PARAM(P_BODYB, BodyB);                  // RigidBody pointer
    FLOCKSDK_PARAM(P_TRIGGER, Trigger);              // bool
}

/// Node's physics collision started. Sent by scene nodes participating in a collision.
FLOCKSDK_EVENT(E_NODECOLLISIONSTART, NodeCollisionStart)
{
    FLOCKSDK_PARAM(P_BODY, Body);                    // RigidBody pointer
    FLOCKSDK_PARAM(P_OTHERNODE, OtherNode);          // Node pointer
    FLOCKSDK_PARAM(P_OTHERBODY, OtherBody);          // RigidBody pointer
    FLOCKSDK_PARAM(P_TRIGGER, Trigger);              // bool
    FLOCKSDK_PARAM(P_CONTACTS, Contacts);            // Buffer containing position (Vector3), normal (Vector3), distance (float), impulse (float) for each contact
}

/// Node's physics collision ongoing. Sent by scene nodes participating in a collision.
FLOCKSDK_EVENT(E_NODECOLLISION, NodeCollision)
{
    FLOCKSDK_PARAM(P_BODY, Body);                    // RigidBody pointer
    FLOCKSDK_PARAM(P_OTHERNODE, OtherNode);          // Node pointer
    FLOCKSDK_PARAM(P_OTHERBODY, OtherBody);          // RigidBody pointer
    FLOCKSDK_PARAM(P_TRIGGER, Trigger);              // bool
    FLOCKSDK_PARAM(P_CONTACTS, Contacts);            // Buffer containing position (Vector3), normal (Vector3), distance (float), impulse (float) for each contact
}

/// Node's physics collision ended. Sent by scene nodes participating in a collision.
FLOCKSDK_EVENT(E_NODECOLLISIONEND, NodeCollisionEnd)
{
    FLOCKSDK_PARAM(P_BODY, Body);                    // RigidBody pointer
    FLOCKSDK_PARAM(P_OTHERNODE, OtherNode);          // Node pointer
    FLOCKSDK_PARAM(P_OTHERBODY, OtherBody);          // RigidBody pointer
    FLOCKSDK_PARAM(P_TRIGGER, Trigger);              // bool
}

}
