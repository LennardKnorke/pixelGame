////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <thread>
#include <random>
#include <vector>
#include <time.h>
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


void createWorldFile(void){
    return;
}

std::string generateString(void){
    const std::string alphabet = "!#$%^&*(){}=-+/abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::srand(std::time(NULL));

    std::string result;
    for (int i = 0 ; i < 15; i++){
        result += alphabet[rand() % ((int)alphabet.size())];
    }
    return result;
}

int findStringIndex(std::string &target, std::vector<std::string> &stringArray){
    for (unsigned int i = 0; i < stringArray.size(); i++){
        if (stringArray[i] == target){
            return i;
        }
    }
    return -1;
}

void addProfileToConfig(nlohmann::json &configFileToSave, std::string &newProfileKey, std::string &newProfileName){
    //Update config
    configFileToSave["nprofiles"] = configFileToSave["nprofiles"].get<int>() + 1;                                         //Increase number of profiles
    configFileToSave["profiles"].push_back({{"name", newProfileName}, {"key", newProfileKey}});    //Add profile and key
    //save file
    std::ofstream savingOutput("../config.json");
    savingOutput << configFileToSave.dump(4);
    savingOutput.close();
}