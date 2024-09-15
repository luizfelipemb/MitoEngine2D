globals.bricks_destroyed = 0

local rows = 1
local columns = 1
function start()
    local totalLength = columns * 64
    for i = 1, columns + globals.level do
        for j = 1, rows + globals.level do
            local go = spawn_prefab("brick.lua")
            go:get_component_transform().position = vec2:new(
                (i - 1) * 64 + (Window.width - totalLength) / 2,
                j * 32 + 50
            )
            local r = math.random(50, 255)
            local g = math.random(50, 255)
            local b = math.random(50, 255)
            go:get_component_sprite().color = color.new(r, g, b)
        end
    end
    globals.level = globals.level + 1
    mito_log("level:" .. tostring(globals.level))
end

function update(gameobject, deltaTime)

end

function on_brick_destroyed()
    mito_log("on_brick_destroyed")
end