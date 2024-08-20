local rigidbody
local transform
local sprite
local velocity = 300

function start(gameobject)
    rigidbody = gameobject:get_component_rigidbody()
    transform = gameobject:get_component_transform()
    sprite = gameobject:get_component_sprite()
    mito_log("Start player function called on " .. gameobject.name)
    mito_log(tostring(gameobject:has_tag("player")))
end

function update(gameobject, deltaTime)
    if transform.position.x < 0 then
        transform.position.x = 0
    elseif (transform.position.x > Window.width - sprite.width) then
        transform.position.x = Window.width - sprite.width
    end
end

function on_key_pressed(gameobject, key)
    if key == "97" then
        rigidbody.velocity.x = rigidbody.velocity.x - velocity
    end
    if key == "100" then
        rigidbody.velocity.x = rigidbody.velocity.x + velocity
    end
    mito_log(gameobject.name .. " with key " .. key)
end

function on_key_released(gameobject, key)
    if key == "97" then
        rigidbody.velocity.x = rigidbody.velocity.x + velocity
    end
    if key == "100" then
        rigidbody.velocity.x = rigidbody.velocity.x - velocity
    end
    mito_log(gameobject.name .. " with key released " .. key)
end

function on_collision_enter(gameobject, other)
    mito_log(gameobject.name .. " collided with " .. other.name)
    local startFunc = other:get_component_script():get_function("test")
    startFunc()
end
