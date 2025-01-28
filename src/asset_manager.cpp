#include "asset_manager.hpp"


asset_manager::~asset_manager(){
    // Destructor
}

bool asset_manager::load(void){
    // Load Textures
    if (!textures.cursors.loadFromFile("assets/sprites/cursor.png", true)){
        std::cout << "Failed to load cursor texture: " << std::endl;
        return false;
    }
    textures.cursors.setSmooth(true);


    // Load Music
    int i = 0;
    for (auto &iterator : std::filesystem::directory_iterator("assets/music/")){
        if (!backgroundMusic[i].openFromFile(iterator.path().string())){
            std::cout << "Failed to load Music: " << iterator.path().string() << std::endl;
            return false;
        }
        i++;
    }

    // Load Font
    if (!font.openFromFile("assets/PFAgoraSlabPro Bold.ttf")){
        std::cout << "Failed to load Font: " << std::endl;
        return false;
    }

    return true;
}