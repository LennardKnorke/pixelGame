#include "application.hpp"
#include "menu.hpp"
#include "gameSaveSummary.hpp"
GAME_STATE Application::menuLoop(void){

    readAllSaveSummaries(&availableSaveFiles);
    
    //Set up menu layers
    std::vector<button*> MenuButtons;
    setUpMenuButtons(&MenuButtons, this);

    //SetUpBackgroundSprite (set as class variable?)
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(textures.menu[menuTextureIdxS::background]);
    backgroundSprite.setPosition(0, 0);
    backgroundSprite.setScale(resolution.x/backgroundSprite.getGlobalBounds().getSize().x, resolution.y/backgroundSprite.getGlobalBounds().getSize().y);

    //actual menu loop
    layersId currentLayer = layersId::Base;
    menuPopUps currentMenuPopUp = menuPopUps::NoPopUp;
    bool allowTextInput = false;

    //prepare a warning message for errors
    sf::Text warningMessage;
    warningMessage.setString("");
    warningMessage.setFillColor(sf::Color::White);
    warningMessage.setFont(gameFont);
    warningMessage.setStyle(sf::Text::Style::Regular);
    warningMessage.setCharacterSize(30 * ratioScaling);

    while (currentLayer != layersId::final && currentLayer != layersId::leave){

        ////BEGIN DRAWING
        window.clear(sf::Color::Transparent);
        window.draw(backgroundSprite);

        if (currentMenuPopUp == menuPopUps::NoPopUp){
            drawMenuButtons(&MenuButtons, currentLayer, &window);
            if (!allowTextInput){
                cursor.draw(&window);
            }
        }
        else {
            drawMenuPopUp(currentMenuPopUp, &window, &warningMessage);
        }
        
        window.display();
        ////END DRAWING




        ////BEING EVENTS
        sf::Event event;
        while (window.pollEvent(event)){
            if (currentMenuPopUp == menuPopUps::NoPopUp){
                //KEYS PRESSED
                if (event.type == sf::Event::KeyPressed){
                ////INPUT EVENTS
                    //escape button pressed
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                        if (allowTextInput){
                            allowTextInput = false;
                            for (button *buttTmp : MenuButtons){
                                WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                                if (childPtr1 && childPtr1->activeInput){
                                    childPtr1->resetInput();
                                }
                            }
                        }
                        else {
                            for (button *buttTmp : MenuButtons){
                                WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                                if (childPtr1){
                                    childPtr1->resetInput();
                                }
                            }
                            if (currentLayer == layersId::Hosting && mode == gameMode::Single){
                                currentLayer = layersId::GameMode;
                            }
                            else {
                                currentLayer = getPreviousLayer(currentLayer);
                            } 
                        }
                    }


////INPUT EVENTS
                    //enter button pressed
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && allowTextInput){
                        for (button *buttTmp : MenuButtons){
                            WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                            if (childPtr1 && childPtr1->focus && childPtr1->activeInput){
                                allowTextInput = false;
                                childPtr1->activeInput = false;
                                if (childPtr1->userText == ""){
                                    childPtr1->resetInput();
                                    if (currentLayer == layersId::Joining && cursor.sprite[cursor.activeSprite].getPosition().y < resolution.y/2){
                                            //Entered HOST IP ADRESS
                                            hostAdress.ip = sf::IpAddress::None;
                                    }
                                    else if (currentLayer == layersId::Joining) {
                                            //ENTERED HOST PORT
                                            hostAdress.port = 0;
                                    }
                                    else if (currentLayer == layersId::Hosting){
                                        hostAdress.pathSave.clear();
                                        currentMenuPopUp = menuPopUps::InvalidName;
                                        setMenuPopUpMessage(currentMenuPopUp, &warningMessage, resolution);
                                    }
                                }
                                else {
                                    if (currentLayer == layersId::Hosting){
                                        //Create a new Play save
                                        std::string newSaveName = childPtr1->userText;
                                        if (createSave(newSaveName, &currentMenuPopUp)){
                                            currentLayer = childPtr1->nextLayer;
                        
                                        }
                                        else {
                                            setMenuPopUpMessage(currentMenuPopUp, &warningMessage, resolution);
                                            childPtr1->resetInput();
                                        }

                                    }
                                    else if (currentLayer == layersId::Joining){
                                        if (cursor.sprite[cursor.activeSprite].getPosition().y < resolution.y/2){
                                            //Entered HOST IP ADRESS
                                            hostAdress.ip = sf::IpAddress(childPtr1->userText);
                                        }
                                        else {
                                            //ENTERED HOST PORT
                                            hostAdress.port = static_cast<unsigned short>(std::strtoul(childPtr1->userText.c_str(), NULL, 0));
                                        }
                                        childPtr1->text.setFillColor(sf::Color::Red);
                                        if (hostAdress.ip != sf::IpAddress::None && hostAdress.port != 0){
                                            currentLayer = layersId::final;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }


////INPUT EVENTS
                //Mouse is pressed
                else if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !allowTextInput){
                    for (button *buttTmp : MenuButtons){
                        if (buttTmp->layer == currentLayer){
                            ClickButton *childPtr1 = dynamic_cast<ClickButton*>(buttTmp);
                            WriteButton *childPtr2 = dynamic_cast<WriteButton*>(buttTmp);
                            GraphicButton *childPtr3 = dynamic_cast<GraphicButton*>(buttTmp);
                            //Either just change the layer
                            if (childPtr1 && buttTmp->focus){   
                                if (currentLayer == layersId::Hosting){
                                    setActiveSafe(childPtr1->stringText);
                                }                         
                                currentLayer = childPtr1->nextLayer;
                                break;
                            }
                            //or activate text input
                            else if (childPtr2 && buttTmp->focus){
                                allowTextInput = true;
                                childPtr2->activeInput = true;
                                childPtr2->text.setString(childPtr2->userText);
                                break;
                            }
                            //update resolution settings and menu ui elements
                            else if (childPtr3 && buttTmp->focus && resolution != childPtr3->newResolution){
                                /*
                                resolution = childPtr3->newResolution;
                                ratioScaling = 1920.0/ resolution.x;
                                std::cout << resolution.x<< "\t";
                                std::cout << resolution.y<< "\t";
                                std::cout << ratioScaling<< std::endl;
                                window.setSize(resolution);

                                backgroundSprite.setScale(resolution.x/backgroundSprite.getGlobalBounds().getSize().x, resolution.y / backgroundSprite.getGlobalBounds().getSize().y);
                                saveSettings("settings.bin");

                                for (menuLayer *tmp : Layers){
                                    tmp->changeRes(this);
                                }
                                */
                            }
                        }

                    }
                }


////INPUT EVENTS
                //Text is entered
                else if (event.type == sf::Event::TextEntered && allowTextInput){
                    for (button *buttTmp : MenuButtons){
                        if (buttTmp->layer == currentLayer){
                            WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                            if (childPtr1 && childPtr1->activeInput && childPtr1->focus){
                                if (event.text.unicode == 8){
                                    childPtr1->delLastInput(resolution);
                                }
                                else if (checkCharacterInput(currentLayer, event.text.unicode, childPtr1->userText.size())){
                                    childPtr1->addInput(event.text.unicode, resolution);
                                }
                            }
                        }

                    }
                }
            }
            else if (currentMenuPopUp == menuPopUps::InvalidName){
                if (event.type == sf::Event::KeyPressed && (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))){
                    currentMenuPopUp = menuPopUps::NoPopUp;
                }
            }
        }
        ////END INPUT EVENTS


        ////BEGIN UPDATES (to avoid bugs and crashes.. only update valid layers)
        if (currentLayer != layersId::final && currentLayer != layersId::leave && currentMenuPopUp == menuPopUps::NoPopUp){
            for (button* butt : MenuButtons){
                if (butt->layer == currentLayer){
                    butt->update(cursor.returnPosition());
                }
            }
            if (!allowTextInput){
                cursor.update();
            }
            getMenuPicks(cursor.returnPosition(), currentLayer, &MenuButtons);
        }
        ////END UPDATES
    }////END OF MENULOPP!

    for (button* objc : MenuButtons){
        delete objc;
    }
    availableSaveFiles.clear();
    ////Return new gameState
    if (currentLayer == layersId::final){
        std::cout<< "Playing: ";
        if (mode == gameMode::Online){
            std::cout << "Online\n";
        }
        else if (mode == gameMode::Single){
            std::cout << "Alone\n";
        }
        else {
            std::cout << "Locally\n";
        }
        if (wantsHost || mode == gameMode::Single){
            //search for a free port
            hostAdress.port = 1024;
            sf::TcpListener tmpListener;
            while (hostAdress.port < USHRT_MAX && tmpListener.listen(hostAdress.port) != sf::Socket::Done){
                hostAdress.port++;
            }
            tmpListener.close();
            //use machine adress as host adress
            if (mode == gameMode::Online){
                hostAdress.ip = machinePublicAdress;
            }
            else {
                hostAdress.ip = machineLocalAdress;
            }
            std::cout << "Hosting Game: " << hostAdress.pathSave <<std::endl;
            std::cout << "Using IP: " << hostAdress.ip.toString() << "::" << hostAdress.port <<std::endl;

        }
        else {
            std::cout << "Joining to: ";
            std::cout << hostAdress.ip.toString() << "::" << hostAdress.port <<std::endl;

        }
        
        return GAME_STATE::GAME;
    }

    std::cout << "Leaving Application!\n";
    return GAME_STATE::QUIT;
}



void Application::getMenuPicks(sf::Vector2f cursorPosition, layersId currentLayer, std::vector<button*> *buttonList){
    if (currentLayer == layersId::GameMode){
        int i = 0;
        for (button *iterator : *buttonList){
            if (iterator->focus){
                if (i == 0){
                    mode = gameMode::Single;
                }
                else if (i == 1){
                    mode = gameMode::Local;
                }
                else {
                    mode = gameMode::Online;
                }
            }
            i++;
        }
    }
    else if (currentLayer == layersId::HostVsClient){
        if (cursorPosition.y < resolution.y/2){
            wantsHost = true;
        }
        else {
            wantsHost = false;
        }
    }
}



bool Application::checkCharacterInput(layersId activeLayer, sf::Uint32 c, int activeLength){
    if (activeLayer == layersId::Hosting){//Entering a new savename only allow "a-z", "A-Z", "0-9".
        if (activeLength >= MAX_LENGTH_SAVENAME){
            return false;
        }
        if ((c < 48 || c > 57) && (c < 65 || c > 90) && (c < 97 || c > 122) && c != 32){
            return false;
        }
    }
    if (activeLayer == layersId::Joining) {//Entering "0-9", "." , ":" for the ip
        if (activeLength >=MAX_LENGTH_IP_PUBLIC){
            return false;
        }
    }
    if (activeLayer == layersId::Settings){

    }
    
    return true;
}



void Application::setActiveSafe(std::string saveName){
    for (gameSaveSummary sum : availableSaveFiles){
        if (sum.saveName == saveName){
            hostAdress.pathSave = sum.pathName;
        }
    }
}



void Application::setMenuPopUpMessage(menuPopUps PopUp, sf::Text *warningMessage, sf::Vector2u res){
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
}


//"FREE FUNCTIONS"
void drawMenuPopUp(menuPopUps PopUp, sf::RenderWindow *window, sf::Text *warningMessage){
    window->draw(*warningMessage);
}


void drawMenuButtons(std::vector<button*> *MenuButtons, layersId currentLayer, sf::RenderWindow *window){
    for (button* buttTmp : *MenuButtons){
        if (buttTmp->layer == currentLayer){
            buttTmp->draw(window);
        }
    }
}

void setUpMenuButtons(std::vector<button*> *buttonList, Application *applicationPointer){
    //layersId::Base
    layersId currentLayer = layersId::Base;
    int maxButtons = 3;
    std::vector<std::string> text = {"Play", "Settings", "Exit"};
    std::vector<layersId> followUpsLayers = {layersId::GameMode, layersId::Settings, layersId::leave};
    for (int i = 0; i < maxButtons; i++){
        buttonList->push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Settings
    currentLayer = layersId::Settings;
    maxButtons = 2;
    text = {"Graphics", "Controls"};
    followUpsLayers = {layersId::Graphic, layersId::Controls};
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
    currentLayer = layersId::GameMode;
    maxButtons = 3;
    text = {"Singleplayer", "Locally", "Online"};
    followUpsLayers = {layersId::Hosting, layersId::HostVsClient, layersId::HostVsClient};
    for (int i = 0; i < maxButtons; i++){
        buttonList->push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::HostVsClient
    currentLayer = layersId::HostVsClient;
    maxButtons = 2;
    text = {"Host", "Join"};
    followUpsLayers = {layersId::Hosting, layersId::Joining};
    for (int i = 0; i < maxButtons; i++){
        buttonList->push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Joining
    currentLayer = layersId::Joining;
    maxButtons = 2;
    text = {"Enter Host Ip", "Enter Host Port"};
    followUpsLayers = {layersId::final, layersId::final};
    for (int i = 0; i < maxButtons; i++){
        buttonList->push_back(new WriteButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Hosting
    currentLayer = layersId::Hosting;
    maxButtons = 1;
    text = {"Create new world"};
    followUpsLayers = {layersId::final};
    for (int i = 0; i < maxButtons; i++){
    }
    for (int i = 0; i < applicationPointer->availableSaveFiles.size(); i++){
        buttonList->push_back(new ClickButton(applicationPointer->availableSaveFiles[i].saveName, layersId::final, applicationPointer, applicationPointer->availableSaveFiles.size()+maxButtons, i+1, layersId::Hosting));
    }
    buttonList->push_back(new WriteButton(text[0], followUpsLayers[0], applicationPointer, maxButtons + applicationPointer->availableSaveFiles.size(), 0, currentLayer));

    text.clear();
    followUpsLayers.clear();
}


layersId getPreviousLayer(layersId currentLayer){
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


///////////////////////
//BUTTON CLASS(es)
///////////////////////
//pretending we define the parent class functions
void button::draw(sf::RenderWindow *window){std::cout << stringText << " parent class" <<std::endl;};
void button::update(sf::Vector2f mousePos){}
void button::changeRes(Application *applicationPointer, int maxButt, int currButt){}

//Pretending over

ClickButton::ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt, layersId currentLayer){
    layer = currentLayer;
    stringText = t;
    nextLayer = followLayer;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(30 / applicationPointer->ratioScaling);
    text.setStyle(sf::Text::Style::Regular);

    imageSprite.setTexture(applicationPointer->textures.menu[menuTextureIdxS::button_red]);
    imageSprite.setScale((300 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().x, (100 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().y);
    imageSprite.setPosition(((*applicationPointer).resolution.x/2.0) - (imageSprite.getGlobalBounds().getSize().x/2), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (imageSprite.getGlobalBounds().getSize().y / 2));

    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));

};ClickButton::~ClickButton(void){};



void ClickButton::changeRes(Application *applicationPointer, int maxButt, int currButt){
    text.setCharacterSize(30 / applicationPointer->ratioScaling);
    imageSprite.setScale((300 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().x, (100 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().y);
    imageSprite.setPosition(((*applicationPointer).resolution.x/2.0) - (imageSprite.getGlobalBounds().getSize().x/2), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (imageSprite.getGlobalBounds().getSize().y / 2));
    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));

}


void ClickButton::draw(sf::RenderWindow *window){
    window->draw(imageSprite);
    window->draw(text);
}



void ClickButton::update(sf::Vector2f mousePos){
    if (mousePos.x > imageSprite.getPosition().x && mousePos.x < imageSprite.getPosition().x + imageSprite.getGlobalBounds().getSize().x
    && mousePos.y > imageSprite.getPosition().y && mousePos.y < imageSprite.getPosition().y + imageSprite.getGlobalBounds().getSize().y){
        if (!focus){
            focus = true;
            text.setStyle(sf::Text::Style::Underlined);
            text.setFillColor(sf::Color::Red);
        }
    }    
    else {
        if (focus){
            focus = false;
            text.setStyle(sf::Text::Style::Regular);
            text.setFillColor(sf::Color::White);
        }
    }
}



WriteButton::WriteButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt, layersId given_layer){
    stringText = t;
    nextLayer = followLayer;
    activeInput = false;
    layer = given_layer;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(30 / applicationPointer->ratioScaling);
    

    imageSprite.setTexture(applicationPointer->textures.menu[menuTextureIdxS::button_red]);
    imageSprite.setScale((300 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().x, (100 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().y);
    imageSprite.setPosition(((*applicationPointer).resolution.x/2.0) - (imageSprite.getGlobalBounds().getSize().x/2), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (imageSprite.getGlobalBounds().getSize().y / 2));

    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));
    ogPosition[0] = text.getPosition().x;
    ogPosition[1] = text.getPosition().y;
    text.setStyle(sf::Text::Style::Regular);
}



WriteButton::~WriteButton(void){};



void WriteButton::draw(sf::RenderWindow *window){
    window->draw(imageSprite);
    window->draw(text);
}



void WriteButton::update(sf::Vector2f mousePos){
    if (!activeInput){
        sf::Vector2f rec = text.getPosition();
        if (mousePos.x > rec.x && mousePos.x < rec.x + text.getLocalBounds().width
        && mousePos.y > rec.y && mousePos.y < rec.y + text.getLocalBounds().height){
            if (!focus){
                focus = true;
                text.setStyle(sf::Text::Style::Underlined);
                text.setFillColor(sf::Color::Red);
            }
        }    
        else {
            if (focus){
                focus = false;
                text.setStyle(sf::Text::Style::Regular);
                text.setFillColor(sf::Color::White);
            }
        }
    }
    else {
        if (text.getFillColor() != sf::Color::Green){
            text.setFillColor(sf::Color::Green);
        }
    }
}



void WriteButton::resetInput(void){
    text.setString(stringText);
    text.setPosition(ogPosition[0], ogPosition[1]);
    text.setFillColor(sf::Color::Red);
    userText = "";
    activeInput = false;
}



void WriteButton::addInput(sf::Uint32 input, sf::Vector2u res){
    userText += input;
    text.setString(userText);
    text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);    
}



void WriteButton::delLastInput(sf::Vector2u res){
    if (userText.size() != 0){
        userText.erase(userText.size() - 1, 1);
        text.setString(userText);
        text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);
    }   
}



void WriteButton::changeRes(Application *applicationPointer, int maxButt, int currButt){
    text.setCharacterSize(30 / applicationPointer->ratioScaling);
    
    imageSprite.setScale((300 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().x, (100 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().y);
    imageSprite.setPosition(((*applicationPointer).resolution.x/2.0) - (imageSprite.getGlobalBounds().getSize().x/2), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (imageSprite.getGlobalBounds().getSize().y / 2));

    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));
    ogPosition[0] = text.getPosition().x;
    ogPosition[1] = text.getPosition().y;
}
//
GraphicButton::GraphicButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt, sf::Vector2u newRes, layersId currentLayer){
    newResolution = newRes;
    layer = currentLayer;
    stringText = t;
    nextLayer = followLayer;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(30 / applicationPointer->ratioScaling);
    text.setStyle(sf::Text::Style::Regular);

    imageSprite.setTexture(applicationPointer->textures.menu[menuTextureIdxS::button_red]);
    imageSprite.setScale((300 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().x, (100 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().y);
    imageSprite.setPosition(((*applicationPointer).resolution.x/2.0) - (imageSprite.getGlobalBounds().getSize().x/2), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (imageSprite.getGlobalBounds().getSize().y / 2));

    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));

};



GraphicButton::~GraphicButton(void){};



void GraphicButton::update(sf::Vector2f mousePos){
    if (mousePos.x > imageSprite.getPosition().x && mousePos.x < imageSprite.getPosition().x + imageSprite.getGlobalBounds().getSize().x
    && mousePos.y > imageSprite.getPosition().y && mousePos.y < imageSprite.getPosition().y + imageSprite.getGlobalBounds().getSize().y){
        if (!focus){
            focus = true;
            text.setStyle(sf::Text::Style::Underlined);
            text.setFillColor(sf::Color::Red);
        }
    }    
    else {
        if (focus){
            focus = false;
            text.setStyle(sf::Text::Style::Regular);
            text.setFillColor(sf::Color::White);
        }
    }
}



void GraphicButton::draw(sf::RenderWindow *window){
    window->draw(imageSprite);
    window->draw(text);
}



void GraphicButton::changeRes(Application *applicationPointer, int maxButt, int currButt){
    text.setCharacterSize(30 / applicationPointer->ratioScaling);

    imageSprite.setScale((300 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().x, (100 / applicationPointer->ratioScaling)/imageSprite.getGlobalBounds().getSize().y);
    imageSprite.setPosition(((*applicationPointer).resolution.x/2.0) - (imageSprite.getGlobalBounds().getSize().x/2), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (imageSprite.getGlobalBounds().getSize().y / 2));

    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));

}