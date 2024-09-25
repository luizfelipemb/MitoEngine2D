function mito.on_mouse_interacted(gameobject)
    mito.audio.play_sound("menu_select.wav")
    mito.log("pressed play again button")
    mito.open_level("game.lua")
end