mito.audio.play_music("menu_music.ogg")
--TODO: music player as component
Level = {
    entities = {
        {
            name = "background",
            components = {
                transform = {
                    position = mito.vec2.new(0, 0),
                    scale = 1,
                    rotation = 0.0,
                },
                sprite = {
                    sprite_name = "",
                    width = mito.window.width,
                    height = mito.window.height,
                    layer = 0,
                    color = mito.color.new(25, 25, 25)
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
                        x = mito.window.width / 2.5,
                        y = mito.window.height / 3
                    }
                },
                sprite = {
                    sprite_name = "",
                    width = 120,
                    height = 60,
                    color = mito.color.new(0, 0, 255)
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
        },
    }
}
