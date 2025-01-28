#include "settings.hpp"


settings_class::settings_class(bool file_exists, bool *dev){
    p_dev = dev;
    if (file_exists){
        load();
        if (*p_dev){
            std::cout << "SETTINGS LOADED" << std::endl;
        }
    }
    else {
        if (*p_dev){
            std::cout << "SETTINGS CREATED" << std::endl;
        }
        // set user id
        userId = "";
        const std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!?";
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<std::string::size_type> dist(0, alphabet.size()-1);
        for (short i = 0 ; i < maxInputLengths::userId; i++){
            //this->localUserID += alphabet[dist(rng)];
            userId += alphabet[dist(rng)];
        }
        // set default values
        volumne = DEFAULT_VOLUME;
        
        controls[up].iType = inputType::KEYBOARD;
        controls[up].input.keyInput = sf::Keyboard::Key::W;

        controls[down].iType = inputType::KEYBOARD;
        controls[down].input.keyInput = sf::Keyboard::Key::S;

        controls[left].iType = inputType::KEYBOARD;
        controls[left].input.keyInput = sf::Keyboard::Key::A;

        controls[right].iType = inputType::KEYBOARD;
        controls[right].input.keyInput = sf::Keyboard::Key::D;

        controls[prevItem].iType = inputType::KEYBOARD;
        controls[prevItem].input.keyInput = sf::Keyboard::Key::Q;

        controls[nextItem].iType = inputType::KEYBOARD;
        controls[nextItem].input.keyInput = sf::Keyboard::Key::E;

        controls[attack].iType = inputType::MOUSE_BUTTON;
        controls[attack].input.mouseInput = sf::Mouse::Button::Left;

        controls[special].iType = inputType::KEYBOARD;
        controls[special].input.keyInput = sf::Keyboard::Key::LShift;

        controls[useItem].iType = inputType::KEYBOARD;
        controls[useItem].input.keyInput = sf::Keyboard::Key::G;

        controls[jump].iType = inputType::KEYBOARD;
        controls[jump].input.keyInput = sf::Keyboard::Key::Space;


        fps = DEFAULT_FPS;
        res.fullscreen = DEFAULT_FULLSCREEN;
        res.height = DEFAULT_WINDOW_HEIGHT;
        res.width = DEFAULT_WINDOW_WIDTH;
        save();
    }
    res.scaling = sf::Vector2f((float)res.width / (float)DEFAULT_WINDOW_WIDTH, (float)res.height / (float)DEFAULT_WINDOW_HEIGHT);

    if (*p_dev){
        std::cout << "\tUser: " << userId << std::endl;
        std::cout << "\tVolume: " << volumne << std::endl;
        std::cout << "\tFPS: " << fps << std::endl;
        std::cout << "\tFullscreen: " << res.fullscreen << std::endl;
        std::cout << "\tResolution: " << res.width << "x" << res.height << std::endl;
        std::cout << std::endl;
    }
}

std::string settings_class::get_userId(void){
    return userId;
}

settings_class::~settings_class(){
    save();
}


void settings_class::save(void){
    /*
    Write into binary file. be careful to maintain order during save and load
    */
    std::ofstream outputFile("settings.bin", std::ios::binary);

    writeStrToFile(outputFile, userId);

    outputFile.write(reinterpret_cast<const char*>(&volumne), sizeof(float));

    for (int i = 0; i < n_keyInputOptions; i++){
        outputFile.write(reinterpret_cast<const char*>(&controls[i].iType), sizeof(inputType));
        if (controls[i].iType == inputType::KEYBOARD){
            outputFile.write(reinterpret_cast<const char*>(&controls[i].input.keyInput), sizeof(sf::Keyboard::Key));
        }
        else if (controls[i].iType == inputType::MOUSE_BUTTON){
            outputFile.write(reinterpret_cast<const char*>(&controls[i].input.mouseInput), sizeof(sf::Mouse::Button));
        }
    }

    outputFile.write(reinterpret_cast<const char*>(&fps), sizeof(short));
    outputFile.write(reinterpret_cast<const char*>(&res.fullscreen), sizeof(bool));

    outputFile.write(reinterpret_cast<const char*>(&res.width), sizeof(unsigned int));
    outputFile.write(reinterpret_cast<const char*>(&res.height), sizeof(unsigned int));

    outputFile.close();

}

void settings_class::load(void){
    /*
    Read from binary file, be careful to maintain order during save and load
    */
    std::ifstream inputFile("settings.bin", std::ios::binary);
    

    readStrOfFile(inputFile, userId);

    inputFile.read(reinterpret_cast<char*>(&volumne), sizeof(float));

    for (int i = 0; i < n_keyInputOptions; i++){
        inputFile.read(reinterpret_cast<char*>(&controls[i].iType), sizeof(inputType));
        if (controls[i].iType == inputType::KEYBOARD){
            inputFile.read(reinterpret_cast<char*>(&controls[i].input.keyInput), sizeof(sf::Keyboard::Key));
        }
        else if (controls[i].iType == inputType::MOUSE_BUTTON){
            inputFile.read(reinterpret_cast<char*>(&controls[i].input.mouseInput), sizeof(sf::Mouse::Button));
        }
    }

    inputFile.read(reinterpret_cast<char*>(&fps), sizeof(short));
    inputFile.read(reinterpret_cast<char*>(&res.fullscreen), sizeof(bool));

    inputFile.read(reinterpret_cast<char*>(&res.width), sizeof(unsigned int));
    inputFile.read(reinterpret_cast<char*>(&res.height), sizeof(unsigned int));

    inputFile.close();
}