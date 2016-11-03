package.path = package.path .. ";dlls/?.lua"
package.cpath = package.cpath .. ";dlls/?.dll"

require("./downpour-io")
require("./downpour-io-parser")

require("./downpour-md5")
require("./downpour-math")

require("./downpour-os")
require("./downpour-os-win32")
require("./downpour-os-linux")
require("./downpour-os-osx")
require("./downpour-os-dummy")

local downpour = require("./downpour")
local downpour_u3d = require("./downpour-u3d")

require("./downpour-u3d-window")

local core = require("./core")

require("./core-i18n")

require("./core-sound")

require("./core-save-load-base")
require("./core-save")
require("./core-load")

require("./core-scene")

require("./core-ui")

require("./core-break")

require("./core-logic-base")

require("./core-front")

require("./core-mod-api")

local debug = require("./development/core-debug")

 table = {

     ["list"] = {
         ["arguments"] = {
             ["play"] = function (plist)
                 local i = 1
                 for i = 1, #plist, 1 do
                     print("plist[" .. i .. " = [" .. plist[i])
                 end
             end
             }
         },

        ["wait"] = {
            ["play"] = function (param)
                print("Khoi waited " .. param[1] .. " minutes.")
            end
        },

        ["walk"] = {
            ["forward"] = {
                ["play"] = function (param)
                   print("Khoi walked forward " .. param[1] .. " times.")
                end
            },

            ["backward"] = {
                ["play"] = function (param)
                print("Khoi walked backwards " .. param[1] .. " times.")
                end
            }
        },

        ["go"] = {
            ["basement"] = {
                ["play"] = function (param)
                    print("Khoi went to the basement for some food.")
                end
            },

            ["garden"] = {
                ["play"] = function (param)
                    print("Khoi went to the garden to water the flowers.");
                end
            },

            ["living_room"] = {
                ["play"] = function (param)
                    print("Khoi went to the living room.");
                end
            }
        }
    }

    function DeepPrint (e)
    -- if e is a table, we should iterate over its elements
    if type(e) == "table" then
        for k,v in pairs(e) do -- for every element in the table
            print(k)
            DeepPrint(v)       -- recursively repeat the same procedure
        end
    else -- if not, we can just print it
        print(e)
    end
end 

function Start()

    --[[
    local log = downpour.ioutils.file.log

    downpour.ioutils.safe(true)
    downpour.ioutils.parser.safe(true)

    print(log.is_valid())
    local b = log.new()
    print(log.is_valid())

    local str = ""
    local parse = downpour.ioutils.parser.parse

    str = "walk forward 5"
    parse(str, table, "table")

    str = "walk backward 42"
    parse(str, table, "table")

    str = "go basement"
    parse(str, table, "table")

    str = "wait 10"
    parse(str, table, "table")

    str = "list arguments 10 20 30 40 42 50 62 72 88 91 101"
    parse(str, table, "table")]]--

    -- log.free()

    --[[
    downpour.osutils.safe(true)
    downpour.osutils.init()
    ]]--

    downpour_u3d.window.set_title("New title set within Lua script.")

    -- downpour.osutils.alert("Sample title", "Sample string")

    -- Execute the common startup for samples
    SampleStart()

    -- Create the scene content
    CreateScene()

    -- Create the UI content
    CreateUI()

    -- Setup the viewport for displaying the scene
    SetupViewport()

    -- Set the mouse mode to use in the sample
    SampleInitMouseMode(MM_RELATIVE)

    -- Subscribe to global events for camera movement
    SubscribeToEvents()

    downpour_u3d.window.set_resolution(1366, 768)
    downpour_u3d.window.set_fullscreen(false)

    downpour_u3d.window.set_icon("textures/IWBHTIcon.png")

    debug.init()
end

function CreateScene()
    scene_ = Scene()

    -- Load scene content prepared in the editor (XML format). GetFile() returns an open file from the resource system
    -- which scene.LoadXML() will read
    local file = cache:GetFile("levels/scene.xml")
    scene_:LoadXML(file)
    -- In Lua the file returned by GetFile() needs to be deleted manually
    file:delete()

    -- Create the camera (not included in the scene file)
    cameraNode = scene_:CreateChild("Camera")
    cameraNode:CreateComponent("Camera")

    -- Set an initial position for the camera scene node above the plane
    cameraNode.position = Vector3(0.0, 2.0, -10.0)
end

function CreateUI()
    -- Set up global UI style into the root UI element
    local style = cache:GetResource("XMLFile", "ui/SDK/DefaultStyle.xml")
    ui.root.defaultStyle = style

    -- Create a Cursor UI element because we want to be able to hide and show it at will. When hidden, the mouse cursor will
    -- control the camera, and when visible, it will interact with the UI
    local cursor = ui.root:CreateChild("Cursor")
    cursor:SetStyleAuto()
    ui.cursor = cursor
    -- Set starting position of the cursor at the rendering window center
    cursor:SetPosition(graphics.width / 2, graphics.height / 2)

    -- Load UI content prepared in the editor and add to the UI hierarchy
    local layoutRoot = ui:LoadLayout(cache:GetResource("XMLFile", "ui/UILoadExample.xml"))
    ui.root:AddChild(layoutRoot)

    -- Subscribe to button actions (toggle scene lights when pressed then released)
    local button = layoutRoot:GetChild("ToggleLight1", true)
    if button ~= nil then
        SubscribeToEvent(button, "Released", "ToggleLight1")
    end
    button = layoutRoot:GetChild("ToggleLight2", true)
    if button ~= nil then
        SubscribeToEvent(button, "Released", "ToggleLight2")
    end
end

function ToggleLight1()
    local lightNode = scene_:GetChild("Light1", true)
    if lightNode  ~= nil then
        lightNode.enabled = not lightNode.enabled
    end

    downpour_u3d.window.set_fullscreen(true)
end

function ToggleLight2()
    local lightNode = scene_:GetChild("Light2", true)
    if lightNode  ~= nil then
        lightNode.enabled = not lightNode.enabled
    end

    downpour_u3d.window.set_fullscreen(false)
end

function SetupViewport()
    -- Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
    local viewport = Viewport:new(scene_, cameraNode:GetComponent("Camera"))
    renderer:SetViewport(0, viewport)
end

function SubscribeToEvents()
    -- Subscribe HandleUpdate() function for camera motion
    SubscribeToEvent("Update", "HandleUpdate")
end

function HandleUpdate(eventType, eventData)
    -- Take the frame time step, which is stored as a float
    local timeStep = eventData["TimeStep"]:GetFloat()

    -- Move the camera, scale movement with time step
    MoveCamera(timeStep)
end

function MoveCamera(timeStep)
    input.mouseVisible = input.mouseMode ~= MM_RELATIVE
    mouseDown = input:GetMouseButtonDown(MOUSEB_RIGHT)

    -- Override the MM_RELATIVE mouse grabbed settings, to allow interaction with UI
    input.mouseGrabbed = mouseDown

    -- Right mouse button controls mouse cursor visibility: hide when pressed
    ui.cursor.visible = not mouseDown

    -- Do not move if the UI has a focused element
    if ui.focusElement ~= nil then
        return
    end

    -- Movement speed as world units per second
    local MOVE_SPEED = 20.0
    -- Mouse sensitivity as degrees per pixel
    local MOUSE_SENSITIVITY = 0.1

    -- Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
    -- Only move the camera when the cursor is hidden
    if not ui.cursor.visible then
        local mouseMove = input.mouseMove
        yaw = yaw + MOUSE_SENSITIVITY * mouseMove.x
        pitch = pitch + MOUSE_SENSITIVITY * mouseMove.y
        pitch = Clamp(pitch, -90.0, 90.0)

        -- Construct new orientation for the camera scene node from yaw and pitch. Roll is fixed to zero
        cameraNode.rotation = Quaternion(pitch, yaw, 0.0)
    end

    -- Read WASD keys and move the camera scene node to the corresponding direction if they are pressed
    if input:GetKeyDown(KEY_W) then
        cameraNode:Translate(Vector3(0.0, 0.0, 1.0) * MOVE_SPEED * timeStep)
    end
    if input:GetKeyDown(KEY_S) then
        cameraNode:Translate(Vector3(0.0, 0.0, -1.0) * MOVE_SPEED * timeStep)
    end
    if input:GetKeyDown(KEY_A) then
        cameraNode:Translate(Vector3(-1.0, 0.0, 0.0) * MOVE_SPEED * timeStep)
    end
    if input:GetKeyDown(KEY_D) then
        cameraNode:Translate(Vector3(1.0, 0.0, 0.0) * MOVE_SPEED * timeStep)
    end
end
