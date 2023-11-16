#include "menu.hpp"
#include "application.hpp"
#include "gameSaveSummary.hpp"
#include "menuButtons.hpp"
/////////////////////////////////////////////////////////////////////////////////
//APPLICATION HPP DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////
GAME_STATE Application::menuLoop(void){

    //Variables to control what to display in menu
    mainMenuLayerId currentLayer = mainMenuLayerId::Base;
    menuPopUps currentMenuPopUp = menuPopUps::NoPopUp;
    bool allowTextInput = false;

    //Read every save name/path for ingame buttons
    readAllSaveSummaries(availableSaveFiles);

    //Set up menu buttons
    std::vector<button*> MenuButtons;
    setUpMenuButtons(MenuButtons, this);

    //SetUpBackgroundSprite
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(textures.menu[menuTextureIdxS::background]);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(resolution.x/backgroundSprite.getGlobalBounds().getSize().x, resolution.y/backgroundSprite.getGlobalBounds().getSize().y);

    //prepare a warning message for errors
    sf::Text warningMessage = initErrorMessage(&gameFont, ratioScaling);
    
    //  MAIN MENU LOOP
    //      1. DRAW
    //      2. Register Key/Mouse Input
    //      3. Update buttons/application features 
    while (currentLayer != mainMenuLayerId::final && currentLayer != mainMenuLayerId::leave){
    
        //  1. DRAW
        window.clear(sf::Color::Transparent);
        window.draw(backgroundSprite);
        if (currentMenuPopUp == menuPopUps::NoPopUp){
            drawMenuButtons(MenuButtons, currentLayer, window);
            if (!allowTextInput){//No text input?->Draw cursor
                cursor.draw(window);
            }
        }
        else {
            drawMenuPopUp(currentMenuPopUp, &window, &warningMessage);
        }
        
        window.display();


        //  2. Handle Events
        sf::Event event;
        while (window.pollEvent(event)){
            if (currentMenuPopUp != menuPopUps::NoPopUp){
                inputErrorPopUp(currentMenuPopUp, &event);
            }
            else {
                if (event.type == sf::Event::KeyPressed){
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                        escapeKeyPressed(&allowTextInput, &MenuButtons, &currentLayer, mode);
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
                        enterKeyPressed(&allowTextInput, &MenuButtons, &currentLayer, &currentMenuPopUp, this);
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) || sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
                        deleteKeyPressed(&allowTextInput,&MenuButtons, currentLayer, this);
                    }
                }
                else if (event.type == sf::Event::TextEntered){
                    textEntered(&allowTextInput,&MenuButtons, currentLayer, this, event, mode);
                }
                else if (event.type == sf::Event::MouseButtonPressed ){
                    mouseButtonPressed(&allowTextInput, &MenuButtons, &currentLayer, this, mode);
                }
            }
        }
                        
        //  3. UPDATE BUTTONS
        if (currentLayer != mainMenuLayerId::final && currentLayer != mainMenuLayerId::leave && currentMenuPopUp == menuPopUps::NoPopUp){
            for (button* butt : MenuButtons){
                if (butt->layer == currentLayer){
                    butt->update(cursor.returnPosition());
                }
            }
            if (!allowTextInput){
                cursor.update();
            }
            getMenuPicks(&MenuButtons, currentLayer, &mode);
        }
    }
    //End of visual menu loop. user chose either to exit or play

    //Dereference and clear everything
    for (button* objc : MenuButtons){
        delete objc;
    }
    availableSaveFiles.clear();
    if (currentLayer == mainMenuLayerId::leave){
        std::cout << "Leaving Application!\n";
        return GAME_STATE::QUIT;
    }
    return GAME_STATE::GAME;
}



/////////////////////////////////////////////////////////////////////////////////
//MENU HPP DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////
sf::Text initErrorMessage(sf::Font *font, float scaling){
    sf::Text newText;
    newText.setString("");
    newText.setFillColor(sf::Color::White);
    newText.setFont(*font);
    newText.setStyle(sf::Text::Style::Regular);
    newText.setCharacterSize(30 * scaling);
    return newText;
}



void drawMenuPopUp(menuPopUps PopUp, sf::RenderWindow *window, sf::Text *warningMessage){
    window->draw(*warningMessage);
}



void inputErrorPopUp(menuPopUps &menuPopUp, sf::Event *ev){
    if (menuPopUp == menuPopUps::InvalidName){
        if(ev->type == sf::Event::KeyPressed && (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))){
            menuPopUp = menuPopUps::NoPopUp;
        }
    }
    return;
}



void escapeKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, gameMode mode){
    //1. cancel input given
    if (*activeText){
        for (button *buttTmp : *menuButtons){
            if (buttTmp->layer == *currentLayer && buttTmp->focus){
                newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
                if (newSavePointer && newSavePointer->activeInput){
                    newSavePointer->resetInput();
                    *activeText = false;
                    return;
                }
                adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
                if (adressPointer && adressPointer->activeInput){
                    adressPointer->resetInput();
                    *activeText = false;
                    return;
                }
            }
        }
        return;
    }

    //2. Otherwise reset all input and go back a layer
    if (*currentLayer == mainMenuLayerId::Hosting || *currentLayer == mainMenuLayerId::Joining){
        for (button *buttTmp : *menuButtons){
            if (buttTmp->layer == *currentLayer){
                newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
                adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
                if (newSavePointer && *currentLayer == mainMenuLayerId::Hosting){
                    newSavePointer->resetInput();
                }
                else if (adressPointer && *currentLayer == mainMenuLayerId::Joining){
                    adressPointer->resetInput();
                }
            }
        }
    }
    *currentLayer = getPreviousLayer(*currentLayer, mode);
}



mainMenuLayerId getPreviousLayer(mainMenuLayerId currentLayer, gameMode mode){
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



void enterKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, menuPopUps *menuWarning, Application *appPointer){
    //1. Do nothing
    if (*activeText == false){
        return;
    }
    
    for (button *buttTmp : *menuButtons){
        if (buttTmp->layer == *currentLayer && buttTmp->focus){

            //2.EITHER INITIATE NEW SAFE
            newSafeButton *safePointer = dynamic_cast<newSafeButton*>(buttTmp);
            if (safePointer && safePointer->activeInput && *currentLayer == mainMenuLayerId::Hosting){
                std::string newSaveName = safePointer->userText;
                if (createSave(newSaveName, menuWarning, appPointer)){
                    *currentLayer = mainMenuLayerId::final;
                    safePointer->activeInput = false;
                    *activeText = false;
                    return;
                }
                safePointer->resetInput();
                *activeText = false;
                return;
            }

            //3. OR FILL IN PORTS/ADRESS
            adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
            if (adressPointer && adressPointer->activeInput && *currentLayer == mainMenuLayerId::Joining){
                if (adressPointer->n_button == 0){
                    appPointer->hostAdress.ip = sf::IpAddress(adressPointer->userText);
                }
                else {
                    appPointer->hostAdress.port = static_cast<unsigned short>(std::strtoul(adressPointer->userText.c_str(), NULL, 0));
                }
                adressPointer->activeInput = false;
                *activeText = false;
                if (appPointer->hostAdress.ip != sf::IpAddress::None && appPointer->hostAdress.port != 0){
                    *currentLayer = mainMenuLayerId::final;
                }
                return;
            }
        }
    }
}
 


bool createSave(std::string newSaveName, menuPopUps *menuWarning, Application *appPointer){
    if (appPointer->availableSaveFiles.size() >= MAX_N_SAVES){
        *menuWarning = menuPopUps::TooManySaves;
        return false;
    }
    for (gameSaveSummary tmp : appPointer->availableSaveFiles){
        if (tmp.game == newSaveName){
            *menuWarning = menuPopUps::InvalidName;
            return false;
        }
    }
    return createNewSafeFile(newSaveName, (appPointer->hostAdress.pathSave));
}



void deleteKeyPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, Application *appPointer){
    //1. Remove Last input
    if (*activeText){
        for (button *buttTmp : *menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
                if (newSavePointer){
                    newSavePointer->delLastInput(appPointer->resolution);
                    return;
                }

                adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
                if (adressPointer){
                    adressPointer->delLastInput(appPointer->resolution);
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
    for (button *buttTmp : *menuButtons){
        if (buttTmp->layer == currentLayer && buttTmp->focus){
            newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
            if (newSavePointer){
                //DELETE SAVE! PENDING
                return;
            }
        }
    }
}



void textEntered(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, Application *appPointer, sf::Event ev, gameMode mode){
    //1. No text input allowed
    if (*activeText == false){
        return;
    }
    //2. add valid characters to display text
    for (button *buttTmp : *menuButtons){
        if (buttTmp->layer == currentLayer && buttTmp->focus){
            newSafeButton *newSavePointer = dynamic_cast<newSafeButton*>(buttTmp);
            adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
            if (newSavePointer && validCharacterInput(currentLayer, ev.text.unicode, newSavePointer->userText.size(), mode, buttTmp->n_button)){
                newSavePointer->addInput(ev.text.unicode, appPointer->resolution);
                return;
            }
            if (adressPointer && validCharacterInput(currentLayer, ev.text.unicode, adressPointer->userText.size(), mode, buttTmp->n_button)){
                adressPointer->addInput(ev.text.unicode, appPointer->resolution);
                return;
            }
            return;
        }
    }
}



bool validCharacterInput(mainMenuLayerId activeLayer, sf::Uint32 c, int activeLength, gameMode mode, int n){
    //Allow only characters for the savename
    if (activeLayer == mainMenuLayerId::Hosting){//Entering a new savename only allow "a-z", "A-Z", "0-9".
        if (activeLength >= maxInputLengths::saveName){
            return false;
        }
        if ((c < 48 || c > 57) && (c < 65 || c > 90) && (c < 97 || c > 122) && c != 32){
            return false;
        }
    }
    //Allow only valid Ip's and Ports
    if (activeLayer == mainMenuLayerId::Joining) {//Entering "0-9", "." , ":" for the ip
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



void mouseButtonPressed(bool *activeText, std::vector<button *> *menuButtons, mainMenuLayerId *currentLayer, Application *appPointer, gameMode mode){
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && *activeText == false){
        for (button *buttTmp : *menuButtons){
            if (buttTmp->layer == *currentLayer && buttTmp->focus){
                ClickButton *clickPointer = dynamic_cast<ClickButton*>(buttTmp);
                if (clickPointer){
                    *currentLayer = buttTmp->nextLayer;
                    return;
                }

                adressButton *adressPointer = dynamic_cast<adressButton*>(buttTmp);
                if (adressPointer){
                    adressPointer->activeInput = true;
                    *activeText = true;
                    return;
                }

                newSafeButton *safePointer = dynamic_cast<newSafeButton*>(buttTmp);
                if (safePointer){
                    safePointer->activeInput = true;
                    *activeText = true;
                    return;
                }
                
                ProfileButton *profPointer = dynamic_cast<ProfileButton*>(buttTmp);
                if (profPointer){
                    setActiveSafe(profPointer->stringText, &(appPointer->hostAdress.pathSave), appPointer->availableSaveFiles);
                    *currentLayer = buttTmp->nextLayer;
                    return;
                }
                return;
            }
        }
    }
}



void setActiveSafe(std::string saveName, std::string *pathToBeLoaded, std::vector<gameSaveSummary> availableSaveFiles){
    for (gameSaveSummary sum : availableSaveFiles){
        if (sum.game == saveName){
            pathToBeLoaded->clear();
            *pathToBeLoaded = sum.path;
        }
    }
}



void getMenuPicks(std::vector<button *> *menuButtons, mainMenuLayerId currentLayer, gameMode *mode){
    if (*mode != gameMode::undefined && (currentLayer == mainMenuLayerId::Base || currentLayer == mainMenuLayerId::Settings || currentLayer == mainMenuLayerId::Controls ||currentLayer == mainMenuLayerId::Graphic)){
        *mode = gameMode::undefined;
        return;
    }

    if (currentLayer == mainMenuLayerId::GameMode){
        for (button *buttTmp : *menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                if (buttTmp->n_button == 0){
                    *mode = gameMode::Single;
                }
                else if (buttTmp->n_button == 1){
                    *mode = gameMode::local;
                }
                else if (buttTmp->n_button == 2){
                    *mode = gameMode::online;
                }
                return;
            }
        }
        *mode = gameMode::undefined;
        return;
    }

    if (currentLayer == mainMenuLayerId::HostVsClient){
        for (button *buttTmp : *menuButtons){
            if (buttTmp->layer == currentLayer && buttTmp->focus){
                //Choosing to be host
                if (buttTmp->n_button == 0){
                    if (*mode == gameMode::local|| *mode == gameMode::Local_client){
                        *mode = gameMode::Local_host;
                    }
                    else if(*mode == gameMode::online|| *mode == gameMode::Online_client){
                        *mode = gameMode::Online_host;
                    }
                    return;
                }
                //Choosing to be client
                else if (buttTmp->n_button == 1){
                    if (*mode == gameMode::local || *mode == gameMode::Local_host){
                        *mode = gameMode::Local_client;
                    }
                    else if(*mode == gameMode::online|| *mode == gameMode::Online_host){
                        *mode = gameMode::Online_client;
                    }
                    return;
                }
            }
        }
    }

}


void setMenuPopUpMessage(void){
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











