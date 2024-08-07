local goingUp = false

function start(gameobject)
    mito_log(gameobject:get_name() .. "start called")
end

function update(gameobject, deltaTime)
    local rigidbody = gameobject:get_component_rigidbody()
    if goingUp then
        rigidbody.velocity = vec2:new(100, -200)
    else
        rigidbody.velocity = vec2:new(100, 200)
    end
end

function on_collision_enter(gameobject, other)
    mito_log(other:get_name())
    if other:get_name() == "player" then
        goingUp = true
    end
end

function test()
    mito_log("test function called")
end
