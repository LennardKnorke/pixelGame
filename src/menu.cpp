#include "application.hpp"
#include "menu.hpp"
#include "menuButtons.hpp"

GAME_STATE Application::menuLoop(void){
    MainMenu *menu = new MainMenu(&window, &cursor,resolution, &assets.textures.menu[menuTextureIdxS::background], &assets.gameFont, &assets.backgroundMusic[musicIdx::mainMenu]);
    GAME_STATE nextState = menu->runMenu();

    if (nextState == GAME_STATE::GAME){
        mode = menu->getMode();
        if (mode == gameMode::Single || mode == gameMode::Host){
            loadingTools.chosenSave = menu->getChosenSave();
        }
        else if (mode == gameMode::Join){
            loadingTools.ip = menu->getHostIp();
            loadingTools.port = menu->getHostPort();
        }
        else {
            std::cout << "ERROR: UNDEFINED GAME MODE. application.cpp\n";
            nextState = GAME_STATE::QUIT;
        }
    }
    delete menu; // AVOID MEMORY LEAKS
    return nextState;
}

gameMode MainMenu::getMode(void){
    return this->mode;
}

sf::IpAddress MainMenu::getHostIp(void){
    return this->hostIp;
}

gamesave_summary MainMenu::getChosenSave(void){
    return this->chosen_save;
}
unsigned short MainMenu::getHostPort(void){
    return this->hostPort;
}

MainMenu::MainMenu(sf::RenderWindow *window, Cursor *cursor, sf::Vector2u res, sf::Texture *bg_Texture, sf::Font *font, sf::Music *bg_music){
    this->font = font;
    this->window = window;
    this->bg_music = bg_music;
    this->cursor = cursor;
    this->resolution = res;
    //Read every save name/path for ingame buttons
    availableSaveFiles = read_all_save_summaries();

    //Set up menu buttons
    setUpMenuButtons();

    //SetUpBackgroundSprite
    backgroundSprite.setTexture(*bg_Texture);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(res.x/backgroundSprite.getGlobalBounds().getSize().x, res.y/backgroundSprite.getGlobalBounds().getSize().y);

    //prepare a warning message for errors
    initErrorMessage();

}



void MainMenu::setUpMenuButtons(void){
    float position[2] = {0.0, 0.0};
    position[0] = resolution.x / 2;

    //layersId::Base
    unsigned short maxButtons = 3;
    std::vector<std::string> text = {"Play", "Settings", "Exit"};
    std::vector<mainMenuLayerId> followUpsLayers = {mainMenuLayerId::modeSelection, mainMenuLayerId::options, mainMenuLayerId::leave};
    for (unsigned short i = 0; i < maxButtons; i++){
        position[1] = resolution.y / float(maxButtons+1) * (i+1);
        menuButtons.push_back(new ClickButton(i, text[i], mainMenuLayerId::base, followUpsLayers[i], position, *font, clickbuttonTypes::standart));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::options (TO DO)
    maxButtons = 2;
    text = {"Graphics", "Controls"};
    followUpsLayers = {mainMenuLayerId::leave, mainMenuLayerId::leave};
    for (unsigned short i = 0; i < maxButtons; i++){

    }
    text.clear();
    followUpsLayers.clear();


    //layersId::modeSelection
    maxButtons = 3;
    text = {"Singleplayer", "Host", "Join"};
    followUpsLayers = {mainMenuLayerId::hosting, mainMenuLayerId::hosting, mainMenuLayerId::joining};
    for (unsigned short i = 0; i < maxButtons; i++){
        position[1] = resolution.y / float(maxButtons+1) * (i+1);
        menuButtons.push_back(new ClickButton(i, text[i], mainMenuLayerId::modeSelection, followUpsLayers[i], position, *font, clickbuttonTypes::standart));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Joining
    maxButtons = 3;
    text = {"Enter Host Ip", "(Optional) Enter Host Port", "Connect"};
    followUpsLayers = {mainMenuLayerId::joining, mainMenuLayerId::joining, mainMenuLayerId::game};
    for (unsigned short i = 0; i < maxButtons - 1; i++){
        position[1] = resolution.y / float(maxButtons+1) * (i+1);
        menuButtons.push_back(new textButton(i, text[i], mainMenuLayerId::joining, followUpsLayers[i], position, *font, textbuttonTypes::adress));
    }
    position[1] = resolution.y / float(maxButtons+1) * (maxButtons);
    menuButtons.push_back(new ClickButton(2, text[2], mainMenuLayerId::joining, mainMenuLayerId::game, position, *font, clickbuttonTypes::standart));
    text.clear();
    followUpsLayers.clear();


    //layersId::Hosting
    if (availableSaveFiles.size() < MAX_SAVES){
        maxButtons = 1 + availableSaveFiles.size();
        position[1] = resolution.y / float(maxButtons+1);
        menuButtons.push_back(new textButton(0, std::string("Start New Game"), mainMenuLayerId::hosting, mainMenuLayerId::game, position, *font, textbuttonTypes::newSafe));
    }    
    for (unsigned short i = 0; i < availableSaveFiles.size(); i++){
        short id;
        if (availableSaveFiles.size() < MAX_SAVES){
            id = i+1;
            maxButtons = availableSaveFiles.size() + 1;
        }
        else {
            maxButtons = MAX_SAVES;
            id = i;
        }
        position[1] = resolution.y / float(maxButtons+1) * (id+1);
        menuButtons.push_back(new ClickButton(id, availableSaveFiles[i].name, mainMenuLayerId::hosting, mainMenuLayerId::game, position, *font, clickbuttonTypes::saveFile));
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
    warningMessage.setPosition((resolution.x/2) - (warningMessage.getGlobalBounds().getSize().x / 2), (resolution.y / 2) - (warningMessage.getGlobalBounds().getSize().y / 2));

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


GAME_STATE MainMenu::runMenu(void){
    bg_music->play();

    //  MAIN MENU LOOP
    //      1. DRAW
    //      2. Register Key/Mouse Input
    //      3. Update buttons/application features 
    while (currentLayer != mainMenuLayerId::leave && currentLayer != mainMenuLayerId::game){
    
        //  1. DRAW
        window->clear(sf::Color::Transparent);
        window->draw(backgroundSprite);
        if (menuWarning == menuPopUps::noPopUp){
            drawMenuButtons();
            if (!writing){//No text input?->Draw cursor
                cursor->draw(*window);
            }
        }
        else {
            drawMenuPopUp();
        }  
        window->display();


        //  2. Handle Events
        sf::Event event;
        while (window->pollEvent(event)){
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
                        
        //  3. UPDATE BUTTONS
        if (currentLayer != mainMenuLayerId::game && currentLayer != mainMenuLayerId::leave && menuWarning == menuPopUps::noPopUp){
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

void MainMenu::drawMenuPopUp(void){
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
    while (fileExists(newpathName+std::to_string(i)+".SAV") && i < 10000){
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
                    buttonPointer->delLastInput(resolution);
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
                textPointer->addInput(ev.text.unicode, resolution);
                return;
            }
        }
    }
}



void MainMenu::mouseButtonPressed(void){
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
                    // Click buttons always change the layer on click
                    if (currentLayer != mainMenuLayerId::joining){
                        currentLayer = buttTmp->layer_next;
                    }
                    //Except for the "Connect" button in the "Joining" layer, which requires a valid IP and Port to change to "Game" layer
                    else {
                        if (currentLayer == mainMenuLayerId::joining && clickPointer->idx == 2 && hostIp != sf::IpAddress::None && hostPort > 2024 && hostPort < 65534){
                            currentLayer = mainMenuLayerId::game;
                        }
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
