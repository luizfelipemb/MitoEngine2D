Level = {
    entities = {
        {
            name = "GameManager",
            components = {
                scripts = {
                    "GameManager.lua"
                }
            }
        },
        {
            name = "player",
            components = {
                transform = {
                    position = { x = 50, y = 650 },
                    scale = 1,
                    rotation = 0.0,
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                boxcollider = {
                    width = 128,
                    height = 16
                },
                sprite = {
                    sprite_name = "assets/images/paddle.png",
                    width = 128,
                    height = 28
                },
                scripts = {
                    "player.lua"
                }
            }
        },
        {
            name = "ball",
            components = {
                transform = {
                    position = { x = 300, y = 300 },
                    scale = 1,
                    rotation = 0.0,
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                boxcollider = {
                    width = 10,
                    height = 10
                },
                sprite = {
                    sprite_name = "assets/images/ball.png",
                    width = 16,
                    height = 16
                },
                scripts = {
                    "ball.lua"
                }
            }
        },
        {
            name = "enemy",
            components = 
            {
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
                scripts = {
                    "enemy.lua"
                }
            }
        }
    }
}
