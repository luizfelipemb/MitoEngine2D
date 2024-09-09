Level = {
    entities = {
        {
            name = "background",
            components = {
                transform = {
                    position = { x = 0, y = 0 },
                    scale = 1,
                    rotation = 0.0,
                },
                sprite = {
                    sprite_name = "",
                    width = Window.width,
                    height = Window.height,
                    layer = 0,
                    red = 25,
                    green = 25,
                    blue = 25
                }
            }
        },
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
            tags = {
                "player"
            },
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
                    height = 28,
                    layer = 1
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
                },
                sprite = {
                    sprite_name = "assets/images/ball.png",
                    width = 16,
                    height = 16,
                    layer = 2
                },
                scripts = {
                    "ball.lua"
                }
            }
        },
        {
            name = "score",
            components = {
                transform = {
                    position = { x = 10, y = 0 },
                    scale = 1,
                    rotation = 0.0,
                },
                text = {
                    text = "Score:",
                    font = "assets/fonts/arial.ttf",
                    scale = 32,
                    layer = 99
                },
                scripts = {
                    "score.lua"
                }
            }
        }
    }
}
