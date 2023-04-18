#include "application.hpp"
//File for class functions around starting and running the main application
//Like loading settings, setting up the screen

void GameApplication::runApplication(void){
    //Initialize a few variable and window/window flags
    applicationRunning = true;
    applicationState = APPLICATIONSTATE_MENU;
    applicationFocused = true;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "fRaCtUrEd");

    //Settings and Folders
    if (FileExists("settings.bin")){
        loadSettings();
    }
    else {
        createSettings();
    }
    if (!DirectoryExists("sav/")){
        std::filesystem::create_directory("sav/");
    }

    //Set up window
    SetWindowSize(windowSettings.windowWidth, windowSettings.windowHeight);
    SetTargetFPS(60);
    HideCursor();
    if (IsWindowFullscreen() != windowSettings.fullScreen){
        ToggleFullscreen();
    }
    if(!IsWindowReady()){
        std::cout << "ERROR: Failed to initialize window!\n";
        return;
    }

    //Load Textures
    loadSprites();

    //Application cycle
    while (applicationRunning){
        if (applicationState == APPLICATIONSTATE_MENU){
            applicationState = runMenu();
        }
        if (applicationState == APPLICATIONSTATE_GAME){
            applicationState = runGame();
        }
        if (applicationState == APPLICATIONSTATE_QUIT){
            applicationRunning = false;
        }
    }

    //Unload Textures
    unloadSprites();
    return;
}


void GameApplication::loadSettings(void){
    std::cout << "Load Settings\n";
    std::ifstream inputFile("settings.bin", std::ios::binary);
    inputFile.read(reinterpret_cast<char *>(&userKey), sizeof(userKey));
    std::cout<<userKey<<"\n";
    inputFile.read(reinterpret_cast<char *>(&windowSettings), sizeof(windowSettings));
    std::cout<<windowSettings.fullScreen 
                << "\t"<< windowSettings.screenHeight
                << "\t"<< windowSettings.screenWidth
                << "\t"<< windowSettings.windowHeight
                << "\t"<< windowSettings.windowWidth
                <<"\n";
    inputFile.close();
}

//Get monitor resolution and standardize to fullscreen 16:9. create key and save
void GameApplication::createSettings(void){
    std::cout << "Initialize Settings\n";
    windowSettings.fullScreen = true;
    //windowSettings.monitorId = 0;
    windowSettings.screenHeight = GetScreenHeight();
    windowSettings.screenWidth = GetScreenWidth();
    windowSettings.windowHeight = windowSettings.screenHeight;
    windowSettings.windowWidth = windowSettings.screenWidth;

    if (windowSettings.windowHeight >= 1080 || windowSettings.windowWidth >= 1920){
        windowSettings.windowHeight = 1080;
        windowSettings.windowWidth =  1920;
    }
    else {
        windowSettings.windowHeight = 720;
        windowSettings.windowWidth =  1280;
    }


    //Generate UserKey
    const std::string alphabet = "!#$%^&*(){}=-+/abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::string::size_type> dist(0, alphabet.size()-1);

    for (int i = 0 ; i < MAX_LENGTH_KEY; i++){
        userKey[i] = alphabet[dist(rng)];
    }
    userKey[MAX_LENGTH_KEY] = '\0';
    std::cout<< "Userkey: "<< userKey << "\n";


    //Save settings
    std::ofstream outputFile("settings.bin", std::ios::binary);
    outputFile.write(reinterpret_cast<const char *>(&userKey), sizeof(userKey));
    outputFile.write(reinterpret_cast<const char *>(&windowSettings), sizeof(windowSettings));
    outputFile.close();
}



void GameApplication::loadSprites(void){
    //Load Menu Textures
    for (auto &fileIterator : std::filesystem::directory_iterator("assets/sprites/menu")){
        char fileId = fileIterator.path().filename().string()[0];
        sprites.menuSprites[((int)fileId)- 48] = LoadTexture((fileIterator.path()).string().c_str());
    }
    return;
}

void GameApplication::unloadSprites(void){
    for (int i = 0; i < N_MENU_TEXTURES; i++){
        UnloadTexture(sprites.menuSprites[i]);
    }
    return;
}

void GameApplication::changeResolution(void){

    return;
}