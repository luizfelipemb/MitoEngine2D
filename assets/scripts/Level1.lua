Level = {
    entities = {
        [0] = {
            --player
            components = {
                transform = {
                    position = { x = 50, y = 100 },
                    scale = 1,
                    rotation = 0.0,
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                boxcollider = {
                    width = 50,
                    height = 50
                },
                sprite = {
                    sprite_name = "assets/images/RoundedSquare.png",
                    width = 32,
                    height = 32,
                    red = 0,
                    green = 255,
                    blue = 0
                }
            }
        },
        {
            --enemy
            components = {
                transform = {
                    position = { x = 200, y = 50 },
                    scale = 1,
                    rotation = 0.0,
                },
                rigidbody = {
                    velocity = { x = -50.0, y = 0.0 }
                },
                boxcollider = {
                    width = 100,
                    height = 100
                },
                sprite = {
                    sprite_name = "assets/images/RoundedSquare.png",
                    width = 100,
                    height = 100,
                    red = 255,
                    green = 0,
                    blue = 0
                },
                on_update_script = {
                    [0] =
                        function(entity, delta_time, ellapsed_time)
                            print("Executing Lua script!")
                        end
                }
            }
        }
    }
}
