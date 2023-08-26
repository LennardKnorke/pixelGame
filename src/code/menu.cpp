#include "application.hpp"
#include "menu.hpp"

GAME_STATE Application::menuLoop(void){
    readAllSaveFiles();
    layersId currentLayer = Base;
    //Set up menu layers
    std::vector<menuLayer*> Layers;
    layersId visibLayers[MaxVisibleLayers] = {Base, Settings, Play, Type, Join, Host};
    for (int i =0; i < MaxVisibleLayers; i++){
        Layers.push_back(new menuLayer(visibLayers[i], this));
    }
    

    //actual menu loop
    bool allowTextInput = false;
    while (currentLayer != final && currentLayer != leave){



        ////BEGIN DRAWING
        window.clear(sf::Color::Black);
        for (button *obj : Layers[currentLayer]->LayerButtons){
            obj->draw(&window);
        }

        if (!allowTextInput){
            cursor.draw(&window);
        }
        window.display();
        ////END DRAWING




        ////BEING INPUT EVENTS
        sf::Event event;
        while (window.pollEvent(event)){
            //KEYS PRESSED
            if (event.type == sf::Event::KeyPressed){
                //escape button pressed
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                    if (allowTextInput){
                        allowTextInput = false;
                        for (button *buttTmp : Layers[currentLayer]->LayerButtons){
                            WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                            if (childPtr1){
                                childPtr1->resetInput();
                            }
                        }
                    }
                    else {
                        currentLayer = Layers[currentLayer]->previousLayer;
                    }
                }
                //enter button pressed
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && allowTextInput){
                    for (button *buttTmp : Layers[currentLayer]->LayerButtons){
                        WriteButton *childPtr1 = dynamic_cast<WriteButton*>(buttTmp);
                        if (childPtr1 && childPtr1->focus && childPtr1->activeInput){
                            allowTextInput = false;
                            childPtr1->activeInput = false;
                            if (currentLayer == Host){
                                //Create a new Play save
                                std::string newSaveName = childPtr1->userText;
                                if (createSaveFile(newSaveName)){
                                    currentLayer = childPtr1->nextLayer;
                                }
                                else {
                                    childPtr1->resetInput();
                                }
                                
                            }
                            else if (currentLayer == Join){
                                if (cursor.sprite[cursor.activeSprite].getPosition().y < resolution.y/2){
                                    //Entered HOST IP ADRESS
                                    hostIp = childPtr1->userText;
                                }
                                else {
                                    //ENTERED HOST PORT
                                    hostPort = childPtr1->userText;
                                }
                                if (hostIp != "" && hostPort != ""){
                                    currentLayer = final;
                                }
                            }
                        }
                    }
                }
            }
            //Mouse is pressed
            else if (event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !allowTextInput){
                for (button *buttTmp : Layers[currentLayer]->LayerButtons){
                    ClickButton *childPtr1 = dynamic_cast<ClickButton*>(buttTmp);
                    WriteButton *childPtr2 = dynamic_cast<WriteButton*>(buttTmp);
                    //Either just change the layer
                    if (childPtr1 && buttTmp->focus){                            
                        currentLayer = childPtr1->nextLayer;
                        if (currentLayer == final){

                        }
                    }
                    //or activate text input
                    else if (childPtr2 && buttTmp->focus){
                        allowTextInput = true;
                        childPtr2->activeInput = true;
                        childPtr2->text.setString(childPtr2->userText);
                    }
                }
            }
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
        ////END INPUT EVENTS


        ////BEGIN UPDATES (to avoid bugs and crashes.. only update valid layers)
        if (currentLayer != final && currentLayer != leave){
            Layers[currentLayer]->update(cursor.returnPosition());
            if (!allowTextInput){
                cursor.update();
            }
            getMenuPicks(cursor.returnPosition(), currentLayer);
        }
        ////END UPDATES
    }////END OF MENULOPP!

    for (menuLayer* objc : Layers){
        delete objc;
    }
    ////Return new gameState
    if (currentLayer == final){
        if (online){
            std::cout << "Playing Online\n";
        }
        else {
            std::cout << "Playing locally\n";
        }
        if (wantsHost){
            std::cout << "Hosting save: " << activeSave.saveName <<std::endl;
        }
        else{
            std::cout << "Joining to: ";
            std::cout << hostIp << "\t" << hostPort <<std::endl;
        }
        
        return GAME;
    }
    std::cout << "Leaving Application!\n";
    return QUIT;
}

void Application::getMenuPicks(sf::Vector2f cursorPosition, layersId currentLayer){
    if (currentLayer == Type){
        if (cursorPosition.y < resolution.y/2){
            online = false;
        }
        else {
            online = true;
        }
    }
    else if (currentLayer == Play){
        if (cursorPosition.y < resolution.y/2){
            wantsHost = true;
        }
        else {
            wantsHost = false;
        }
    }
}

bool Application::checkCharacterInput(layersId activeLayer, sf::Uint32 c, int activeLength){
    if (activeLayer == Host){//Entering a new savename only allow "a-z", "A-Z", "0-9".
        if (activeLength >= MAX_LENGTH_SAVENAME){
            return false;
        }
        if ((c < 48 || c > 57) && (c < 65 || c > 90) && (c < 97 || c > 122) && c != 32){
            return false;
        }
    }
    else {//Entering "0-9", "." , ":" for the ip
        if (activeLength >=MAX_LENGTH_IPPORT){
            return false;
        }
        if ((c < 48 || c > 58) && c != 46){
            return false;
        }
    }
    
    return true;
}
///////////////////////
//LAYER CLASS
///////////////////////

menuLayer::menuLayer(layersId assignedLayer, Application *applicationPointer){
    layerInformation info;
    info.lay = assignedLayer;
    layerType = assignedLayer;
    if (assignedLayer == Base){
        info.nButtons = 3;
        info.buttonTexts.push_back(std::string("Play"));
        info.buttonTexts.push_back(std::string("Settings"));//16:9 1920x1080, 1280x720, 960x540
        info.buttonTexts.push_back(std::string("Exit"));
        info.followUpLay.push_back(Type);
        info.followUpLay.push_back(Settings);
        info.followUpLay.push_back(leave);
        previousLayer = leave;
    }
    else if (assignedLayer == Settings){
        info.nButtons = 0;

        previousLayer = Base;
    }
    else if (assignedLayer == Play){
        info.nButtons = 2;
        info.buttonTexts.push_back(std::string("Host"));
        info.buttonTexts.push_back(std::string("Join"));
        info.followUpLay.push_back(Host);
        info.followUpLay.push_back(Join);
        previousLayer = Type;

    }
    else if (assignedLayer == Type){
        info.nButtons = 2;
        info.buttonTexts.push_back(std::string("Local"));
        info.buttonTexts.push_back(std::string("Online"));
        info.followUpLay.push_back(Play);
        info.followUpLay.push_back(Play);
        previousLayer = Base;
    }
    else if (assignedLayer == Join){
        info.nButtons = 2;
        info.buttonTexts.push_back(std::string("Host Ip"));
        info.buttonTexts.push_back(std::string("Host Port"));
        info.followUpLay.push_back(final);
        info.followUpLay.push_back(final);
        previousLayer = Play;
    }
    else if (assignedLayer == Host){
        info.buttonTexts.push_back(std::string("Create New Save"));
        info.followUpLay.push_back(final);
        for (int i = 0; i < applicationPointer->availableSaveFiles.size(); i++){
            info.buttonTexts.push_back(applicationPointer->availableSaveFiles[i].saveName);
            info.followUpLay.push_back(final);
        }
        info.nButtons = 1 + info.buttonTexts.size();
        previousLayer = Play;
    }
    else {
        assert(("ERROR: Attempted to create invalid menu layer!\n", false));
    }


    init(info, applicationPointer);
}

void menuLayer::init(layerInformation setUpInfo, Application *applicationPointer){
    //Load click buttons for each savefile and new writing button to create a new one
    if (layerType == Host){
        LayerButtons.push_back(new WriteButton(setUpInfo.buttonTexts[0], setUpInfo.followUpLay[0], applicationPointer, setUpInfo.nButtons, 0));
        for (int i = 1; i < setUpInfo.buttonTexts.size(); i++){
            LayerButtons.push_back(new ClickButton(setUpInfo.buttonTexts[i], setUpInfo.followUpLay[i], applicationPointer, setUpInfo.nButtons, i));
        }
    } 
    //Only layer with 2 writing buttons
    else if (layerType == Join){
        for (int i = 0; i < setUpInfo.nButtons; i++){
            LayerButtons.push_back(new WriteButton(setUpInfo.buttonTexts[i], setUpInfo.followUpLay[i], applicationPointer, setUpInfo.nButtons, i));
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
//Pretending over


ClickButton::ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt){
    stringText = t;
    nextLayer = followLayer;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(42);
    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getLocalBounds().width/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getLocalBounds().height / 2));
    text.setStyle(sf::Text::Style::Regular);
};
ClickButton::~ClickButton(void){};


void ClickButton::draw(sf::RenderWindow *window){
    window->draw(text);
}
void ClickButton::update(sf::Vector2f mousePos){
    sf::Vector2f rec = text.getPosition();
    if (mousePos.x > rec.x && mousePos.x < rec.x + text.getLocalBounds().width
    && mousePos.y > rec.y && mousePos.y < rec.y + text.getLocalBounds().height){
        focus = true;
        if (text.getStyle() != sf::Text::Style::Bold){
            text.setStyle(sf::Text::Style::Underlined);
        }
    }    
    else {
        focus = false;
        if (text.getStyle() != sf::Text::Style::Regular){
            text.setStyle(sf::Text::Style::Regular);
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
    text.setCharacterSize(42);
    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getLocalBounds().width/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getLocalBounds().height / 2));
    ogPosition[0] = text.getPosition().x;
    ogPosition[1] = text.getPosition().y;
    text.setStyle(sf::Text::Style::Regular);
}
WriteButton::~WriteButton(void){};

void WriteButton::draw(sf::RenderWindow *window){
    window->draw(text);
}
void WriteButton::update(sf::Vector2f mousePos){
    if (!activeInput){
        sf::Vector2f rec = text.getPosition();
        if (mousePos.x > rec.x && mousePos.x < rec.x + text.getLocalBounds().width
        && mousePos.y > rec.y && mousePos.y < rec.y + text.getLocalBounds().height){
            focus = true;
            if (text.getStyle() != sf::Text::Style::Bold){
                text.setStyle(sf::Text::Style::Underlined);
            }
        }
        else {
            focus = false;
            if (text.getStyle() != sf::Text::Style::Regular){
                text.setStyle(sf::Text::Style::Regular);
            }
        }
    }
}

void WriteButton::resetInput(void){
    text.setString(stringText);
    text.setPosition(ogPosition[0], ogPosition[1]);
    userText = "";
    activeInput = false;
}

void WriteButton::addInput(sf::Uint32 input, sf::Vector2u res){
    if (userText.size() < maxInput){
        userText += input;
        text.setString(userText);
        text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);
    }
}

void WriteButton::delLastInput(sf::Vector2u res){
    if (userText.size() != 0){
        userText.erase(userText.size() - 1, 1);
        text.setString(userText);
        text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);
    }   
}