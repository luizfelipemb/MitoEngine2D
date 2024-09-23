local rigidbody, transform, sprite
local gamemanager

function start(gameobject)
    gamemanager = find_by_tag("gamemanager")
    mito_log("gamemanager: " .. tostring(gamemanager))
    mito_log(gameobject.name .. "start called")
    transform = gameobject:get_component_transform()
    rigidbody = gameobject:get_component_rigidbody()
    sprite = gameobject:get_component_sprite()
    rigidbody.acceleration.y = 10
    rigidbody.velocity.y = 200
end

function InvertBallY()
    rigidbody.velocity.y = -rigidbody.velocity.y
    rigidbody.acceleration.y = -rigidbody.acceleration.y
end

function update(gameobject, deltaTime)
    mito_log(tostring(rigidbody.velocity.y))
    
    if transform.position.y < 0 and rigidbody.velocity.y < 0 then
        play_sound("ball_hit.wav")
        InvertBallY()
    end
    if transform.position.y > Window.height then
        --gameover
        open_level("lose.lua")
    end
    
    if transform.position.x > Window.width - sprite.width 
            or transform.position.x < 0 then
        rigidbody.velocity.x = -rigidbody.velocity.x
        play_sound("ball_hit.wav")
    end
end

function on_collision_enter(gameobject, other, direction)
    mito_log(other.name .. " with direction: " .. tostring(direction.x)..","..tostring(direction.y))
    if other:has_tag("player") then
        local x_dif = other:get_component_transform().position.x - transform.position.x + other:get_component_sprite().width/2
        InvertBallY()
        rigidbody.velocity.x = -x_dif
        play_sound("ball_hit.wav")
    end
    if other:has_tag("brick") then
        if direction.x ~= 0 and -0 then
            rigidbody.velocity.x = -rigidbody.velocity.x
        else
            InvertBallY()
        end
        destroy(other)
        globals.bricks_destroyed = globals.bricks_destroyed + 1
        play_sound("brick_destroyed.wav")
        gamemanager:get_component_script():get_function("on_brick_destroyed")()
    end
end

function test()
    mito_log("test function called")
end
