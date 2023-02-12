////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <string>

//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>

#include "declr.h"

nlohmann::json read_json_file (const std::string &path){
    std::ifstream input_file(path);
    nlohmann::json j;
    input_file >> j;
    return j;
}

void resizeTextureAndImages (Image imageList[], Texture2D textureList[],  int sizes[][2], int numberImages){
    for (int i = 0; i < numberImages; i++){
        UnloadTexture(textureList[i]);
        ImageResize(&imageList[i], sizes[i][0] * gV::screenRatio, sizes[i][1] * gV::screenRatio);
        textureList[i] = LoadTextureFromImage(imageList[i]);
    }
    
}