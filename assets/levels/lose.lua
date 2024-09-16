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
                    color = color.new(60,25,25)
                },
                audio = {
                    id = "game_over",
                    path = "assets/sounds/game_over.mp3",
                    auto_play = true, --- default -> false
                    music = false  --- default -> false
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
                        x = Window.width / 2.5,
                        y = Window.height / 3
                    }
                },
                sprite = {
                    sprite_name = "",
                    width = 120,
                    height = 60,
                    color = color.new(0,0,255)
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