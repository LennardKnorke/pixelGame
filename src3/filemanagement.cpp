#include "declr.hpp"
#include "tilemakros.hpp"
//free up name pointer
void Player::freePlayer(void){
    free(this->name);
}

//set default values if the created instance is a new player
void Player::createPlayer(std::string name, std::string key){
    this->name = (char *)malloc(sizeof(char) * (name.size()));
    std::strcpy(this->name, name.c_str());
    std::strcpy(this->key, key.c_str());
    this->level = 1;
    this->alive = true;
}

//save player information in given binary file
void Player::writePlayer(std::ofstream &outputFile){
    //Save Name
    size_t stringSize = strlen(this->name);
    outputFile.write(reinterpret_cast<char *>(&stringSize), sizeof(stringSize));
    outputFile.write(reinterpret_cast<char *>(&name), stringSize);
    //Save Key
    stringSize = strlen(this->key);
    outputFile.write(reinterpret_cast<char *>(&key), stringSize);
    //Save game stats.. Fill up as features get added
    outputFile.write(reinterpret_cast<char *>(&level), sizeof(int));
    outputFile.write(reinterpret_cast<char *>(&alive), sizeof(bool));
}

//read player information from file into instance
void Player::readPlayer(std::ifstream &inputFile){
    //Read Name
    size_t stringSize = 0;
    inputFile.read(reinterpret_cast<char *>(&stringSize), sizeof(stringSize));
    inputFile.read(reinterpret_cast<char *>(&name), stringSize);
    //Read Key
    stringSize = strlen(this->key);
    inputFile.read(reinterpret_cast<char *>(&key), stringSize);
    //Read more game stats
    inputFile.read(reinterpret_cast<char *>(&level), sizeof(int));
    inputFile.read(reinterpret_cast<char *>(&alive), sizeof(bool));

}


// SAVE FILE CLASS FUNCTIONS
//Free up name pointers
void SaveFile::freeSaveFile(void){
    for (auto &joinedPlayerIterator : joinedPlayers){
        joinedPlayerIterator.freePlayer();
    }
    free(this->worldName);
}

//form instance into a new save template
void SaveFile::createNewSave(std::string name, std::string ownerName, std::string ownerKey){
    //Add owner key
    std::strcpy(this->owner_Key, ownerKey.c_str());

    //Add world name
    this->worldName = (char *)malloc(sizeof(char) * (name.size()));
    std::strcpy(this->worldName, name.c_str());

    //Set game stats
    this->initialized = false;

    //Add owner to players
    Player p;
    p.createPlayer(ownerName, ownerKey);
    this->joinedPlayers.push_back(p);
}

//saves the File
void SaveFile::writeIntoFile(std::ofstream &outputFile){
    //Save world Name
    size_t stringSize = strlen(this->worldName);
    outputFile.write(reinterpret_cast<char *>(&stringSize), sizeof(stringSize));
    outputFile.write(reinterpret_cast<char *>(&worldName), stringSize);

    //Save owner Key
    stringSize = strlen(this->owner_Key);
    outputFile.write(reinterpret_cast<char *>(&owner_Key), stringSize);

    //Save whether save is initialized
    outputFile.write(reinterpret_cast<char *>(&initialized), sizeof(bool));
    if (initialized){//Save more stuff if save is initialized
        outputFile.write(reinterpret_cast<char *>(&seed), sizeof(int));
        //Save tiles if initialized
    }

    //Save all Players
    int nplayers = joinedPlayers.size();
    outputFile.write(reinterpret_cast<char *>(&nplayers), sizeof(int));
    for (auto &tmpPlayer: joinedPlayers){
        tmpPlayer.writePlayer(outputFile);
    }
}
//Fill the instance with binaries from file
void SaveFile::readFromFile(std::ifstream &inputFile){
    //Read World Name
    size_t stringSize;
    inputFile.read(reinterpret_cast<char *>(&stringSize), sizeof(stringSize));
    inputFile.read(reinterpret_cast<char *>(&worldName), stringSize);
    //Read owner key
    stringSize = strlen(this->owner_Key);
    inputFile.read(reinterpret_cast<char *>(&owner_Key), stringSize);
    //Read whether its inialized
    inputFile.read(reinterpret_cast<char *>(&initialized), sizeof(bool));
    //if initliazed... read more information
    if (initialized){
        inputFile.read(reinterpret_cast<char *>(&seed), sizeof(int));
        //Read TileInformation
    }
    //Read number of players
    int nPlayers;
    inputFile.read(reinterpret_cast<char *>(&nPlayers), sizeof(int));
    for (int i = 0; i < nPlayers; i++){
       Player tmp;
       tmp.readPlayer(inputFile);
       joinedPlayers.push_back(tmp);
    }
}


//Opens a json file and returns its content
nlohmann::json read_json_file (const std::string &path){
    std::ifstream input_file(path);
    nlohmann::json j;
    input_file >> j;
    return j;
}


/////PROCESSING OF WORLD FILE!!
//World information to be saved in a file!
void createWorldFile (std::string ownerName, std::string ownerKey, std::string worldName){
    //Prepare FileName
    int i = 0;
    std::string fileName = "../worlds/";
    do 
    {
        std::string fileCopy = fileName;
        fileCopy.append(std::to_string(i));
        fileCopy.append(".WORLDSAVE");
        if (std::filesystem::exists(fileCopy)){
            i++;
        }
        else {
            fileName = fileCopy;
            break;
        }
    } while (1);    
    //Generate seeed;

    //Create new world Information struct
    SaveFile newSafe;
    newSafe.createNewSave(worldName, ownerName, ownerKey);

    std::ofstream file(fileName, std::ios::binary);
    if (file){
        newSafe.writeIntoFile(file);
        newSafe.~SaveFile();
        file.close();
    }
    else{
        std::exit(0);
    }

    
    return;
}