#include "menuButtons.hpp"
#include "application.hpp"
///////////////////////
//BUTTON CLASS(es)
///////////////////////
//pretending we define the parent class functions
void button::draw(sf::RenderWindow &window){std::cout << stringText << " parent class" <<std::endl;};
void button::update(sf::Vector2f mousePos){}
button::~button(){};
//Pretending over

ClickButton::ClickButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, mainMenuLayerId currentLayer){
    layer = currentLayer;
    stringText = t;
    n_button = currButt;
    nextLayer = followLayer;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(GAMEFONT_SIZE);
    text.setStyle(sf::Text::Style::Regular);


    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));

};ClickButton::~ClickButton(void){};





void ClickButton::draw(sf::RenderWindow &window){

    window.draw(text);
}



void ClickButton::update(sf::Vector2f mousePos){
    if (mousePos.x > text.getPosition().x && mousePos.x < text.getPosition().x + text.getGlobalBounds().getSize().x
    && mousePos.y > text.getPosition().y && mousePos.y < text.getPosition().y + text.getGlobalBounds().getSize().y){
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



adressButton::adressButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, mainMenuLayerId given_layer){
    stringText = t;
    nextLayer = followLayer;
    activeInput = false;
    n_button = currButt;
    layer = given_layer;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(GAMEFONT_SIZE);
    

    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));
    ogPosition[0] = text.getPosition().x;
    ogPosition[1] = text.getPosition().y;
    text.setStyle(sf::Text::Style::Regular);
}



adressButton::~adressButton(void){};



void adressButton::draw(sf::RenderWindow &window){
    window.draw(text);
}



void adressButton::update(sf::Vector2f mousePos){
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



void adressButton::resetInput(void){
    text.setString(stringText);
    text.setPosition(ogPosition[0], ogPosition[1]);
    text.setFillColor(sf::Color::Red);
    userText = "";
    activeInput = false;
}



void adressButton::addInput(sf::Uint32 input, sf::Vector2u res){
    userText += input;
    text.setString(userText);
    text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);    
}



void adressButton::delLastInput(sf::Vector2u res){
    if (userText.size() != 0){
        userText.erase(userText.size() - 1, 1);
        text.setString(userText);
        text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);
    }   
}



//
GraphicButton::GraphicButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, sf::Vector2u newRes, mainMenuLayerId currentLayer){
    newResolution = newRes;
    layer = currentLayer;
    stringText = t;
    n_button = currButt;
    nextLayer = followLayer;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(GAMEFONT_SIZE);
    text.setStyle(sf::Text::Style::Regular);

    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));

};



GraphicButton::~GraphicButton(void){};



void GraphicButton::update(sf::Vector2f mousePos){
    if (mousePos.x > text.getPosition().x && mousePos.x < text.getPosition().x + text.getGlobalBounds().getSize().x
    && mousePos.y > text.getPosition().y && mousePos.y < text.getPosition().y + text.getGlobalBounds().getSize().y){
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



void GraphicButton::draw(sf::RenderWindow &window){
    window.draw(text);
}





ProfileButton::ProfileButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, mainMenuLayerId currentLayer){
    layer = currentLayer;
    stringText = t;
    nextLayer = followLayer;
    n_button = currButt;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(GAMEFONT_SIZE);
    text.setStyle(sf::Text::Style::Regular);

    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));

};ProfileButton::~ProfileButton(void){};





void ProfileButton::draw(sf::RenderWindow &window){
    window.draw(text);
}



void ProfileButton::update(sf::Vector2f mousePos){
    if (mousePos.x > text.getPosition().x && mousePos.x < text.getPosition().x + text.getGlobalBounds().getSize().x
    && mousePos.y > text.getPosition().y && mousePos.y < text.getPosition().y + text.getGlobalBounds().getSize().y){
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


newSafeButton::newSafeButton(std::string t, mainMenuLayerId followLayer, Application *applicationPointer, int maxButt, int currButt, mainMenuLayerId given_layer){
    stringText = t;
    nextLayer = followLayer;
    activeInput = false;
    layer = given_layer;
    n_button = currButt;
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(applicationPointer->gameFont);
    text.setCharacterSize(GAMEFONT_SIZE);
    
    text.setPosition(((*applicationPointer).resolution.x/2.0) - (text.getGlobalBounds().getSize().x/2.0), (((*applicationPointer).resolution.y/(maxButt + 1)) * (currButt + 1)) - (text.getGlobalBounds().getSize().y/2));
    ogPosition[0] = text.getPosition().x;
    ogPosition[1] = text.getPosition().y;
    text.setStyle(sf::Text::Style::Regular);
}



newSafeButton::~newSafeButton(void){};



void newSafeButton::draw(sf::RenderWindow &window){
    window.draw(text);
}



void newSafeButton::update(sf::Vector2f mousePos){
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



void newSafeButton::resetInput(void){
    text.setString(stringText);
    text.setPosition(ogPosition[0], ogPosition[1]);
    text.setFillColor(sf::Color::Red);
    userText = "";
    activeInput = false;
}



void newSafeButton::addInput(sf::Uint32 input, sf::Vector2u res){
    userText += input;
    text.setString(userText);
    text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);    
}



void newSafeButton::delLastInput(sf::Vector2u res){
    if (userText.size() != 0){
        userText.erase(userText.size() - 1, 1);
        text.setString(userText);
        text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);
    }   
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FREE FUNCTIONS
void drawMenuButtons(std::vector<button*> &MenuButtons, mainMenuLayerId currentLayer, sf::RenderWindow &window){
    for (button* buttTmp : MenuButtons){
        if (buttTmp->layer == currentLayer){
            buttTmp->draw(window);
        }
    }
}

void setUpMenuButtons(std::vector<button*> &buttonList, Application *applicationPointer){
    //layersId::Base
    mainMenuLayerId currentLayer = mainMenuLayerId::Base;
    int maxButtons = 3;
    std::vector<std::string> text = {"Play", "Settings", "Exit"};
    std::vector<mainMenuLayerId> followUpsLayers = {mainMenuLayerId::GameMode, mainMenuLayerId::Settings, mainMenuLayerId::leave};
    for (int i = 0; i < maxButtons; i++){
        buttonList.push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
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
        buttonList.push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::HostVsClient
    currentLayer = mainMenuLayerId::HostVsClient;
    maxButtons = 2;
    text = {"Host", "Join"};
    followUpsLayers = {mainMenuLayerId::Hosting, mainMenuLayerId::Joining};
    for (int i = 0; i < maxButtons; i++){
        buttonList.push_back(new ClickButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Joining
    currentLayer = mainMenuLayerId::Joining;
    maxButtons = 2;
    text = {"Enter Host Ip", "Enter Host Port"};
    followUpsLayers = {mainMenuLayerId::final, mainMenuLayerId::final};
    for (int i = 0; i < maxButtons; i++){
        buttonList.push_back(new adressButton(text[i], followUpsLayers[i], applicationPointer, maxButtons, i, currentLayer));
    }
    text.clear();
    followUpsLayers.clear();


    //layersId::Hosting
    currentLayer = mainMenuLayerId::Hosting;
    maxButtons = 1;
    text = {"Create New World"};
    followUpsLayers = {mainMenuLayerId::final};
    buttonList.push_back(new newSafeButton(text[0], followUpsLayers[0], applicationPointer, maxButtons + applicationPointer->availableSaveFiles.size(), 0, currentLayer));
    for (long long unsigned int i = 0; i < applicationPointer->availableSaveFiles.size(); i++){
        buttonList.push_back(new ProfileButton(applicationPointer->availableSaveFiles[i].game, mainMenuLayerId::final, applicationPointer, applicationPointer->availableSaveFiles.size()+maxButtons, i+1, mainMenuLayerId::Hosting));
    }

    text.clear();
    followUpsLayers.clear();
}