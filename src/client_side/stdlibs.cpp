#include "stdlibs.hpp"

////OUT OF SCOPE FUNCTIONS
bool fileExists(const std::string &filename){
    std::ifstream file(filename);
    bool works = file.good();
    file.close();
    return works;
}

bool mode_Host(gameMode mode){
    if (mode == gameMode::Local_host || mode == gameMode::Online_host || mode == gameMode::Single){
        return true;
    }
    return false;
}

bool mode_Online(gameMode mode){
    if (mode == gameMode::Online_client || mode == gameMode::Online_host){
        return true;
    }
    return false;
}