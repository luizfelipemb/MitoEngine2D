﻿globals.bricks_destroyed = 0

function start()
    for i = 1, 6 do
        for j = 1, 3 do
            local go = create("brick_" .. tostring(i))
            go:add_component_transform(vec2:new(i * 64, j * 32+50))
            go:add_component_sprite("assets/images/brick.png")
            go:add_component_boxcollider()
            go:add_component_script("brick.lua")
        end
    end
end

function update(gameobject, deltaTime)
    
end
