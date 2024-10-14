local rigidbody
local transform
local sprite
local velocity = 300
local Apressed = false
local Dpressed = false

function mito.start(gameobject)
    rigidbody = gameobject:get_component_rigidbody()
    transform = gameobject:get_component_transform()
    sprite = gameobject:get_component_sprite()
    mito.log("Start player function called on " .. gameobject.name)
    mito.log(tostring(gameobject:has_tag("player")))
end

function mito.update(gameobject, deltaTime)
    if transform.position.x < 0 then
        transform.position.x = 0
    elseif (transform.position.x > mito.window.width - sprite.width) then
        transform.position.x = mito.window.width - sprite.width
    end
end

function mito.on_mouse_pressed(gameobject, key)
    
end

function mito.on_mouse_interacted(gameobject)
    mito.log("on_mouse_interacted called")
end

function adjust_velocity()
    if Apressed then
        if Dpressed then
            rigidbody.velocity.x = 0
        else
            rigidbody.velocity.x = -velocity
        end
    elseif Dpressed then
        rigidbody.velocity.x = velocity
    else
        rigidbody.velocity.x = 0
    end
end

function mito.on_key_pressed(gameobject, key)
    if key == mito.keycode.a then
        Apressed = true
    end
    if key == mito.keycode.d then
        Dpressed = true
    end
    adjust_velocity()
    mito.log(gameobject.name .. " with key " .. key)
end

function mito.on_key_released(gameobject, key)
    if key == mito.keycode.a then
        Apressed = false
    end
    if key == mito.keycode.d then
        Dpressed = false
    end
    adjust_velocity()
    mito.log(gameobject.name .. " with key released " .. key)
end

function mito.on_collision_enter(gameobject, other)
    mito.log(gameobject.name .. " collided with " .. other.name)
    local startFunc = other:get_component_script():get_function("test")
    startFunc()
end
