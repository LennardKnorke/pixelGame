//Contains Function definitions and structures for the menu and loading screen!
#include "declr.hpp"


//Opens a json file and returns its content
nlohmann::json read_json_file (const std::string &path)
{
    std::ifstream input_file(path);
    nlohmann::json j;
    input_file >> j;
    return j;
}

//Generate a random string for profile keys
std::string generateString(void)
{
    const std::string alphabet = "!#$%^&*(){}=-+/abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::srand(std::time(NULL));

    std::string result;
    for (int i = 0 ; i < 15; i++)
    {
        result += alphabet[rand() % ((int)alphabet.size())];
    }
    return result;
}

//return the index of a string in a vector of strings. return -1 if its not there
int findStringIndex(std::string &target, std::vector<std::string> &stringArray)
{
    for (unsigned int i = 0; i < stringArray.size(); i++)
    {
        if (stringArray[i] == target)
        {
            return i;
        }
    }
    return -1;
}
//Given information for a new profile, the function updates the config file and saves it
void addProfileToConfig(nlohmann::json &configFileToSave, std::string &newProfileKey, std::string &newProfileName)
{
    //Update config data
    configFileToSave["nprofiles"] = configFileToSave["nprofiles"].get<int>() + 1;                  //Increase number of profiles
    configFileToSave["profiles"].push_back({{"name", newProfileName}, {"key", newProfileKey}});    //Add profile and key
    //save file
    std::ofstream savingOutput("../config.json");
    savingOutput << configFileToSave.dump(4);
    savingOutput.close();
}

void deleteProfile(nlohmann::json &configFile, std::string &ProfileKey, std::string &ProfileName)
{
    //for (auto &files : std::experimental::filesystem::recursive_directory_iterator("../worlds/")){
    //    std::cout<< files << std::endl;
    //}
    
}

/////PROCESSING OF WORLD FILE!!
//World information to be saved in a file!
struct worldTileInformationTemplate
{
    std::string owner_name;                                  //Remeber the owner!
    std::string owner_key;     
    bool initialized;                                   //
    short unsigned worldTileInformation[WORLD_WIDTH_C][WORLD_DEPTH_C] = {EMPTY_TILE};   //Saves the "state" of each tile
    int seed;                                           //Seed used to create world
    std::vector<PlayerInformation> joinedPlayers;       //List of players that joined and their information that needs saving
}; 
//Information to be saved in a save file!
struct PlayerInformation
{
    std::string name;                                   //PlayerName
    std::string key;                                    //PlayerKey
    bool initialized;                                        //if false, loading the world will set up all the stats for the player
    int level;                                          //what is the level?
    bool alive;                                         //are they alive?
};

//
void createWorldFile (std::string ownerName, std::string ownerKey, std::string worldName)
{
    //Prepare FileName
    std::string fileName = "../worlds/";
    fileName.append(gV::hosting_chosen_world.c_str());
    fileName.append(".WORLDSAVE\0");

    //Create new world Information struct
    SaveFile newSafe;
    newSafe.initialized = false;
    newSafe.owner_name = ownerName;
    newSafe.owner_key = ownerKey;
    newSafe.seed = 0;
    //Create Player entry for owner
    PlayerInformation addedOwner;
    addedOwner.initialized = false;
    addedOwner.name = ownerName;
    addedOwner.key = ownerKey;
    newSafe.joinedPlayers.push_back(addedOwner);

    //Create File;
    std::ofstream file(fileName.c_str(),std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<char *>(&newSafe), sizeof(newSafe));
        file.close();
        std::cout << "yey?\n";
    } 
    else 
    {
        std::cout << "Failed to produce file\n";
    }
    std::cout << "Check 5\n";
}


SaveFile openOwnedSave(std::string ownerKey, std::string fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    SaveFile loadedSave;
    file.read(reinterpret_cast<char*>(&loadedSave), sizeof(SaveFile));

    //Return default if its not the right save!
    if (loadedSave.owner_key == ownerKey)
    {
        return loadedSave;
    } 
    else 
    {
        return SaveFile();
    }
}