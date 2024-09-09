Prefab = {
    name = "brick",
    tags = { "brick"},
    components = {
        transform = {
            position = { x = 0, y = 0 },
            scale = 1,
            rotation = 0.0,
        },
        sprite = {
            sprite_name = "assets/images/brick.png"
        },
        boxcollider = {
        },
        scripts = { "brick.lua" }
    }
}
