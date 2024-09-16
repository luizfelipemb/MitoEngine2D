local velocity = vec2:new(0, -200)
local rigidbody, transform, sprite
local goingUp
local audio_manager
local gamemanager

function start(gameobject)
    gamemanager = find_by_tag("gamemanager")
    mito_log(tostring(gamemanager))
    mito_log(gameobject.name .. "start called")
    gameobject:add_component_audio()

    transform = gameobject:get_component_transform()
    rigidbody = gameobject:get_component_rigidbody()
    sprite = gameobject:get_component_sprite()
    audio_manager = gameobject:get_component_audio()

    audio_manager:load_sound("cymbal", "assets/sounds/cymbal-83127.mp3")
    audio_manager:load_sound("screen_tap", "assets/sounds/screen_tap.mp3")


end

function update(gameobject, deltaTime)
    if goingUp then
        rigidbody.velocity.y = -200
    else
        rigidbody.velocity.y = 200
    end
    
    if transform.position.y < 0 then
        goingUp = false
    end
    if transform.position.y > Window.height then
        --gameover
        open_level("lose.lua")
    end
    
    if transform.position.x > Window.width - sprite.width or transform.position.x<0 then
        rigidbody.velocity.x = -rigidbody.velocity.x
    end
end

function on_collision_enter(gameobject, other, direction)
    mito_log(other.name .. " with direction: " .. tostring(direction.x)..","..tostring(direction.y))
    if other.name == "player" then  
        audio_manager:play_sound("screen_tap", 0)
        local x_dif = other:get_component_transform().position.x - transform.position.x + other:get_component_sprite().width/2
        goingUp = true
        rigidbody.velocity.x = -x_dif
    end
    if other:has_tag("brick") then

        audio_manager:play_sound("cymbal", 0)
        if direction.x ~= 0 and -0 then
            rigidbody.velocity.x = -rigidbody.velocity.x
        else
            goingUp = not goingUp
        end
        destroy(other)
        globals.bricks_destroyed = globals.bricks_destroyed + 1
    
        if globals.bricks_destroyed >= globals.max_bricks then
            open_level("won.lua")

        end
        gamemanager:get_component_script():get_function("on_brick_destroyed")()
    end
end

function test()
    mito_log("test function called")
end
