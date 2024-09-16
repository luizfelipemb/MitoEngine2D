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
                    width = Window.width,
                    height = Window.height,
                    red = 25,
                    green = 180,
                    blue = 25
                },
                audio = {
                    id = "you_win",
                    path = "assets/sounds/you_win.mp3",
                    auto_play = true,
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
                    text = "You won!!",
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
                    red = 0,
                    green = 0,
                    blue = 255
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
        }
    }
}