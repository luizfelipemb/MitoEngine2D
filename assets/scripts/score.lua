local textcomp
function mito.start(gameobject)
	mito.log("START SCORE")
	textcomp = gameobject:get_component_text()
end

function mito.update()
	textcomp.text = "Score: ".. tostring(globals.bricks_destroyed)
end