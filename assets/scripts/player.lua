
function start(gameobject)
    --mito_log("Start player function called on "..gameobject:get_name())
end

function update(gameobject)
    --mito_log("Update player function called on "..gameobject:get_name())
end

function on_key_pressed(gameobject, key)
    mito_log(key)
    local transform = gameobject:get_component_transform()
    local pos = transform.position
    mito_log(tostring(pos.x))
    pos.x = 0;
    --set_position(gameobject,0,0)
end