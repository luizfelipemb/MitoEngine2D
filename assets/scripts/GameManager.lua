
function start()
    for i = 1, 5 do
        mito_log(tostring(i))
        local go = create("brick_" .. tostring(i))
        go:add_component_transform(vec2:new(i*64, 100))
        go:add_component_sprite("assets/images/brick.png")
        go:add_component_boxcollider()
        go:add_component_script("brick.lua")
    end
    
end

function update(gameobject, deltaTime)
    --mito_log(go:get_name())
end