local goRight = false
local goLeft = false

function start(gameobject)
    mito_log("Start player function called on "..gameobject:get_name())
end

function update(gameobject,deltaTime)
    local transform = gameobject:get_component_transform()
    if (goRight) then
        transform.position.x = transform.position.x + deltaTime * 300;
    else
        if (goLeft) then
            transform.position.x = transform.position.x - deltaTime * 300;
        end
    end
end

function on_key_pressed(gameobject, key)
    if (key == "97") then
        goLeft = true
    end
    if (key == "100")then
        goRight = true
    end
    mito_log(gameobject:get_name().." with key "..key)
end

function on_key_released(gameobject, key)
    if (key == "97") then
        goLeft = false
    end
    if (key == "100")then
        goRight = false
    end
    mito_log(gameobject:get_name().." with key released "..key)
end

function on_collision_enter(gameobject, other)
    mito_log(gameobject:get_name().." collided with "..other:get_name())
end