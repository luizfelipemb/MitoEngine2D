play_music("menu_music.ogg")
--TODO: music player as component
Level = {
    entities = {
        {
            name = "background",
            components = {
                transform = {
                    position = vec2.new(0, 0),
                    scale = 1,
                    rotation = 0.0,
                },
                sprite = {
                    sprite_name = "",
                    width = Window.width,
                    height = Window.height,
                    layer = 0,
                    color = color.new(25, 25, 25)
                }
            }
        },
        {
            name = "title",
            components = {
                transform = {
                    position =
                    {
                        x = Window.width / 7,
                        y = Window.height / 7
                    }
                },
                text = {
                    text = "Breakout",
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
                        x = Window.width / 2.5,
                        y = Window.height / 3
                    }
                },
                sprite = {
                    sprite_name = "",
                    width = 120,
                    height = 60,
                    color = color.new(0, 0, 255)
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
                        x = Window.width / 2.5,
                        y = Window.height / 3
                    }
                },
                text = {
                    text = "Play",
                    font = "assets/fonts/charriot.ttf",
                    scale = 60
                }
            }
        },
    }
}
