#include "save_management.hpp"


std::vector<gamesave_summary> read_all_save_summaries (void){
    std::vector<gamesave_summary> saveFiles;

    std::filesystem::path pathDirectory = std::string("sav/");
    for (auto &iterator : std::filesystem::recursive_directory_iterator(pathDirectory)){
        if (iterator.path().extension() == std::filesystem::path(".SAV")){
            saveFiles.push_back(load_summary(iterator.path().string()));
        }
    }
    return saveFiles;
}


gamesave_summary load_summary(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);
    gamesave_summary sumTmp;
    sumTmp.path = filename;

    readStrOfFile(inputFile, sumTmp.name);

    inputFile.read(reinterpret_cast<char*>(&sumTmp.initialized), sizeof(bool));

    inputFile.close();

    
    return sumTmp;

}

void save_settings(app_settings s){
    /*
    Write into binary file. be careful to maintain order during save and load
    */
    std::ofstream outputFile("settings.bin", std::ios::binary);

    writeStrToFile(outputFile, s.userId);

    outputFile.write(reinterpret_cast<const char*>(&s.volumne), sizeof(float));

    for (int i = 0; i < n_keyInputOptions; i++){
        outputFile.write(reinterpret_cast<const char*>(&s.controls[i].iType), sizeof(inputType));
        if (s.controls[i].iType == inputType::KEYBOARD){
            outputFile.write(reinterpret_cast<const char*>(&s.controls[i].input.keyInput), sizeof(sf::Keyboard::Key));
        }
        else if (s.controls[i].iType == inputType::MOUSE_BUTTON){
            outputFile.write(reinterpret_cast<const char*>(&s.controls[i].input.mouseInput), sizeof(sf::Mouse::Button));
        }
    }

    outputFile.write(reinterpret_cast<const char*>(&s.fps), sizeof(short));
    outputFile.write(reinterpret_cast<const char*>(&s.fullscreen), sizeof(bool));

    outputFile.write(reinterpret_cast<const char*>(&s.res_x), sizeof(unsigned int));
    outputFile.write(reinterpret_cast<const char*>(&s.res_y), sizeof(unsigned int));

    outputFile.close();

}

app_settings load_settings(void){
    /*
    Read from binary file, be careful to maintain order during save and load
    */
    app_settings s;
    std::ifstream inputFile("settings.bin", std::ios::binary);
    

    readStrOfFile(inputFile, s.userId);

    inputFile.read(reinterpret_cast<char*>(&s.volumne), sizeof(float));

    for (int i = 0; i < n_keyInputOptions; i++){
        inputFile.read(reinterpret_cast<char*>(&s.controls[i].iType), sizeof(inputType));
        if (s.controls[i].iType == inputType::KEYBOARD){
            inputFile.read(reinterpret_cast<char*>(&s.controls[i].input.keyInput), sizeof(sf::Keyboard::Key));
        }
        else if (s.controls[i].iType == inputType::MOUSE_BUTTON){
            inputFile.read(reinterpret_cast<char*>(&s.controls[i].input.mouseInput), sizeof(sf::Mouse::Button));
        }
    }

    inputFile.read(reinterpret_cast<char*>(&s.fps), sizeof(short));
    inputFile.read(reinterpret_cast<char*>(&s.fullscreen), sizeof(bool));

    inputFile.read(reinterpret_cast<char*>(&s.res_x), sizeof(unsigned int));
    inputFile.read(reinterpret_cast<char*>(&s.res_y), sizeof(unsigned int));

    inputFile.close();

    return s;
}