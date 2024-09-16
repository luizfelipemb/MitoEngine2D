globals.bricks_destroyed = 0
local rows = 2
local columns = 2
globals.max_bricks = rows + columns

function start()
    for i = 1, columns do
        for j = 1, rows do
            local go = spawn_prefab("brick.lua")
            go:get_component_transform().position = vec2:new(i * 64, j * 32 + 50)
            --local r = math.random(50,255)
            --local g = math.random(50,255)
            --local b = math.random(50,255)
            --go:add_component_sprite("assets/images/brick.png",64,32,r,g,b)    
            --TODO: create above function in lua
        end
    end
end

function update(gameobject, deltaTime)
    
end
