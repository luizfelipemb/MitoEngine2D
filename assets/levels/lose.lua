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
                    red = 255,
                    green = 25,
                    blue = 25
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
                        x = Window.width / 2 - 100, 
                        y = Window.height / 2
                    }
                },
                text = {
                    text = "You Lose",
                    font = "assets/fonts/arial.ttf",
                    scale = 48
                },
                scripts = {
                }
            }
        }
    }
}