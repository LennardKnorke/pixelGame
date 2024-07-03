#include "application.hpp"
#include "game.hpp"

GAME_STATE Application::gameLoop(void){
    std::cout<<"Game Loop Started. Hurray"<<std::endl;

    // Prints to double check mode and loading save and connection tools
    if (mode == gameMode::Join){
        std::cout << "Joining Host: " << loadingTools.ip << std::endl;
    }
    else {
        std::cout << "Loading Game: " << loadingTools.chosenSave.name << " from " << loadingTools.chosenSave.path;
        if (gameMode::Single == mode){
            std::cout << " as SinglePlayer" << std::endl;
        }
        else {
            std::cout << " as Host" << std::endl;
        }
    }

    Game *game = new Game;
    GAME_STATE next_state = game->run();


    delete game;
    return next_state;
}

GAME_STATE Game::run(void){
    return GAME_STATE::QUIT;
}
