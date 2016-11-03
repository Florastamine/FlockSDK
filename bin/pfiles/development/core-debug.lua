--[[
Copyright (c) 2016, Florastamine

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------
This standalone module contains debugging functionalities and utilities when developing IWBHT.
It's intended to be used for development purposes.
------------------------------------------
]]--

debug = {}

debug["init"] = function ()
    -- Get default style
    local uiStyle = cache:GetResource("XMLFile", "ui/SDK/DefaultStyle.xml")
    if uiStyle ~= nil then
        io.stdout:write("Logging module loaded.")
    end

    -- Create console
    engine:CreateConsole()
    console.defaultStyle = uiStyle
    console.background.opacity = 0.8
    console:SetVisible(true)

    -- Create debug HUD
    engine:CreateDebugHud()
    debugHud.defaultStyle = uiStyle
end

debug["register_keys"] = function(evt, evd)
    local key = evd["Key"]:GetInt()

    if key == KEY_F1 then
        console:Toggle()

    elseif key == KEY_F2 then
        debugHud:ToggleAll()

    elseif ui.focusElement == nil then
        -- Preferences / Pause
        if key == KEY_SELECT then
            paused = not paused

        -- Texture quality
        elseif key == KEY_1 then
            local quality = renderer.textureQuality
            quality = quality + 1
            if quality > QUALITY_HIGH then
                quality = QUALITY_LOW
            end
            renderer.textureQuality = quality

        -- Material quality
        elseif key == KEY_2 then
            local quality = renderer.materialQuality
            quality = quality + 1
            if quality > QUALITY_HIGH then
                quality = QUALITY_LOW
            end
            renderer.materialQuality = quality

        -- Specular lighting
        elseif key == KEY_3 then
            renderer.specularLighting = not renderer.specularLighting

        -- Shadow rendering
        elseif key == KEY_4 then
            renderer.drawShadows = not renderer.drawShadows

        -- Shadow map resolution
        elseif key == KEY_5 then
            local shadowMapSize = renderer.shadowMapSize
            shadowMapSize = shadowMapSize * 2
            if shadowMapSize > 2048 then
                shadowMapSize = 512
            end
            renderer.shadowMapSize = shadowMapSize

        -- Shadow depth and filtering quality
        elseif key == KEY_6 then
            local quality = renderer.shadowQuality
            quality = quality + 1
            if quality > SHADOWQUALITY_BLUR_VSM then
                quality = SHADOWQUALITY_SIMPLE_16BIT
            end
            renderer.shadowQuality = quality

        -- Occlusion culling
        elseif key == KEY_7 then
            local occlusion = renderer.maxOccluderTriangles > 0
            occlusion = not occlusion
            if occlusion then
                renderer.maxOccluderTriangles = 5000
            else
                renderer.maxOccluderTriangles = 0
            end

        -- Instancing
        elseif key == KEY_8 then
            renderer.dynamicInstancing = not renderer.dynamicInstancing

        -- Take screenshot
        elseif key == KEY_9 then
            local screenshot = Image()
            graphics:TakeScreenShot(screenshot)
            local timeStamp = Time:GetTimeStamp()
            timeStamp = string.gsub(timeStamp, "[:. ]", "_")
            -- Here we save in the Data folder with date and time appended
            screenshot:SavePNG(fileSystem:GetProgramDir() .. "Data/Screenshot_" .. timeStamp .. ".png")
        end
    end
end

return debug
