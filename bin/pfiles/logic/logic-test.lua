package.path = package.path .. ";../pfiles/?.lua"

local logic = require("./core-logic-base")

CAMERA_MIN_DIST = 1.0
CAMERA_MAX_DIST = 20.0

cameraDistance = 5.0

CTRL_FORWARD = 1
CTRL_BACK = 2
CTRL_LEFT = 4
CTRL_RIGHT = 8
local CTRL_JUMP = 16

local MOVE_FORCE = 0.8
local INAIR_MOVE_FORCE = 0.02
local BRAKE_FORCE = 0.2
local JUMP_FORCE = 7.0
local YAW_SENSITIVITY = 0.1
local INAIR_THRESHOLD_TIME = 0.1
firstPerson = false -- First person camera flag

local characterNode = nil

function HandleUpdate(eventType, eventData)
    if characterNode == nil then
        return
    end

    local character = characterNode:GetScriptObject()
    if character == nil then
        return
    end

    -- Clear previous controls
    character.controls:Set(CTRL_FORWARD + CTRL_BACK + CTRL_LEFT + CTRL_RIGHT + CTRL_JUMP, false)

    -- Update controls using keys
    if ui.focusElement == nil then
        if input:GetKeyDown(KEY_W) then character.controls:Set(CTRL_FORWARD, true) end
        if input:GetKeyDown(KEY_S) then character.controls:Set(CTRL_BACK, true) end
        if input:GetKeyDown(KEY_A) then character.controls:Set(CTRL_LEFT, true) end
        if input:GetKeyDown(KEY_D) then character.controls:Set(CTRL_RIGHT, true) end
        if input:GetKeyDown(KEY_SPACE) then character.controls:Set(CTRL_JUMP, true) end

        -- Add character yaw & pitch from the mouse motion or touch input
        character.controls.yaw = character.controls.yaw + input.mouseMoveX * YAW_SENSITIVITY
        character.controls.pitch = character.controls.pitch + input.mouseMoveY * YAW_SENSITIVITY

        -- Limit pitch
        character.controls.pitch = Clamp(character.controls.pitch, -80.0, 80.0)
        -- Set rotation already here so that it's updated every rendering frame instead of every physics frame
        characterNode.rotation = Quaternion(character.controls.yaw, Vector3(0.0, 1.0, 0.0))

        -- Switch between 1st and 3rd person
        if input:GetKeyPress(KEY_F) then
            firstPerson = not firstPerson
        end
    end
end

function HandlePostUpdate(eventType, eventData)
    if characterNode == nil then
        return
    end

    local character = characterNode:GetScriptObject()
    if character == nil then
        return
    end

    -- Get camera lookat dir from character yaw + pitch
    local rot = characterNode.rotation
    local dir = rot * Quaternion(character.controls.pitch, Vector3(1.0, 0.0, 0.0))

    -- Turn head to camera pitch, but limit to avoid unnatural animation
    local headNode = characterNode:GetChild("Bip01_Head", true)
    local limitPitch = Clamp(character.controls.pitch, -45.0, 45.0)
    local headDir = rot * Quaternion(limitPitch, Vector3(1.0, 0.0, 0.0))
    -- This could be expanded to look at an arbitrary target, now just look at a point in front
    local headWorldTarget = headNode.worldPosition + headDir * Vector3(0.0, 0.0, 1.0)
    headNode:LookAt(headWorldTarget, Vector3(0.0, 1.0, 0.0))
    -- Correct head orientation because LookAt assumes Z = forward, but the bone has been authored differently (Y = forward)
    headNode:Rotate(Quaternion(0.0, 90.0, 90.0))

    --[[
    if firstPerson then
        -- First person camera: position to the head bone + offset slightly forward & up
        cameraNode.position = headNode.worldPosition + rot * Vector3(0.0, 0.15, 0.2)
        cameraNode.rotation = dir
    else
        -- Third person camera: position behind the character
        local aimPoint = characterNode.position + rot * Vector3(0.0, 1.7, 0.0) -- You can modify x Vector3 value to translate the fixed character position (indicative range[-2;2])

        -- Collide camera ray with static physics objects (layer bitmask 2) to ensure we see the character properly
        local rayDir = dir * Vector3(0.0, 0.0, -1.0) -- For indoor scenes you can use dir * Vector3(0.0, 0.0, -0.5) to prevent camera from crossing the walls
        local rayDistance = cameraDistance
        local result = scene_:GetComponent("PhysicsWorld"):RaycastSingle(Ray(aimPoint, rayDir), rayDistance, 2)
        if result.body ~= nil then
            rayDistance = Min(rayDistance, result.distance)
        end
        rayDistance = Clamp(rayDistance, CAMERA_MIN_DIST, cameraDistance)

        cameraNode.position = aimPoint + rayDir * rayDistance
        cameraNode.rotation = dir
    end
    ]]--
end

-- Character script object class
cCharacter = ScriptObject()

function Character:Start()
    -- Character controls.
    self.controls = Controls()
    -- Grounded flag for movement.
    self.onGround = false
    -- Jump flag.
    self.okToJump = true
    -- In air timer. Due to possible physics inaccuracy, character can be off ground for max. 1/10 second and still be allowed to move.
    self.inAirTimer = 0.0

    self:SubscribeToEvent(self.node, "NodeCollision", "Character:HandleNodeCollision")
end

function Character:HandleNodeCollision(eventType, eventData)
    local contacts = eventData["Contacts"]:GetBuffer()

    while not contacts.eof do
        local contactPosition = contacts:ReadVector3()
        local contactNormal = contacts:ReadVector3()
        local contactDistance = contacts:ReadFloat()
        local contactImpulse = contacts:ReadFloat()

        -- If contact is below node center and mostly vertical, assume it's a ground contact
        if contactPosition.y < self.node.position.y + 1.0 then
            local level = Abs(contactNormal.y)
            if level > 0.75 then
                self.onGround = true
            end
        end
    end
end

function Character:FixedUpdate(timeStep)
    -- Could cache the components for faster access instead of finding them each frame
    local body = self.node:GetComponent("RigidBody")
    local animCtrl = self.node:GetComponent("AnimationController")

    -- Update the in air timer. Reset if grounded
    if not self.onGround then
        self.inAirTimer = self.inAirTimer + timeStep
    else
        self.inAirTimer = 0.0
    end
    -- When character has been in air less than 1/10 second, it's still interpreted as being on ground
    local softGrounded = self.inAirTimer < INAIR_THRESHOLD_TIME

    -- Update movement & animation
    local rot = self.node.rotation
    local moveDir = Vector3(0.0, 0.0, 0.0)
    local velocity = body.linearVelocity
    -- Velocity on the XZ plane
    local planeVelocity = Vector3(velocity.x, 0.0, velocity.z)

    if self.controls:IsDown(CTRL_FORWARD) then
        moveDir = moveDir + Vector3(0.0, 0.0, 1.0)
    end
    if self.controls:IsDown(CTRL_BACK) then
        moveDir = moveDir + Vector3(0.0, 0.0, -1.0)
    end
    if self.controls:IsDown(CTRL_LEFT) then
        moveDir = moveDir + Vector3(-1.0, 0.0, 0.0)
    end
    if self.controls:IsDown(CTRL_RIGHT) then
        moveDir = moveDir + Vector3(1.0, 0.0, 0.0)
    end

    -- Normalize move vector so that diagonal strafing is not faster
    if moveDir:LengthSquared() > 0.0 then
        moveDir:Normalize()
    end

    -- If in air, allow control, but slower than when on ground
    if softGrounded then
        body:ApplyImpulse(rot * moveDir * MOVE_FORCE)
    else
        body:ApplyImpulse(rot * moveDir * INAIR_MOVE_FORCE)
    end

    if softGrounded then
        -- When on ground, apply a braking force to limit maximum ground velocity
        local brakeForce = planeVelocity * -BRAKE_FORCE
        body:ApplyImpulse(brakeForce)

        -- Jump. Must release jump control inbetween jumps
        if self.controls:IsDown(CTRL_JUMP) then
            if self.okToJump then
                body:ApplyImpulse(Vector3(0.0, 1.0, 0.0) * JUMP_FORCE)
                self.okToJump = false
            end
        else
            self.okToJump = true
        end
    end

    -- Play walk animation if moving on ground, otherwise fade it out
    if softGrounded and not moveDir:Equals(Vector3(0.0, 0.0, 0.0)) then
        animCtrl:PlayExclusive("objects/Jack_Walk.ani", 0, true, 0.2)
    else
        animCtrl:Stop("objects/Jack_Walk.ani", 0.2)
    end
    -- Set walk animation speed proportional to velocity
    animCtrl:SetSpeed("objects/Jack_Walk.ani", planeVelocity:Length() * 0.3)

    -- Reset grounded flag for next frame
    self.onGround = false
end
