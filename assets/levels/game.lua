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
                    color = color.new(25,25,25)
                }
            }
        },
        {
            name = "GameManager",
            components = {
                scripts = {
                    "gamemanager.lua"
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
                    position = { x = Window.width/2.5, y = 650 },
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
                clickable = {
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
                    position = { x = Window.width/2, y = 300 },
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
                    font = "assets/fonts/charriot.ttf",
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
