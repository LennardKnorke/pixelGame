#include "menu.hpp"
#include "application.hpp"
#include "menuButtons.hpp"

/////////////////////////////////////////////////////////////////////////////////
//APPLICATION HPP DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////

GAME_STATE Application::menuLoop(void){
    MainMenu *menu = new MainMenu(this);
    GAME_STATE nextState = menu->runMenu();

    if (nextState == GAME_STATE::GAME){
        mode = menu->mode;
    }
    delete menu;
    return nextState;
}

MainMenu::MainMenu(Application *appPointer){
    // Set Variables to control what to display in menu
    currentLayer = mainMenuLayerId::Base;
    menuWarning = menuPopUps::NoPopUp;
    allowTextInput = false;
    applicationPointer = appPointer;
    mode = gameMode::undefined;
    //Read every save name/path for ingame buttons
    readAllSaveSummaries();

    //Set up menu buttons
    setUpMenuButtons();

    //SetUpBackgroundSprite
    backgroundSprite.setTexture(applicationPointer->textures.menu[menuTextureIdxS::background]);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(applicationPointer->resolution.x/backgroundSprite.getGlobalBounds().getSize().x, applicationPointer->resolution.y/backgroundSprite.getGlobalBounds().getSize().y);

    //prepare a warning message for errors
    warningMessage = initErrorMessage(applicationPointer->gameFont);

    // Set up window pointer
    window = &(applicationPointer->window);
}

void MainMenu::readAllSaveSummaries(void){
    std::filesystem::path pathDirectory = std::string("sav/");
    for (auto &iterator : std::filesystem::recursive_directory_iterator(pathDirectory)){
        if (iterator.path().extension() == std::filesystem::path(".SAV")){
            availableSaveFiles.push_back(loadSaveSummary(iterator.path().string()));
        }
    }
}

gameSaveSummary MainMenu::loadSaveSummary(const std::string &filename){
    std::ifstream inputFile(filename, std::ios::binary);
    gameSaveSummary sumTmp;

    readStrOfFile(inputFile, sumTmp.game);

    readStrOfFile(inputFile, sumTmp.file);

    readStrOfFile(inputFile, sumTmp.path);

    inputFile.close();
    return sumTmp;
}


void MainMenu::setUpMenuButtons(void){
    //layersId::Base
    mainMenuLayerId currentLayer = mainMenuLayerId::Base;
    int maxButtons = 3;
    std::vector<std::string> text = {"Play", "Settings", "Exit"};
    std::vector<mainMenuLayerId> followUpsLayers = {mainMenuLayerId::GameMode, mainMenuLayerId::Settings, mainMenuLayerId::leave};
    for (int i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Settings
    currentLayer = mainMenuLayerId::Settings;
    maxButtons = 2;
    text = {"Graphics", "Controls"};
    followUpsLayers = {mainMenuLayerId::Graphic, mainMenuLayerId::Controls};
    for (int i = 0; i < maxButtons; i++){

    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Graphic
    /*
    layersId currentLayer = layersId::Graphic;
    maxButtons = 5;
    text.push_back("1920 x 1080"); text.push_back("1600 x 900"); "1280 x 720";"1024 x 576";"960 x 540";
    followUpsLayers.push_back(layersId::Graphic);
    followUpsLayers.push_back(layersId::Graphic);
    followUpsLayers.push_back(layersId::Graphic);
    followUpsLayers.push_back(layersId::Graphic);
    followUpsLayers.push_back(layersId::Graphic);
    for (int i = 0; i < maxButtons; i++){

    }
    text.clear();
    followUpsLayers.clear();
    */


    //layersId::Control
    maxButtons = 0;
    for (int i = 0; i < maxButtons; i++){

    }
    text.clear();
    followUpsLayers.clear();


    //layersId::GameMode
    currentLayer = mainMenuLayerId::GameMode;
    maxButtons = 3;
    text = {"Singleplayer", "Locally", "Online"};
    followUpsLayers = {mainMenuLayerId::Hosting, mainMenuLayerId::HostVsClient, mainMenuLayerId::HostVsClient};
    for (int i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::HostVsClient
    currentLayer = mainMenuLayerId::HostVsClient;
    maxButtons = 2;
    text = {"Host", "Join"};
    followUpsLayers = {mainMenuLayerId::Hosting, mainMenuLayerId::Joining};
    for (int i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Joining
    currentLayer = mainMenuLayerId::Joining;
    maxButtons = 2;
    text = {"Enter Host Ip", "Enter Host Port"};
    followUpsLayers = {mainMenuLayerId::final, mainMenuLayerId::final};
    for (int i = 0; i < maxButtons; i++){
        menuButtons.push_back(new adressButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Hosting
    currentLayer = mainMenuLayerId::Hosting;
    maxButtons = 1;
    text = {"Create New World"};
    followUpsLayers = {mainMenuLayerId::final};
    menuButtons.push_back(new newSafeButton(text[0], followUpsLayers[0], applicationPointer, maxButtons + availableSaveFiles.size(), 0, currentLayer));
    for (long long unsigned int i = 0; i < availableSaveFiles.size(); i++){
        menuButtons.push_back(new ProfileButton(availableSaveFiles[i].game, mainMenuLayerId::final, applicationPointer, availableSaveFiles.size()+maxButtons, i+1, mainMenuLayerId::Hosting));
    }

    text.clear();
    followUpsLayers.clear();
}

sf::Text MainMenu::initErrorMessage(sf::Font &font){
    sf::Text newText;
    newText.setString("");
    newText.setFillColor(sf::Color::White);
    newText.setFont(font);
    newText.setStyle(sf::Text::Style::Regular);
    newText.setCharacterSize(GAMEFONT_SIZE);
    return newText;
}

MainMenu::~MainMenu(void){
    //delete all buttons
    for (button* objc : menuButtons){
        delete objc;
    }
    menuButtons.clear();
    availableSaveFiles.clear();
}


GAME_STATE MainMenu::runMenu(void){
    applicationPointer->backgroundMusic[musicIdx::mainMenu].play();


    //  MAIN MENU LOOP
    //      1. DRAW
    //      2. Register Key/Mouse Input
    //      3. Update buttons/application features 
    while (currentLayer != mainMenuLayerId::final && currentLayer != mainMenuLayerId::leave){
    
        //  1. DRAW
        window->clear(sf::Color::Transparent);
        window->draw(backgroundSprite);
        if (menuWarning == menuPopUps::NoPopUp){
            drawMenuButtons();
            if (!allowTextInput){//No text input?->Draw cursor
                applicationPointer->cursor.draw(*window);
            }
        }
        else {
            drawMenuPopUp();
        }
        
        window->display();


        //  2. Handle Events
        sf::Event event;
        while (window->pollEvent(event)){
            if (menuWarning != menuPopUps::NoPopUp){
                inputErrorPopUp(event);
            }
            else {
                if (event.type == sf::Event::KeyPressed){
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                        escapeKeyPressed();
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
                        enterKeyPressed();
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) || sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
                        deleteKeyPressed();
                    }
                }
                else if (event.type == sf::Event::TextEntered){
                    textEntered(event);
                }
                else if (event.type == sf::Event::MouseButtonPressed ){
                    mouseButtonPressed();
                }
            }
        }
                        
        //  3. UPDATE BUTTONS
        if (currentLayer != mainMenuLayerId::final && currentLayer != mainMenuLayerId::leave && menuWarning == menuPopUps::NoPopUp){
            for (button* butt : menuButtons){
                if (butt->layer == currentLayer){
                    butt->update(applicationPointer->cursor.returnPosition());
                }
            }
            if (!allowTextInput){
                applicationPointer->cursor.update();
            }
            getMenuPicks();
        }
    }
    //End of visual menu loop. user chose either to exit or play

    
    if (currentLayer == mainMenuLayerId::leave){
        std::cout << "Leaving Application!\n";
        return GAME_STATE::QUIT;
    }
    std::cout << "Starting Game!\n";
    return GAME_STATE::GAME;
}    
    
    
void MainMenu::drawMenuButtons(void){
    for (button* buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer){
            buttTmp->draw(*window);
        }
    }
}  



void MainMenu::inputErrorPopUp(sf::Event &event){
    if (menuWarning == menuPopUps::InvalidName){
        if(event.type == sf::Event::KeyPressed && (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))){
            menuWarning = menuPopUps::NoPopUp;
        }
    }
    return;
}



void MainMenu::drawMenuPopUp(void){
    window->draw(warningMessage);
}



void MainMenu::escapeKeyPressed(void){
    //1. cancel input given
    if (allowTextInput){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
                if (newSavePointer && newSavePointer->activeInput){
                    newSavePointer->resetInput();
                    allowTextInput = false;
                    return;
                }
                adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
                if (adressPointer && adressPointer->activeInput){
                    adressPointer->resetInput();
                    allowTextInput = false;
                    return;
                }
            }
        }
        return;
    }

    //2. Otherwise reset all input and go back a layer
    if (currentLayer == mainMenuLayerId::Hosting || currentLayer == mainMenuLayerId::Joining){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer){
                newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
                adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
                if (newSavePointer && currentLayer == mainMenuLayerId::Hosting){
                    newSavePointer->resetInput();
                }
                else if (adressPointer && currentLayer == mainMenuLayerId::Joining){
                    adressPointer->resetInput();
                }
            }
        }
    }
    currentLayer = getPreviousLayer();
}



mainMenuLayerId MainMenu::getPreviousLayer(){
    if (currentLayer == Base){
        return leave;
    }
    if (currentLayer == Settings){
        return Base;
    }
    if (currentLayer == GameMode){
        return Base;
    }
    if (currentLayer == HostVsClient){
        return GameMode;
    }
    if (currentLayer == Joining){
        return HostVsClient;
    }
    if (currentLayer == Hosting){
        if (mode == gameMode::Single){
            return GameMode;
        }
        return HostVsClient;
    }
    if (currentLayer == Graphic){
        return Settings;
    }
    if (currentLayer == Controls){
        return Settings;
    }

    std::cout << "ERROR: EXITED FROM NON EXISTING LAYER. menu.cpp\n";
    return final;
}



void MainMenu::enterKeyPressed(void){
    //1. Do nothing
    if (allowTextInput == false){
        return;
    }
    
    for (button *buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer && buttTmp->focus){

            //2.EITHER INITIATE NEW SAFE
            newSafeButton *safePointer = dynamic_cast<newSafeButton*>(buttTmp);
            if (safePointer && safePointer->activeInput && currentLayer == mainMenuLayerId::Hosting){
                std::string newSaveName = safePointer->userText;
                if (createSave(newSaveName)){
                    currentLayer = mainMenuLayerId::final;
                    safePointer->activeInput = false;
                    allowTextInput = false;
                    return;
                }
                safePointer->resetInput();
                allowTextInput = false;
                return;
            }

            //3. OR FILL IN PORTS/ADRESS
            adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
            if (adressPointer && adressPointer->activeInput && currentLayer == mainMenuLayerId::Joining){
                if (adressPointer->n_button == 0){
                    applicationPointer->hostAdress.ip = sf::IpAddress(adressPointer->userText);
                }
                else {
                    applicationPointer->hostAdress.port = static_cast<unsigned short>(std::strtoul(adressPointer->userText.c_str(), NULL, 0));
                }
                adressPointer->activeInput = false;
                allowTextInput = false;
                if (applicationPointer->hostAdress.ip != sf::IpAddress::None && applicationPointer->hostAdress.port != 0){
                    currentLayer = mainMenuLayerId::final;
                }
                return;
            }
        }
    }
}
 


bool MainMenu::createSave(std::string newSaveName){
    if (availableSaveFiles.size() >= MAX_N_SAVES){
        menuWarning = menuPopUps::TooManySaves;
        return false;
    }
    for (gameSaveSummary tmp : availableSaveFiles){
        if (tmp.game == newSaveName){
            menuWarning = menuPopUps::InvalidName;
            return false;
        }
    }
    return createNewSafeFile(newSaveName, (applicationPointer->hostAdress.pathSave));
}


bool MainMenu::createNewSafeFile(std::string newSaveName, std::string &pathToLoad){
    gameSaveSummary newSave;
    std::string newFileName = "WORLD_";
    std::string newpathName = "sav/"+newFileName;
    int i = 0;
    while (fileExists(newpathName+std::to_string(i)+".SAV")){
        i++;
    }
    newFileName = newFileName+std::to_string(i);
    newpathName = newpathName+std::to_string(i);
    newFileName = newFileName +  ".SAV";
    newpathName = newpathName +  ".SAV";
    std::cout <<newpathName <<std::endl;

    pathToLoad = newpathName;   //REMEMBER THE PATH FOR THE MAIN APPLICATION TO LOAD THE NEW SAVE

    //Set up variables for uninitialized World save files
    newSave.game = newSaveName;
    newSave.file = newFileName;
    newSave.path = newpathName;
    bool initialized = false;

    //Save the uninitiliazed file (only needs to be done once)
    //As soon as the file is opened and modified for the first time will initialize the save
    std::ofstream file(newpathName, std::ios::binary);

    writeStrToFile(file,newSave.game);

    writeStrToFile(file,newSave.file);

    writeStrToFile(file,newSave.path);

    //Save whether save has been initialized
    file.write(reinterpret_cast<char*>(&initialized), sizeof(bool));

    file.close(); 
    return true;
}


void MainMenu::deleteKeyPressed(void){
    //1. Remove Last input
    if (allowTextInput){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
                if (newSavePointer){
                    newSavePointer->delLastInput(applicationPointer->resolution);
                    return;
                }

                adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
                if (adressPointer){
                    adressPointer->delLastInput(applicationPointer->resolution);
                    return;
                }
            }
        }
    }
    //2. Do nothing
    if (currentLayer != mainMenuLayerId::Hosting){
        return;
    }

    //3. Delete Save
    for (button *buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer && buttTmp->focus){
            newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
            if (newSavePointer){
                //DELETE SAVE! PENDING
                return;
            }
        }
    }
}



void MainMenu::textEntered(sf::Event ev){
    //1. No text input allowed
    if (allowTextInput == false){
        return;
    }
    //2. add valid characters to display text
    for (button *buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer && buttTmp->focus){
            newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
            adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
            if (newSavePointer && validCharacterInput(ev.text.unicode, newSavePointer->userText.size(), buttTmp->n_button)){
                newSavePointer->addInput(ev.text.unicode, applicationPointer->resolution);
                return;
            }
            if (adressPointer && validCharacterInput(ev.text.unicode, adressPointer->userText.size(), buttTmp->n_button)){
                adressPointer->addInput(ev.text.unicode, applicationPointer->resolution);
                return;
            }
            return;
        }
    }
}



bool MainMenu::validCharacterInput(sf::Uint32 c, int activeLength, int n){
    //Allow only characters for the savename
    if (currentLayer == mainMenuLayerId::Hosting){//Entering a new savename only allow "a-z", "A-Z", "0-9".
        if (activeLength >= maxInputLengths::saveName){
            return false;
        }
        if ((c < 48 || c > 57) && (c < 65 || c > 90) && (c < 97 || c > 122) && c != 32){
            return false;
        }
    }
    //Allow only valid Ip's and Ports
    if (currentLayer == mainMenuLayerId::Joining) {//Entering "0-9", "." , ":" for the ip
        //First check port
        if (n == 1){
            if (activeLength >= maxInputLengths::port){
                return false;
            }
            if (c < 48 || c > 57){
                return false;
            }
        }
        //Then ip for either local or public validty
        if (mode == gameMode::Local_client){
            if (activeLength >= maxInputLengths::localIp){
                return false;
            }
            if ((c < 48 || c > 57) && c != 46){
                return false;
            }
        }
        if (mode == gameMode::Online_client){
            if (activeLength >= maxInputLengths::publicIp){
                return false;
            }
            if ((c < 48 || c > 57) && c != 46){
                return false;
            }
        }
    } 
    return true;
}



void MainMenu::mouseButtonPressed(void){
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && allowTextInput == false){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                ClickButton *clickPointer = dynamic_cast<ClickButton*>(buttTmp);
                if (clickPointer){
                    currentLayer = buttTmp->nextLayer;
                    return;
                }

                adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
                if (adressPointer){
                    adressPointer->activeInput = true;
                    allowTextInput = true;
                    return;
                }

                newSafeButton *safePointer = dynamic_cast<newSafeButton*>(buttTmp);
                if (safePointer){
                    safePointer->activeInput = true;
                    allowTextInput = true;
                    return;
                }
                
                ProfileButton *profPointer = dynamic_cast<ProfileButton*>(buttTmp);
                if (profPointer){
                    setActiveSafe(profPointer->stringText, &(applicationPointer->hostAdress.pathSave));
                    currentLayer = buttTmp->nextLayer;
                    return;
                }
                return;
            }
        }
    }
}



void MainMenu::setActiveSafe(std::string saveName, std::string *pathToBeLoaded){
    for (gameSaveSummary sum : availableSaveFiles){
        if (sum.game == saveName){
            pathToBeLoaded->clear();
            *pathToBeLoaded = sum.path;
        }
    }
}



void MainMenu::getMenuPicks(void){
    if (mode != gameMode::undefined && (currentLayer == mainMenuLayerId::Base || currentLayer == mainMenuLayerId::Settings || currentLayer == mainMenuLayerId::Controls ||currentLayer == mainMenuLayerId::Graphic)){
        mode = gameMode::undefined;
        return;
    }

    if (currentLayer == mainMenuLayerId::GameMode){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                if (buttTmp->n_button == 0){
                    mode = gameMode::Single;
                }
                else if (buttTmp->n_button == 1){
                    mode = gameMode::local;
                }
                else if (buttTmp->n_button == 2){
                    mode = gameMode::online;
                }
                return;
            }
        }
        mode = gameMode::undefined;
        return;
    }

    if (currentLayer == mainMenuLayerId::HostVsClient){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                //Choosing to be host
                if (buttTmp->n_button == 0){
                    if (mode == gameMode::local|| mode == gameMode::Local_client){
                        mode = gameMode::Local_host;
                    }
                    else if(mode == gameMode::online|| mode == gameMode::Online_client){
                        mode = gameMode::Online_host;
                    }
                    return;
                }
                //Choosing to be client
                else if (buttTmp->n_button == 1){
                    if (mode == gameMode::local || mode == gameMode::Local_host){
                        mode = gameMode::Local_client;
                    }
                    else if(mode == gameMode::online|| mode == gameMode::Online_host){
                        mode = gameMode::Online_client;
                    }
                    return;
                }
            }
        }
    }

}


void MainMenu::setMenuPopUpMessage(void){
    /*
    if (PopUp == menuPopUps::InvalidName){
        warningMessage->setString("Choose a new/valid name for your save\nPress Enter\\Escape to continue");
    }
    else if (PopUp == menuPopUps::TooManySaves){
        warningMessage->setString("Too many saves. Delete at least one save to create a new one");
    }
    else if (PopUp == menuPopUps::deleteSave){
        warningMessage->setString("TMP");
    }
    else if (PopUp == menuPopUps::NoPopUp){
        warningMessage->setString("");
    }
    warningMessage->setPosition((res.x/2) - (warningMessage->getGlobalBounds().getSize().x / 2), (res.y / 2) - (warningMessage->getGlobalBounds().getSize().y / 2));
    */
}











