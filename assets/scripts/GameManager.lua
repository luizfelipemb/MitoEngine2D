
function start(gameobject)
    --create("name")
    mito_log("got from id:" .. get_from_id(0):get_name())
end

function update(gameobject, deltaTime)
    mito_log(go:get_name())
end