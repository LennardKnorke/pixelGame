////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
#include <random>
#include <vector>

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

void createWorldFile(void){
    return;
}

std::string generateString(void){
    const std::string alphabet = "!#$%^&*(){}=-+/abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, alphabet.length() - 1);
    std::string result;
    for (int i = 0 ; i < 15; i++){
        result += alphabet[distribution(generator)];
    }
    return result;
}

void addProfileToConfig(nlohmann::json configFileToSave, std::string newProfileKey,std::string newProfileName){
    return;
}