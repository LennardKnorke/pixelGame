#include "application.hpp"
#include "menu.hpp"

GAME_STATE Application::menuLoop(void){

    readAllSaveSummaries();
    
    //Set up menu layers
    std::vector<menuLayer*> Layers;
    layersId visibLayers[MaxVisibleLayers] = {Base, Settings, Type, Multiplayer, Join, Host, Graphic, Controls};//
    for (int i =0; i < MaxVisibleLayers; i++){
        Layers.push_back(new menuLayer(visibLayers[i], this));
    }

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
            for (button *obj : Layers[currentLayer]->LayerButtons){
                obj->draw(&window);
            }
            if (currentLayer == Host){
                //Draw instructions. Click to select, del for removing
            }
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
                        for (button *buttTmp : Layers[currentLayer]->LayerButtons){
                            WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                            if (childPtr1 && childPtr1->activeInput){
                                childPtr1->resetInput();
                            }
                        }
                    }
                    else {
                        for (button *buttTmp : Layers[currentLayer]->LayerButtons){
                            WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                            if (childPtr1){
                                childPtr1->resetInput();
                            }
                        }
                        if (currentLayer == layersId::Host && mode == gameMode::Single){
                            currentLayer = layersId::Type;
                        }
                        else {
                            currentLayer = Layers[currentLayer]->previousLayer;
                        } 
                    }
                }


////INPUT EVENTS
                //enter button pressed
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && allowTextInput){
                    for (button *buttTmp : Layers[currentLayer]->LayerButtons){
                        WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                        if (childPtr1 && childPtr1->focus && childPtr1->activeInput){
                            allowTextInput = false;
                            childPtr1->activeInput = false;
                            if (childPtr1->userText == ""){
                                childPtr1->resetInput();
                                if (currentLayer == layersId::Join && cursor.sprite[cursor.activeSprite].getPosition().y < resolution.y/2){
                                        //Entered HOST IP ADRESS
                                        hostIp = "";
                                }
                                else if (currentLayer == layersId::Join) {
                                        //ENTERED HOST PORT
                                        hostPort = "";
                                }
                                else if (currentLayer == layersId::Host){
                                    currentMenuPopUp = menuPopUps::InvalidName;
                                    setMenuPopUpMessage(currentMenuPopUp, &warningMessage, resolution);
                                }
                            }
                            else {
                                if (currentLayer == layersId::Host){
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
                                else if (currentLayer == layersId::Join){
                                    if (cursor.sprite[cursor.activeSprite].getPosition().y < resolution.y/2){
                                        //Entered HOST IP ADRESS
                                        hostIp = childPtr1->userText;
                                    }
                                    else {
                                        //ENTERED HOST PORT
                                        hostPort = childPtr1->userText;
                                    }
                                    childPtr1->text.setFillColor(sf::Color::Red);
                                    if (hostIp != "" && hostPort != ""){
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
                for (button *buttTmp : Layers[currentLayer]->LayerButtons){
                    ClickButton *childPtr1 = dynamic_cast<ClickButton*>(buttTmp);
                    WriteButton *childPtr2 = dynamic_cast<WriteButton*>(buttTmp);
                    GraphicButton *childPtr3 = dynamic_cast<GraphicButton*>(buttTmp);
                    //Either just change the layer
                    if (childPtr1 && buttTmp->focus){   
                        if (currentLayer == layersId::Host){
                            setActiveSafe(childPtr1->stringText);
                        }                         
                        currentLayer = childPtr1->nextLayer;
                    }
                    //or activate text input
                    else if (childPtr2 && buttTmp->focus){
                        allowTextInput = true;
                        childPtr2->activeInput = true;
                        childPtr2->text.setString(childPtr2->userText);
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


////INPUT EVENTS
            //Text is entered
            else if (event.type == sf::Event::TextEntered && allowTextInput){
                for (button *buttTmp : Layers[currentLayer]->LayerButtons){
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
            else if (currentMenuPopUp == menuPopUps::InvalidName){
                if (event.type == sf::Event::KeyPressed && (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)||sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))){
                    currentMenuPopUp = menuPopUps::NoPopUp;
                }
            }
        }
        ////END INPUT EVENTS


        ////BEGIN UPDATES (to avoid bugs and crashes.. only update valid layers)
        if (currentLayer != layersId::final && currentLayer != layersId::leave && currentMenuPopUp == menuPopUps::NoPopUp){
            Layers[currentLayer]->update(cursor.returnPosition());
            if (!allowTextInput){
                cursor.update();
            }
            getMenuPicks(cursor.returnPosition(), currentLayer, Layers[currentLayer]);
        }
        ////END UPDATES
    }////END OF MENULOPP!

    for (menuLayer* objc : Layers){
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
            std::cout << "Active Game: " << activeSave->getSaveName() <<std::endl;
        }
        else {
            std::cout << "Joining to: ";
            std::cout << hostIp << "::" << hostPort <<std::endl;
        }
        
        return GAME_STATE::GAME;
    }

    std::cout << "Leaving Application!\n";
    return GAME_STATE::QUIT;
}



void Application::getMenuPicks(sf::Vector2f cursorPosition, layersId currentLayer, menuLayer *Lay){
    if (currentLayer == layersId::Type){
        int i = 0;
        for (button *iterator : Lay->LayerButtons){
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
    else if (currentLayer == layersId::Multiplayer){
        if (cursorPosition.y < resolution.y/2){
            wantsHost = true;
        }
        else {
            wantsHost = false;
        }
    }
}



bool Application::checkCharacterInput(layersId activeLayer, sf::Uint32 c, int activeLength){
    if (activeLayer == layersId::Host){//Entering a new savename only allow "a-z", "A-Z", "0-9".
        if (activeLength >= MAX_LENGTH_SAVENAME){
            return false;
        }
        if ((c < 48 || c > 57) && (c < 65 || c > 90) && (c < 97 || c > 122) && c != 32){
            return false;
        }
    }
    if (activeLayer == layersId::Join) {//Entering "0-9", "." , ":" for the ip
        if (activeLength >=MAX_LENGTH_IPPORT){
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
            activeSave = new gameSave(sum.saveName,sum.pathName);
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

void Application::drawMenuPopUp(menuPopUps PopUp, sf::RenderWindow *window, sf::Text *warningMessage){
    window->draw(*warningMessage);
}



///////////////////////
//LAYER CLASS
///////////////////////
menuLayer::menuLayer(layersId assignedLayer, Application *applicationPointer){
    layerInformation info;
    info.lay = assignedLayer;
    layerType = assignedLayer;
    if (assignedLayer == layersId::Base){
        info.nButtons = 3;
        info.buttonTexts.push_back(std::string("Play"));
        info.buttonTexts.push_back(std::string("Settings"));//16:9 1920x1080, 1280x720, 960x540
        info.buttonTexts.push_back(std::string("Exit"));
        info.followUpLay.push_back(layersId::Type);
        info.followUpLay.push_back(layersId::Settings);
        info.followUpLay.push_back(layersId::leave);
        previousLayer = layersId::leave;
    }
    else if (assignedLayer == layersId::Settings){
        info.nButtons = 2;
        info.buttonTexts.push_back(std::string("Graphics"));
        info.buttonTexts.push_back(std::string("Controls"));
        info.followUpLay.push_back(layersId::Graphic);
        info.followUpLay.push_back(layersId::Controls);
        previousLayer = layersId::Base;
    }
    else if (assignedLayer == layersId::Graphic){
        info.nButtons = 5;
        info.buttonTexts.push_back(std::string("1920 x 1080"));
        info.buttonTexts.push_back(std::string("1600 x 900"));
        info.buttonTexts.push_back(std::string("1280 x 720"));
        info.buttonTexts.push_back(std::string("1024 x 576"));
        info.buttonTexts.push_back(std::string("960 x 540"));

        info.followUpLay.push_back(layersId::Graphic);    
        info.followUpLay.push_back(layersId::Graphic);    
        info.followUpLay.push_back(layersId::Graphic);    
        info.followUpLay.push_back(layersId::Graphic);    
        info.followUpLay.push_back(layersId::Graphic);    

        previousLayer = layersId::Settings;
    }
    else if (assignedLayer == layersId::Controls){
        info.nButtons = 0;
        previousLayer = layersId::Settings;
    }
    else if (assignedLayer == layersId::Multiplayer){
        info.nButtons = 2;
        info.buttonTexts.push_back(std::string("Host"));
        info.buttonTexts.push_back(std::string("Join"));
        info.followUpLay.push_back(layersId::Host);
        info.followUpLay.push_back(layersId::Join);
        previousLayer = layersId::Type;

    }
    else if (assignedLayer == layersId::Type){
        info.nButtons = 3;
        info.buttonTexts.push_back(std::string("Alone"));
        info.buttonTexts.push_back(std::string("Local"));
        info.buttonTexts.push_back(std::string("Online"));
        info.followUpLay.push_back(layersId::Host);
        info.followUpLay.push_back(layersId::Multiplayer);
        info.followUpLay.push_back(layersId::Multiplayer);
        previousLayer = layersId::Base;
    }
    else if (assignedLayer == layersId::Join){
        info.nButtons = 2;
        info.buttonTexts.push_back(std::string("Host Ip"));
        info.buttonTexts.push_back(std::string("Host Port"));
        info.followUpLay.push_back(layersId::final);
        info.followUpLay.push_back(layersId::final);
        previousLayer = layersId::Multiplayer;
    }
    else if (assignedLayer == layersId::Host){
        info.buttonTexts.push_back(std::string("Create New Save"));
        info.followUpLay.push_back(layersId::final);
        for (int i = 0; i < applicationPointer->availableSaveFiles.size(); i++){
            info.buttonTexts.push_back(applicationPointer->availableSaveFiles[i].saveName);
            info.followUpLay.push_back(layersId::final);
        }
        info.nButtons = 1 + info.buttonTexts.size();
        previousLayer = layersId::Multiplayer;
    }
    else {
        assert(("ERROR: Attempted to create invalid menu layer!\n", false));
    }


    init(info, applicationPointer);
}



void menuLayer::init(layerInformation setUpInfo, Application *applicationPointer){
    //Load click buttons for each savefile and new writing button to create a new one
    if (layerType == layersId::Host){
        LayerButtons.push_back(new WriteButton(setUpInfo.buttonTexts[0], setUpInfo.followUpLay[0], applicationPointer, setUpInfo.nButtons, 0));
        for (int i = 1; i < setUpInfo.buttonTexts.size(); i++){
            LayerButtons.push_back(new ClickButton(setUpInfo.buttonTexts[i], setUpInfo.followUpLay[i], applicationPointer, setUpInfo.nButtons, i));
        }
    } 
    //Only layer with 2 writing buttons
    else if (layerType == layersId::Join){
        for (int i = 0; i < setUpInfo.nButtons; i++){
            LayerButtons.push_back(new WriteButton(setUpInfo.buttonTexts[i], setUpInfo.followUpLay[i], applicationPointer, setUpInfo.nButtons, i));
        }
    }    
    else if (layerType == layersId::Graphic){
        sf::Vector2u rtmp[5] = {sf::Vector2u(1920,1080),sf::Vector2u(1600,900),sf::Vector2u(1280,720),sf::Vector2u(1024,576),sf::Vector2u(960,540)};
        for (int i = 0; i < setUpInfo.nButtons; i++){
            LayerButtons.push_back(new GraphicButton(setUpInfo.buttonTexts[i], setUpInfo.followUpLay[i], applicationPointer, setUpInfo.nButtons, i, rtmp[i]));
        }
    }
    //everything else is just clicking
    else {
        for (int i = 0; i < setUpInfo.nButtons; i++){
            LayerButtons.push_back(new ClickButton(setUpInfo.buttonTexts[i], setUpInfo.followUpLay[i], applicationPointer, setUpInfo.nButtons, i));
        }
    } 
}



void menuLayer::update(sf::Vector2f mousePos){
    for (button *obj : LayerButtons){
        obj->update(mousePos);
    }
}



void menuLayer::changeRes(Application *applicationPointer){
    int i = 0;
    for (button *obj : LayerButtons){
        obj->changeRes(applicationPointer, LayerButtons.size(), i);
        i++;
    }
}

menuLayer::~menuLayer(void){
    for (button *obj : LayerButtons){
        delete obj;
    }
}



///////////////////////
//BUTTON CLASS(es)
///////////////////////
//pretending we define the parent class functions
void button::draw(sf::RenderWindow *window){std::cout << stringText << " parent class" <<std::endl;};
void button::update(sf::Vector2f mousePos){}
void button::changeRes(Application *applicationPointer, int maxButt, int currButt){}

//Pretending over

ClickButton::ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt){
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



WriteButton::WriteButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt){
    stringText = t;
    nextLayer = followLayer;
    activeInput = false;
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
GraphicButton::GraphicButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt, sf::Vector2u newRes){
    newResolution = newRes;

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