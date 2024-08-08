local velocity = vec2:new(0, -200)

function start(gameobject)
    mito_log(gameobject.name .. "start called")
end

function update(gameobject, deltaTime)
    local rigidbody = gameobject:get_component_rigidbody()
    if goingUp then
        rigidbody.velocity.y = -200
    else
        rigidbody.velocity.y = 200
    end
end

function on_collision_enter(gameobject, other)
    mito_log(other.name)
    if other.name == "player" then
        goingUp = true
    end
    if string.find(other.name, "brick") then
        goingUp = false
        destroy(other)
    end
end

function test()
    mito_log("test function called")
end
