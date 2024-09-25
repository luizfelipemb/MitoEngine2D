globals.bricks_destroyed = 0
globals.level = 0

Level = {
    entities = {
        {
            name = "background",
            components = {
                transform = {
                    position = { x = 0, y = 0 }
                },
                sprite = {
                    sprite_name = "",
                    width = mito.window.width,
                    height = mito.window.height,
                    color = mito.color.new(60,25,25)
                },
                scripts = {
                }
            }
        },
        {
            name = "title",
            components = {
                transform = {
                    position =
                    {
                        x = mito.window.width / 7,
                        y = mito.window.height / 7
                    }
                },
                text = {
                    text = "You Lose",
                    font = "assets/fonts/charriot.ttf",
                    scale = 100
                },
                scripts = {
                }
            }
        },
        {
            name = "play_button",
            components = {
                transform = {
                    position =
                    {
                        x = mito.window.width / 2.5,
                        y = mito.window.height / 3
                    }
                },
                sprite = {
                    sprite_name = "",
                    width = 120,
                    height = 60,
                    color = mito.color.new(0,0,255)
                },
                clickable = {
                },
                scripts = {
                    "playbutton.lua"
                }
            }
        },
        {
            name = "play_text",
            components = {
                transform = {
                    position =
                    {
                        x = mito.window.width / 2.5,
                        y = mito.window.height / 3
                    }
                },
                text = {
                    text = "Play",
                    font = "assets/fonts/charriot.ttf",
                    scale = 60
                }
            }
        }
    }
}