local rows = 1 + globals.level
local columns = 1 + globals.level
local localBricksDestroyed = 0
local totalBricks = rows * columns

function mito.start()
    mito.spawn_prefab("explosionFX.lua")
    local totalLength = columns * 64
    for i = 1, columns do
        for j = 1, rows do
            local go = mito.spawn_prefab("brick.lua")
            go:get_component_transform().position = mito.vec2:new(
                (i - 1) * 64 + (mito.window.width - totalLength) / 2,
                j * 32 + 50
            )
            local r = math.random(50, 255)
            local g = math.random(50, 255)
            local b = math.random(50, 255)
            go:get_component_sprite().color = mito.color.new(r, g, b)
        end
    end
    mito.log("level:" .. tostring(globals.level))
end

function mito.update(gameobject, deltaTime)

end

function mito.on_brick_destroyed()
    localBricksDestroyed = localBricksDestroyed + 1
    mito.log("on_brick_destroyed")
    if localBricksDestroyed == totalBricks then
        globals.level = globals.level + 1
        mito.open_level("game.lua")
    end
end