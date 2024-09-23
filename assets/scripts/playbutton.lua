function on_mouse_interacted(gameobject)
    play_sound("menu_select.wav")
    mito_log("pressed play again button")
    open_level("game.lua")
end