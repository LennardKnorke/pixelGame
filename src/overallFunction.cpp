////GAME
//Standart libs
#include <iostream>
#include <fstream>
#include <string>

//External libs
#include "nlohmann/json.hpp"
#include <raylib.h>


nlohmann::json read_json_file (const std::string &path){
    std::ifstream input_file(path);
    nlohmann::json j;
    input_file >> j;
    return j;
}