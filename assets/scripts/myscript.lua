-- This is a Lua global variable
some_variable = 1
-- This is a Lua table that contain values for the initialization of our game engine
config = {
    title = "My Game Engine",
    fullscreen = false,
    resolution = {
        width = 800,
        height = 600
    }
}

-- This is a Lua function that calculates the factorial of a given number
function factorial(n)
    result = 1
    for i = n,1,-1 do
        result = result * i
    end
    print("factorial of",n, "is ",result)
    -- TODO:
    -- Go ahead and implement the factorial function in Lua
    -- and return the factorial of the number "n"
    -- For example:
    --     5! = 5x4x3x2x1 = 120
    --     7! = 7x6x5x4x3x2x1 = 5040
    -- You can use a simple print(result) to show the final result in the console
end

factorial(3)
factorial(4)
factorial(7)