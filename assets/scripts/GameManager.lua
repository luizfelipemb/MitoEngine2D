globals.bricks_destroyed = 0

function start()
    for i = 1, 8 do
        local go = create("brick_" .. tostring(i))
        go:add_component_transform(vec2:new(i*64, 100))
        go:add_component_sprite("assets/images/brick.png")
        go:add_component_boxcollider()
        go:add_component_script("brick.lua")
    end
end

function update(gameobject, deltaTime)
    mito_log(tostring(globals.bricks_destroyed))
end