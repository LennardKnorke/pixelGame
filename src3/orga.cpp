//Contains Function definitions and structures for the menu and loading screen!
#include "declr.hpp"
#include "tilemakros.hpp"


//Generate a random string for profile keys
std::string generateString(void){
    const std::string alphabet = "!#$%^&*(){}=-+/abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::srand(std::time(NULL));

    std::string result;
    for (int i = 0 ; i < MAX_KEY_LENGTH; i++){
        result += alphabet[rand() % ((int)alphabet.size())];
    }
    return result;
}

//return the index of a string in a vector of strings. return -1 if its not there
int findStringIndex(std::string &target, std::vector<std::string> &stringArray){
    for (unsigned int i = 0; i < stringArray.size(); i++){
        if (stringArray[i] == target){
            return i;
        }
    }
    return -1;
}

//Given information for a new profile, the function updates the config file and saves it
void addProfileToConfig(nlohmann::json &configFileToSave, std::string &newProfileKey, std::string &newProfileName){
    //Update config data
    configFileToSave["nprofiles"] = configFileToSave["nprofiles"].get<int>() + 1;                  //Increase number of profiles
    configFileToSave["profiles"].push_back({{"name", newProfileName}, {"key", newProfileKey}});    //Add profile and key
    //save file
    std::ofstream savingOutput("../config.json");
    savingOutput << configFileToSave.dump(4);
    savingOutput.close();
}

void deleteProfile(nlohmann::json &configFile, std::string &ProfileKey, std::string &ProfileName){
    //for (auto &files : std::experimental::filesystem::recursive_directory_iterator("../worlds/")){
    //    std::cout<< files << std::endl;
    //}
    
}
