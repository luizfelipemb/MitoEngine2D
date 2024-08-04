local goRight = false
local goLeft = false

function start(gameobject)
    mito_log("Start player function called on " .. gameobject:get_name())
    mito_log("Window height:" .. Window.height)
end

function update(gameobject, deltaTime)
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
        local go = create("name")
        mito_log(tostring(go))
        go:add_component_transform(vec2:new(100, 100))
        go:add_component_sprite("assets/images/RoundedSquare.png", 100, 100)
        mito_log("created in lua, object named:" .. go:get_name())
        goLeft = true
    end
    if (key == "100") then
        goRight = true
    end
    mito_log(gameobject:get_name() .. " with key " .. key)
end

function on_key_released(gameobject, key)
    if (key == "97") then
        goLeft = false
    end
    if (key == "100") then
        goRight = false
    end
    mito_log(gameobject:get_name() .. " with key released " .. key)
end

function on_collision_enter(gameobject, other)
    mito_log(gameobject:get_name() .. " collided with " .. other:get_name())
    local startFunc = other:get_component_script():get_function("test")
    startFunc()
end
