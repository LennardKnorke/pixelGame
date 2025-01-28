#include "menu.hpp"


MainMenu::MainMenu(sf::RenderWindow *window, Cursor *cursor, settings_class *ps, asset_manager *assets, stateInfo *stateTracker, bool dev){
    this->DEV_MODE = dev;
    if (DEV_MODE){
        std::cout << "MAIN MENU INITIALIZED" << std::endl;
    }
    this->cursor = cursor;
    this->settings = ps;
    this->assets = assets;
    this->window = window;
    this->stateTracker = stateTracker;

    //Read every save name/path for ingame buttons
    availableSaveFiles = read_all_save_summaries();

    //Set up menu buttons
    setUpMenuButtons();

    //prepare a warning message for errors
    warningMessage = new sf::Text(assets->font,"", GAMEFONT_SIZE);
    initErrorMessage();
    //assets->backgroundMusic[0].play();



    this->running = true;
}

MainMenu::~MainMenu(void){
    //delete all buttons
    for (button* objc : menuButtons){
        delete objc;
    }
    menuButtons.clear();
    availableSaveFiles.clear();
}

void MainMenu::run(void){
    window->setActive(true);
    while (running){
        this->draw();
        this->handleEvents();
        this->update();
    }


    return;
}



void MainMenu::draw(){
    window->clear(sf::Color::Black);
    if (menuWarning == menuPopUps::noPopUp){
        drawMenuButtons();
        if (!writing){//No text input?->Draw cursor
            cursor->draw();
        }
    }
    else {
        drawMenuPopUp();
    }
    window->display();
}

void MainMenu::handleEvents(){
    while (const std::optional event = window->pollEvent()){
        //Close window
        if (!window->hasFocus()){
            if (event->is<sf::Event::Closed>()){
                std::cout << "Closing window" << std::endl;
                window->close();
                running = false;
                break;
            }
            continue;
        }

        // Handle events while window is in focus
        // 1. Keypresses
        if (event->is<sf::Event::KeyPressed>()){
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
                escapeKeyPressed();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
                enterKeyPressed();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Delete) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace)){
                deleteKeyPressed();
            }

            // temporary fullscreen toggle for graphic practice
            else {
                /*
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2)){
                    sf::VideoMode new_desktop;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1)){
                        new_desktop = sf::VideoMode::getDesktopMode();
                        window->create(new_desktop, "ReFrAcTuReD", sf::Style::Fullscreen);
                        settings->res.fullscreen = true;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2)){
                        new_desktop = sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 32);
                        window->create(new_desktop, "ReFrAcTuReD", sf::Style::Titlebar | sf::Style::Close);
                        settings->res.fullscreen = false;
                    }

                    settings->res.width = new_desktop.width;
                    settings->res.height = new_desktop.height;
                    settings->res.scaling = sf::Vector2f((float)settings->res.width / (float)DEFAULT_WINDOW_WIDTH, (float)settings->res.height / (float)DEFAULT_WINDOW_HEIGHT);
                    
                    
                    window->setFramerateLimit(60);
                    window->setMouseCursorGrabbed(true);
                    window->setMouseCursorVisible(false);

                    if (DEV_MODE){
                        std::cout << "Toggle resolution:" << std::endl;
                        std::cout << "\tFullscreen: " << settings->res.fullscreen << std::endl;
                        std::cout << "\tResolution:\t" << settings->res.width << "x\t" << settings->res.height << std::endl;
                    }
                    // Rescale buttons and textures
                    for (button *buttTmp : menuButtons){
                        buttTmp->changeResolution(settings->res.width, settings->res.height, settings->res.scaling);
                    }
                    backgroundSprite.setScale(settings->res.scaling);
                    cursor->changeResolution(DEV_MODE);

                    settings->save();

                }*/
            }
            
        // 2. Text input
        }
        else if (event->is<sf::Event::TextEntered>()){
            textEntered(event);
        }
        // 3. Mouse input
        else if (event->is<sf::Event::MouseButtonPressed>()){
            mouseButtonPressed();
        }
    }
}

void MainMenu::update(){
    if (currentLayer == mainMenuLayerId::game || currentLayer == mainMenuLayerId::leave){
        if (currentLayer == mainMenuLayerId::leave){
            stateTracker->setAppState(appState::EXIT);
        }
        running = false;
        return;
    }

    if (window->hasFocus() && menuWarning == menuPopUps::noPopUp){
        for (button* butt : menuButtons){
            if (butt->layer == currentLayer){
                butt->update(cursor->returnPosition());
            }
        }
        if (!writing){
            cursor->update();
        }
        getMenuPicks();
    }
}






void MainMenu::setUpMenuButtons(void){
    //layersId::Base
    short maxButtons = 3;
    std::vector<std::string> text = {"Play", "Settings", "Exit"};
    std::vector<mainMenuLayerId> followUpsLayers = {mainMenuLayerId::modeSelection, mainMenuLayerId::options, mainMenuLayerId::leave};
    for (short i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(i, maxButtons,text[i], mainMenuLayerId::base, followUpsLayers[i], settings->res.width, settings->res.height, settings->res.scaling, assets->font, clickbuttonTypes::standart));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::options (TO DO)
    maxButtons = 1;
    text = {"Toggle Fullscreen"};
    followUpsLayers = {mainMenuLayerId::options};
    for (short i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(i, maxButtons, text[i], mainMenuLayerId::options, followUpsLayers[i], settings->res.width, settings->res.height, settings->res.scaling, assets->font, clickbuttonTypes::graphic));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::modeSelection
    maxButtons = 3;
    text = {"Singleplayer", "Host", "Join"};
    followUpsLayers = {mainMenuLayerId::hosting, mainMenuLayerId::hosting, mainMenuLayerId::joining};
    for (short i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(i, maxButtons, text[i], mainMenuLayerId::modeSelection, followUpsLayers[i], settings->res.width, settings->res.height, settings->res.scaling, assets->font, clickbuttonTypes::standart));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Joining
    maxButtons = 3;
    text = {"Enter Host Ip", "(Optional) Enter Host Port", "Connect"};
    followUpsLayers = {mainMenuLayerId::joining, mainMenuLayerId::joining, mainMenuLayerId::game};
    for (short i = 0; i < maxButtons - 1; i++){
        menuButtons.push_back(new textButton(i, maxButtons, text[i], mainMenuLayerId::joining, followUpsLayers[i], settings->res.width, settings->res.height, settings->res.scaling, assets->font, textbuttonTypes::adress));
    }
    menuButtons.push_back(new ClickButton(2, maxButtons, text[2], mainMenuLayerId::joining, mainMenuLayerId::game, settings->res.width, settings->res.height, settings->res.scaling, assets->font, clickbuttonTypes::standart));
    text.clear();
    followUpsLayers.clear();


    //layersId::Hosting
    if (availableSaveFiles.size() < MAX_SAVES){
        maxButtons = 1 + availableSaveFiles.size();
        menuButtons.push_back(new textButton(0, maxButtons, std::string("Start New Game"), mainMenuLayerId::hosting, mainMenuLayerId::game, settings->res.width, settings->res.height, settings->res.scaling, assets->font, textbuttonTypes::newSafe));
    }    
    for (long long unsigned i = 0; i < availableSaveFiles.size(); i++){
        short id;
        if (availableSaveFiles.size() < MAX_SAVES){
            id = i+1;
            maxButtons = availableSaveFiles.size() + 1;
        }
        else {
            maxButtons = MAX_SAVES;
            id = i;
        }
        menuButtons.push_back(new ClickButton(id, maxButtons, availableSaveFiles[i].name, mainMenuLayerId::hosting, mainMenuLayerId::game, settings->res.width, settings->res.height, settings->res.scaling, assets->font, clickbuttonTypes::saveFile));
    }

    text.clear();
    followUpsLayers.clear();
}

void MainMenu::initErrorMessage(void){
    if (menuWarning == menuPopUps::noPopUp){
        warningMessage->setString("");
    }
    else if (menuWarning == menuPopUps::invalidName){
        warningMessage->setString("Chosen Name is already taken. Press Escape to continue");
    }
    else if (menuWarning == menuPopUps::tooManySaves){
        warningMessage->setString("Too many saves. Delete at least one save to create a new one");
    }
    else if (menuWarning == menuPopUps::invalidIp){
        warningMessage->setString("Invalid IP Adress. Enter a valid IP Adress (port < 65535). Press Escape to continue");
    }
    else if (menuWarning == menuPopUps::saveError){
        warningMessage->setString("Error creating save file. Please try again. Press Escape to continue");
    }
    else {
        warningMessage->setString("UNEXPECTED ERROR: UNDEFINED WARNING. menu.cpp\n");
    }
    warningMessage->setFillColor(sf::Color::White);
    warningMessage->setFont(assets->font);
    warningMessage->setStyle(sf::Text::Style::Regular);
    warningMessage->setCharacterSize(GAMEFONT_SIZE);
    warningMessage->setPosition(sf::Vector2f((settings->res.width/2) - (warningMessage->getGlobalBounds().size.x / 2), (settings->res.height / 2) - (warningMessage->getGlobalBounds().size.y / 2)));

    return;
}



    
    
void MainMenu::drawMenuButtons(){
    for (button* buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer){
            window->draw(*buttTmp->text);
        }
    }
}

void MainMenu::drawMenuPopUp(){
    window->draw(*warningMessage);
}



void MainMenu::escapeKeyPressed(void){
    //0. If there is a warning message, clear it
    if (menuWarning != menuPopUps::noPopUp){
        menuWarning = menuPopUps::noPopUp;
        return;
    }
    
    //1. cancel any input currently ongoing
    if (writing){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                textButton *buttonPointer = dynamic_cast<textButton*>(buttTmp);
                if (buttonPointer && buttonPointer->activeInput){
                    buttonPointer->resetInput();
                    writing = false;
                    return;
                }
            }
        }
        return;
    }

    //2. Otherwise go back a layer. reset buttons if necessary
    if (currentLayer == mainMenuLayerId::hosting || currentLayer == mainMenuLayerId::joining){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer){
                textButton *buttonPointer = dynamic_cast<textButton*>(buttTmp);
                if (buttonPointer && buttonPointer->layer == currentLayer){
                    buttonPointer->resetInput();
                }
            }
        }
    }
    currentLayer = getPreviousLayer();
}



mainMenuLayerId MainMenu::getPreviousLayer(){
    if (currentLayer == mainMenuLayerId::base){
        return mainMenuLayerId::leave;
    }
    if (currentLayer == mainMenuLayerId::hosting || currentLayer == mainMenuLayerId::joining){
        return mainMenuLayerId::modeSelection;
    }
    return mainMenuLayerId::base;
    

    std::cout << "ERROR: EXITED FROM NON EXISTING LAYER. menu.cpp\n";
    return leave;
}



void MainMenu::enterKeyPressed(void){
    //1. Do nothing
    if (!writing){
        return;
    }
    //2. end error message
    if (menuWarning != menuPopUps::noPopUp){
        menuWarning = menuPopUps::noPopUp;
        return;
    }
    //3. accept input.
    for (button *buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer && buttTmp->focus){
            textButton *buttonPointer = dynamic_cast<textButton*>(buttTmp);
            if (buttonPointer && buttonPointer->activeInput){
                std::string userText = buttonPointer->userText;
                // 3.1 new safe initialized
                if (buttonPointer->type == textbuttonTypes::newSafe){
                    if (userText.empty()){
                        menuWarning = menuPopUps::invalidName;
                        initErrorMessage();
                        buttonPointer->resetInput();
                        writing = false;
                        return;
                    }
                    if (createSave(userText)){
                        currentLayer = mainMenuLayerId::game;
                        buttonPointer->activeInput = false;
                        writing = false;
                        return;
                    }
                    else {
                        initErrorMessage();
                        std::cout << "ERROR checkpoint" << std::endl;
                        buttonPointer->resetInput();
                        writing = false;
                        return;
                    }
                    return;
                }
                // 3.2 entered ip adress
                else if (buttonPointer->type == textbuttonTypes::adress){
                    if (userText.empty()){
                        menuWarning = menuPopUps::invalidIp;
                        initErrorMessage();
                        buttonPointer->resetInput();
                        writing = false;
                        return;
                    }
                    if (buttonPointer->idx == 0){
                        stateTracker->next.getConnectTo().ip = sf::IpAddress::resolve(userText);
                        if (stateTracker->next.getConnectTo().ip == std::nullopt){
                            if (DEV_MODE){
                                std::cout << "Invalid IP entered. menu.cpp\n";
                            }
                            menuWarning = menuPopUps::invalidIp;
                            initErrorMessage();
                            buttonPointer->resetInput();
                        }
                        writing = false;
                        buttonPointer->activeInput = false;
                        return;
                    }
                    else {
                        unsigned int port = std::stoi(userText);
                        if (port < 2024 || port > 65535){
                            menuWarning = menuPopUps::invalidIp;
                            initErrorMessage();
                            buttonPointer->resetInput();
                            writing = false;
                            return;
                        }
                        stateTracker->next.getConnectTo().port = port;
                        buttonPointer->activeInput = false;
                        writing = false;
                        return;
                    }
                }
            }
        }
    }
}
 


bool MainMenu::createSave(std::string newSaveName){
    if (availableSaveFiles.size() >= MAX_SAVES){
        menuWarning = menuPopUps::tooManySaves;
        return false;
    }
    for (gamesave_summary tmp : availableSaveFiles){
        if (tmp.name == newSaveName){
            menuWarning = menuPopUps::invalidName;
            return false;
        }
    }
    if (createNewSafeFile(newSaveName)){
        availableSaveFiles = read_all_save_summaries();
        return true;
    }
    else {
        menuWarning = menuPopUps::saveError;
        return false;
    }
}


bool MainMenu::createNewSafeFile(std::string newSaveName){
    gamesave_summary newSave;
    std::string newFileName = "WORLD_";
    std::string newpathName = "sav/"+newFileName;
    int i = 0;

    while (std::filesystem::exists(newpathName+std::to_string(i)+".SAV") && i < 10000){
        i++;
    }
    if (i == 10000){
        std::cout << "UNUSUAL AMOUNT OF SAVES. menu.cpp\n";
        return false;
    }

    newpathName = newpathName+std::to_string(i);
    newpathName = newpathName +  ".SAV";
    std::cout <<newpathName <<std::endl;

    

    //Set up variables for uninitialized World save files
    newSave.name = newSaveName;
    newSave.path = newpathName;
    newSave.initialized = false;

    //Save the uninitiliazed file (only needs to be done once)
    //As soon as the file is opened and modified for the first time will initialize the save
    std::ofstream file(newpathName, std::ios::binary);

    writeStrToFile(file,newSave.name);

    //Save whether save has been initialized
    file.write(reinterpret_cast<char*>(&newSave.initialized), sizeof(bool));

    file.close(); 

    stateTracker->next.getHostFile().file = newSave.path;   //REMEMBER THE PATH FOR THE MAIN APPLICATION TO LOAD THE NEW SAVE

    std::cout << "New Save Created: " << newSave.name << ". Filepath: "<<  newSave.path << std::endl;
    return true;
}


void MainMenu::deleteKeyPressed(void){
    //1. Remove Last input
    if (writing){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                textButton *buttonPointer = dynamic_cast<textButton*>(buttTmp);
                if (buttonPointer && buttonPointer->activeInput){
                    buttonPointer->delLastInput(sf::Vector2u(settings->res.width, settings->res.height));
                    return;
                }
            }
        }
    }

    //3. Delete Save (TO DO)
    if (currentLayer == mainMenuLayerId::hosting){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                //DELETE SAVE! PENDING
                return;
            }
        }
    }
}



void MainMenu::textEntered(std::optional<sf::Event> event){
    //1. No text input allowed
    if (writing == false){
        return;
    }
    //2. add valid characters to display text
    for (button *buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer && buttTmp->focus){
            textButton *textPointer = dynamic_cast<textButton*>(buttTmp);
            if (textPointer){
                auto *charEntered = event->getIf<sf::Event::TextEntered>();
                textPointer->addInput(static_cast<char>(charEntered->unicode), sf::Vector2u(settings->res.width, settings->res.height));
                return;
            }
        }
    }
}



void MainMenu::mouseButtonPressed(void){
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && writing == false){

        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                ClickButton *clickPointer = dynamic_cast<ClickButton*>(buttTmp);
                if (clickPointer){
                    if (clickPointer->type == clickbuttonTypes::saveFile){
                        for (gamesave_summary tmp : availableSaveFiles){
                            if (tmp.name == clickPointer->stringText){
                                stateTracker->next.getHostFile().file = tmp.path;
                                break;
                            }
                        }
                    }
                    // Joining. Change to "Game" layer if the "Connect" button is pressed and the IP and Port are valid
                    if (currentLayer == mainMenuLayerId::joining && clickPointer->idx == 2 && stateTracker->next.getConnectTo().ip != std::nullopt && stateTracker->next.getConnectTo().port > 2024 && stateTracker->next.getConnectTo().port < 65534){
                        currentLayer = mainMenuLayerId::game;
                    }
                    else if (currentLayer == mainMenuLayerId::options && clickPointer->idx == 0){
                        //changeFullscreen(window);
                    }
                    //Except for the "Connect" button in the "Joining" layer, which requires a valid IP and Port to change to "Game" layer
                    else {
                        currentLayer = clickPointer->layer_next;
                    }
                    
                }

                textButton *textPointer = dynamic_cast<textButton*>(buttTmp);
                if (textPointer){
                    textPointer->activeInput = true;
                    writing = true;
                    return;
                }
            }
        }
    }
}



void MainMenu::getMenuPicks(void){
    if (stateTracker->next.mode != gameMode::undefined && currentLayer == mainMenuLayerId::base){
        stateTracker->next.mode = gameMode::undefined;
        return;
    }

    if (currentLayer == mainMenuLayerId::modeSelection){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                if (buttTmp->idx == 0){
                    stateTracker->next.mode = gameMode::SINGLEPLAYER;
                }
                else if (buttTmp->idx == 1){
                    stateTracker->next.mode = gameMode::MULTIPLAYER_HOST;
                }
                else if (buttTmp->idx == 2){
                    stateTracker->next.mode = gameMode::MULTIPLAYER_JOIN;
                }
                return;
            }
        }
        stateTracker->next.mode = gameMode::undefined;
        return;
    }

}
