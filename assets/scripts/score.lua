local textcomp
function mito.event.start(gameobject)
	mito.log("START SCORE")
	textcomp = gameobject:get_component_text()
end

function mito.event.update()
	textcomp.text = "Score: ".. tostring(globals.bricks_destroyed)
end