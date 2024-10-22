# Mito 2D Game Engine

A lightweight game engine built with **C++**, **SDL2**, and **ImGui**, featuring **Lua scripting** for game logic.

## Features
- **2D Rendering** with SDL2
- **Lua Scripting** for customizable game behavior
- **Unity** familiarities like namings and scene structure
- **Collision and Logs** debugging by pressing F3
- **Pong Game** already implemented as an example

## Installation

To set up the engine:

1. Clone this repository:
    ```bash
    git clone https://github.com/luizfelipemb/MitoEngine2D
    ```
2. Run the provided batch file to generate the project:
    ```bash
    GenerateProject.bat
    ```
## Important Notes
- The first level that loads is always `main.lua`, which should be placed in the `assets/levels/` folder.
- Folder names matters so don't change the already existing ones.

## Creating a Scene

In Mito 2D Game Engine, scene files are Lua scripts that define the layout, entities, and components within a game level. These files describe the entities that will appear in the scene, along with their respective components such as transform, sprite, text, and scripts. Below is an example scene file.

### Example: `menu.lua`

```lua
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
                    position = {
                        x = mito.window.width / 7,
                        y = mito.window.height / 7
                    }
                },
                text = {
                    text = "Breakout",
                    font = "assets/fonts/charriot.ttf",
                    scale = 100
                }
            }
        }
    }
}
```
## Understanding Prefab Files

Prefab files in Mito 2D Game Engine are reusable templates that define a set of components for a game object just like we did in the scene above. 
They allow you to create complex objects once and instantiate them multiple times throughout your game. Below is an example.

### Example: `explosionFX.lua`

```lua
Prefab = {
    name = "explosion",
    components = {
        transform = {
            position = { x = 0, y = 0 },
            scale = 1,
            rotation = 0.0,
        },
        sprite = {
            sprite_name = "assets/images/explosion.png",
            width = 64,
            height = 64
        },
        animation = { 
            frame_count = 13,
            frame_duration = 20,
            loop = false
        }
    }
}
```

## Lua Bindings in Mito 2D Game Engine

The Mito 2D Game Engine provides a comprehensive set of Lua bindings, allowing developers to create and manipulate game objects and components using Lua scripts. Below is a detailed overview of the available bindings:

### Global Functions

- **`mito.log(message)`**  
  Logs a message to the console.

- **`mito.gameobject.create(name)`**  
  Creates a new `GameObject` with an optional name.

- **`mito.gameobject.destroy(gameObject)`**  
  Destroys a specified `GameObject`.

- **`mito.gameobject.find_by_tag(tag)`**  
  Finds and returns a `GameObject` by its tag.

- **`mito.open_level(name)`**  
  Opens a specified level.

- **`mito.spawn_prefab(name)`**  
  Spawns a prefab with the given name.

### GameObject Methods

- **`gameobject.get_id()`**  
  Returns the ID of the game object.

- **`gameobject.name()`**  
  Gets the name of the game object.

- **`gameobject.has_tag(tag)`**  
  Checks if the game object has a specified tag.

- **`gameobject.set_tag(tag)`**  
  Sets a tag for the game object.

#### Adding Components to GameObjects

- **`gameobject.add_component_transform(position, scale, rotation)`**  
  Adds a `TransformComponent` to the game object with optional position (`vec2`), scale, and rotation.

- **`gameobject.add_component_sprite(sprite, width, height, layer, color, source)`**  
  Adds a `SpriteComponent` with options for dimensions, layer, color, and source rectangle (`vec4`).

- **`gameobject.add_component_animation(frameCount, frameDuration, loop, autoDestroy)`**  
  Adds an `AnimationComponent` with optional frame count, duration, looping, and auto-destroy behavior.

- **`gameobject.add_component_text(text, font, layer, scale, color)`**  
  Adds a `TextComponent` with text, font, and optional color (`color` object).

- **`gameobject.add_component_rigidbody(velocity, acceleration)`**  
  Adds a `RigidBody2DComponent` with optional velocity and acceleration (`vec2`).

- **`gameobject.add_component_boxcollider(width, height, offset)`**  
  Adds a `BoxCollider2DComponent` with optional dimensions and offset (`vec2`).

- **`gameobject.add_component_clickable(width, height, offset)`**  
  Adds a `ClickableComponent` to make the object interactable, with optional size and offset.

- **`gameobject.add_component_script(scriptName)`**  
  Adds a `ScriptComponent` to the object, linking it to the specified Lua script.

#### Getting Components from GameObjects

- **`gameobject.get_component_transform()`**  
  Retrieves the `TransformComponent` from the game object.

- **`gameobject.get_component_sprite()`**  
  Retrieves the `SpriteComponent`.

- **`gameobject.get_component_animation()`**  
  Retrieves the `AnimationComponent`.

- **`gameobject.get_component_text()`**  
  Retrieves the `TextComponent`.

- **`gameobject.get_component_boxcollider()`**  
  Retrieves the `BoxCollider2DComponent`.

- **`gameobject.get_component_clickable()`**  
  Retrieves the `ClickableComponent`.

- **`gameobject.get_component_rigidbody()`**  
  Retrieves the `RigidBody2DComponent`.

- **`gameobject.get_component_script()`**  
  Retrieves the `ScriptComponent`.

### Components

- **`transform_component`**  
  Contains properties: `position`, `scale`, and `rotation`.

- **`rigidbody_component`**  
  Contains properties: `velocity` and `acceleration`.

- **`sprite_component`**  
  Contains properties: `width`, `height`, and `color`.

- **`animation_component`**  
  Methods: `play()`, `pause()`, `stop()`, `is_playing()`, `current_frame()`, `loop_count()`, and `set_frame(frame)`.

- **`text_component`**  
  Contains properties: `text`, `font_size`, and `color`.

- **`script_component`**  
  Method: `get_function(name)` retrieves a script function by name.

### Audio

- **`mito.audio.load_sound(path)`**  
  Loads a sound from the specified path.

- **`mito.audio.play_sound(path)`**  
  Plays the specified sound.

- **`mito.audio.stop_sound(path)`**  
  Stops the specified sound.

- **`mito.audio.load_music(path)`**  
  Loads background music from the specified path.

- **`mito.audio.play_music(path)`**  
  Plays the loaded background music.

- **`mito.audio.stop_music()`**  
  Stops the currently playing background music.

- **`mito.audio.set_music_volume(volume)`**  
  Sets the volume of the background music.

- **`mito.audio.set_sound_volume(volume)`**  
  Sets the global sound effect volume.

- **`mito.audio.get_music_volume()`**  
  Gets the current music volume.

- **`mito.audio.get_sound_volume()`**  
  Gets the current global sound volume.

### Input (Key and Mouse)

- **`mito.mousecode`**  
  Enum for mouse buttons: `right`, `left`, `middle`.

- **`mito.keycode`**  
  Enum for key codes, including letters (`a`, `b`, etc.), numbers (`0`, `1`, etc.), and special keys (`escape`, `return`, `space`, `shift`, etc.).

### Window Settings

- **`window.width`**  
  Gets the width of the game window.

- **`window.height`**  
  Gets the height of the game window.


[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
