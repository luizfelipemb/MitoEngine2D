#include "./Game/Game.h"
#include <sol.hpp>

int main(int argc, char* argv[])
{
    
    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();
    return 0;
}
