#include "application.hpp"
#include "menu.hpp"

GAME_STATE Application::menuLoop(void){
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
        for (button * obj : Layers[currentLayer]->LayerButtons){
            obj->draw(&window);
            //Layers[currentLayer].LayerButtons[i].draw(&window);
        }
        //Layers[currentLayer].draw(&window);
        cursor.draw(&window);
        window.display();
        ////END DRAWING

        ////BEING INPUT EVENTS
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::KeyPressed){
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                    if (!allowTextInput){
                        currentLayer = Layers[currentLayer]->previousLayer;
                    }
                } 
            }
        }
        ////END INPUT EVENTS


        ////BEGIN UPDATES
        Layers[currentLayer]->update();
        cursor.update();
        ////END UPDATES
        

       
    }

    for (menuLayer* objc : Layers){
        delete objc;
    }
    ////Return new gameState
    if (currentLayer == final){
        return GAME;
    }
    return QUIT;
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
        info.buttonTexts.push_back(std::string("Settings"));
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
        info.nButtons = 1;
        info.buttonTexts.push_back(std::string("Create New Save"));
        info.followUpLay.push_back(final);
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

    } 
    //Only layer with 2 writing buttons
    else if (layerType == Join){
        for (int i = 0; i < setUpInfo.nButtons; i++){
        }
    }    
    //everything else is just clicking
    else {
        for (int i = 0; i < setUpInfo.nButtons; i++){
            LayerButtons.push_back(new ClickButton(setUpInfo.buttonTexts[i], setUpInfo.followUpLay[i], applicationPointer, setUpInfo.nButtons, i));
        }
    } 
}



void menuLayer::update(void){
    for (button *obj : LayerButtons){
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
void button::update(void){}
//Pretending over


ClickButton::ClickButton(std::string t, layersId followLayer, Application *applicationPointer, int maxButt, int currButt){
    stringText = t;
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
void ClickButton::update(void){

}

WriteButton::WriteButton(std::string t, layersId followLayer, Application *applicationPointer){

}
WriteButton::~WriteButton(void){};

void WriteButton::draw(sf::RenderWindow *window){

}
void WriteButton::update(void){
    
}