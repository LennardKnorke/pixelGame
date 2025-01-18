#include "application.hpp"
#include "menu.hpp"
#include "menuButtons.hpp"

GAME_STATE Application::menuLoop(void){

    MainMenu *menu = new MainMenu(cursor, settings, assets.textures.menu, &assets.gameFont, &assets.backgroundMusic[musicIdx::mainMenu], DEV_MODE);
    GAME_STATE next_state;
    while (menu->running){
        menu->draw(&window);

        menu->handleEvents(&window);

        menu->update(&window);
    }
    // End of menu loop. Get next state and necessary tools
    if (menu->currentLayer == mainMenuLayerId::game){
        mode = menu->getGameMode();
        if (mode == gameMode::Join){
            loadingTools.ip = menu->getHostIp();
            loadingTools.port = menu->getHostPort();
            next_state = GAME_STATE::GAME;
        }
        else if (mode == gameMode::Host || mode == gameMode::Single){
            loadingTools.chosenSave = menu->getChosenSave();
            next_state = GAME_STATE::GAME;
        }
        else {
            next_state = GAME_STATE::QUIT;
            std::cout << "ERROR: UNDEFINED GAME MODE. application.cpp\n";
            error = errorCodes::Error;
        }
        
    }
    else{
        next_state = GAME_STATE::QUIT;
    }
    window.setActive(false);
    delete menu; // AVOID MEMORY LEAKS
    return next_state;
}


void MainMenu::draw(sf::RenderWindow *window){
    window->clear(sf::Color::Transparent);
    window->draw(backgroundSprite);
    if (menuWarning == menuPopUps::noPopUp){
        drawMenuButtons(window);
        if (!writing){//No text input?->Draw cursor
            cursor->draw(*window);
        }
    }
    else {
        drawMenuPopUp(window);
    }
    window->display();
}

void MainMenu::handleEvents(sf::RenderWindow *window){
    sf::Event event;
    while (window->pollEvent(event)){
        //Close window
        if (!window->hasFocus()){
            if (event.type == sf::Event::Closed){
                std::cout << "Closing window" << std::endl;
                window->close();
                running = false;
                break;
            }
            continue;
        }

        // Handle events while window is in focus
        // 1. Keypresses
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

            // temporary fullscreen toggle for graphic practice
            else {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1) || sf::Keyboard::isKeyPressed(sf::Keyboard::F2)){
                    sf::VideoMode new_desktop;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)){
                        new_desktop = sf::VideoMode::getDesktopMode();
                        window->create(new_desktop, "ReFrAcTuReD", sf::Style::Fullscreen);
                        settings->res.fullscreen = true;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)){
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

                }
            }
        // 2. Text input
        }
        else if (event.type == sf::Event::TextEntered){
            textEntered(event);
        }
        // 3. Mouse input
        else if (event.type == sf::Event::MouseButtonPressed ){
            mouseButtonPressed(window);
        }
    }
}

void MainMenu::update(sf::RenderWindow *window){
    if (currentLayer == mainMenuLayerId::game || currentLayer == mainMenuLayerId::leave){
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


MainMenu::MainMenu(Cursor *cursor, settings_class *ps, sf::Texture *bg_Texture, sf::Font *font, sf::Music *bg_music, bool dev){
    this->DEV_MODE = dev;
    if (DEV_MODE){
        std::cout << "MAIN MENU INITIALIZED" << std::endl;
    }
    
    this->font = font;
    this->bg_music = bg_music;
    this->cursor = cursor;
    this->settings = ps;
    
    //Read every save name/path for ingame buttons
    availableSaveFiles = read_all_save_summaries();

    //Set up menu buttons
    setUpMenuButtons();

    //SetUpBackgroundSprite
    backgroundSprite.setTexture(*bg_Texture);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(settings->res.width/backgroundSprite.getGlobalBounds().getSize().x, settings->res.height/backgroundSprite.getGlobalBounds().getSize().y);

    //prepare a warning message for errors
    initErrorMessage();
    this->bg_music->play();
}



void MainMenu::setUpMenuButtons(void){
    //layersId::Base
    short maxButtons = 3;
    std::vector<std::string> text = {"Play", "Settings", "Exit"};
    std::vector<mainMenuLayerId> followUpsLayers = {mainMenuLayerId::modeSelection, mainMenuLayerId::options, mainMenuLayerId::leave};
    for (short i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(i, maxButtons,text[i], mainMenuLayerId::base, followUpsLayers[i], settings->res.width, settings->res.height, settings->res.scaling, *font, clickbuttonTypes::standart));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::options (TO DO)
    maxButtons = 1;
    text = {"Toggle Fullscreen"};
    followUpsLayers = {mainMenuLayerId::options};
    for (short i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(i, maxButtons, text[i], mainMenuLayerId::options, followUpsLayers[i], settings->res.width, settings->res.height, settings->res.scaling, *font, clickbuttonTypes::graphic));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::modeSelection
    maxButtons = 3;
    text = {"Singleplayer", "Host", "Join"};
    followUpsLayers = {mainMenuLayerId::hosting, mainMenuLayerId::hosting, mainMenuLayerId::joining};
    for (short i = 0; i < maxButtons; i++){
        menuButtons.push_back(new ClickButton(i, maxButtons, text[i], mainMenuLayerId::modeSelection, followUpsLayers[i], settings->res.width, settings->res.height, settings->res.scaling, *font, clickbuttonTypes::standart));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Joining
    maxButtons = 3;
    text = {"Enter Host Ip", "(Optional) Enter Host Port", "Connect"};
    followUpsLayers = {mainMenuLayerId::joining, mainMenuLayerId::joining, mainMenuLayerId::game};
    for (short i = 0; i < maxButtons - 1; i++){
        menuButtons.push_back(new textButton(i, maxButtons, text[i], mainMenuLayerId::joining, followUpsLayers[i], settings->res.width, settings->res.height, settings->res.scaling, *font, textbuttonTypes::adress));
    }
    menuButtons.push_back(new ClickButton(2, maxButtons, text[2], mainMenuLayerId::joining, mainMenuLayerId::game, settings->res.width, settings->res.height, settings->res.scaling, *font, clickbuttonTypes::standart));
    text.clear();
    followUpsLayers.clear();


    //layersId::Hosting
    if (availableSaveFiles.size() < MAX_SAVES){
        maxButtons = 1 + availableSaveFiles.size();
        menuButtons.push_back(new textButton(0, maxButtons, std::string("Start New Game"), mainMenuLayerId::hosting, mainMenuLayerId::game, settings->res.width, settings->res.height, settings->res.scaling, *font, textbuttonTypes::newSafe));
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
        menuButtons.push_back(new ClickButton(id, maxButtons, availableSaveFiles[i].name, mainMenuLayerId::hosting, mainMenuLayerId::game, settings->res.width, settings->res.height, settings->res.scaling, *font, clickbuttonTypes::saveFile));
    }

    text.clear();
    followUpsLayers.clear();
}

void MainMenu::initErrorMessage(void){
    if (menuWarning == menuPopUps::noPopUp){
        warningMessage.setString("");
    }
    else if (menuWarning == menuPopUps::invalidName){
        warningMessage.setString("Chosen Name is already taken. Press Escape to continue");
    }
    else if (menuWarning == menuPopUps::tooManySaves){
        warningMessage.setString("Too many saves. Delete at least one save to create a new one");
    }
    else if (menuWarning == menuPopUps::invalidIp){
        warningMessage.setString("Invalid IP Adress. Enter a valid IP Adress (port < 65535). Press Escape to continue");
    }
    else if (menuWarning == menuPopUps::saveError){
        warningMessage.setString("Error creating save file. Please try again. Press Escape to continue");
    }
    else {
        warningMessage.setString("UNEXPECTED ERROR: UNDEFINED WARNING. menu.cpp\n");
    }
    warningMessage.setFillColor(sf::Color::White);
    warningMessage.setFont(*font);
    warningMessage.setStyle(sf::Text::Style::Regular);
    warningMessage.setCharacterSize(GAMEFONT_SIZE);
    warningMessage.setPosition((settings->res.width/2) - (warningMessage.getGlobalBounds().getSize().x / 2), (settings->res.height / 2) - (warningMessage.getGlobalBounds().getSize().y / 2));

    return;
}

MainMenu::~MainMenu(void){
    //delete all buttons
    for (button* objc : menuButtons){
        delete objc;
    }
    menuButtons.clear();
    availableSaveFiles.clear();
}

    
    
void MainMenu::drawMenuButtons(sf::RenderWindow *window){
    for (button* buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer){
            window->draw(buttTmp->text);
        }
    }
}

void MainMenu::drawMenuPopUp(sf::RenderWindow *window){
    window->draw(warningMessage);
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
                        hostIp = sf::IpAddress(userText);
                        if (hostIp == sf::IpAddress::None){
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
                        hostPort = port;
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

    chosen_save = newSave;   //REMEMBER THE PATH FOR THE MAIN APPLICATION TO LOAD THE NEW SAVE

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



void MainMenu::textEntered(sf::Event ev){
    //1. No text input allowed
    if (writing == false){
        return;
    }
    //2. add valid characters to display text
    for (button *buttTmp : menuButtons){
        if (buttTmp->layer == currentLayer && buttTmp->focus){
            textButton *textPointer = dynamic_cast<textButton*>(buttTmp);
            if (textPointer){
                textPointer->addInput(ev.text.unicode, sf::Vector2u(settings->res.width, settings->res.height));
                return;
            }
        }
    }
}



void MainMenu::mouseButtonPressed(sf::RenderWindow *window){
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && writing == false){

        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                ClickButton *clickPointer = dynamic_cast<ClickButton*>(buttTmp);
                if (clickPointer){
                    if (clickPointer->type == clickbuttonTypes::saveFile){
                        for (gamesave_summary tmp : availableSaveFiles){
                            if (tmp.name == clickPointer->stringText){
                                chosen_save = tmp;
                                break;
                            }
                        }
                    }
                    // Joining. Change to "Game" layer if the "Connect" button is pressed and the IP and Port are valid
                    if (currentLayer == mainMenuLayerId::joining && clickPointer->idx == 2 && hostIp != sf::IpAddress::None && hostPort > 2024 && hostPort < 65534){
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
    if (mode != gameMode::undefined && currentLayer == mainMenuLayerId::base){
        mode = gameMode::undefined;
        return;
    }

    if (currentLayer == mainMenuLayerId::modeSelection){
        for (button *buttTmp : menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                if (buttTmp->idx == 0){
                    mode = gameMode::Single;
                }
                else if (buttTmp->idx == 1){
                    mode = gameMode::Host;
                }
                else if (buttTmp->idx == 2){
                    mode = gameMode::Join;
                }
                return;
            }
        }
        mode = gameMode::undefined;
        return;
    }

}

gameMode MainMenu::getGameMode(void){
    return mode;
}
sf::IpAddress MainMenu::getHostIp(void){
    return hostIp;
}
unsigned short MainMenu::getHostPort(void){
    return hostPort;
}
gamesave_summary MainMenu::getChosenSave(void){
    return chosen_save;
}