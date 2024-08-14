local textcomp
function start(gameobject)
	mito_log("START SCORE")
	textcomp = gameobject:get_component_text()
end

function update()
	textcomp.text = "Score: ".. tostring(globals.bricks_destroyed)
end