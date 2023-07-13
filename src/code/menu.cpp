#include "application.hpp"
#include "menu.hpp"


int Application::menu(void){
    bool menu_running = true;
    bool active_mouse = true;
    bool holding_mouse = false;
    int next_state = STATE_QUIT;
    bool writing_input = false;
    initializeMenu();

    int current_layer = LAYER_BASE;
    while (menu_running && current_layer != Layer_leave){
        //START DRAWING
        window.clear(sf::Color::Black);
        //Draw buttons
        for (int i = 0; i < MAX_DEFAULTBUTTONS_CLICK; i++){
            if (Buttons_Menu.ClickButtons[i].getLayer() == current_layer){
                Buttons_Menu.ClickButtons[i].draw(&window);
            }
        }
        for (int i = 0; i< MAX_DEFAULTBUTTONS_WRITING; i++){

        }
        for (int i = 0; i <Buttons_Menu.SaveFileButtons.size();i++){
        }
        //Draw non-default buttons

        //Draw Mouse
        if (active_mouse){
            cursor.draw(&window);
            //window.draw(cursor.sprite[cursor.activeSprite]);
        }

        //Draw Error

        window.display();
        //END DRAWING

        //START EVENTS/INPUT
        sf::Event event;
        while (window.pollEvent(event)){
            //Close if the window is closed
            if (event.type == sf::Event::Closed){
                window.close();
                menu_running = false;
                next_state = STATE_QUIT;
            }
            //Handle Key Input
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape){
                    //Change layer in case you are not writing 
                    if (!writing_input){
                        if (current_layer == LAYER_BASE){
                            window.close();
                            menu_running = false;
                            next_state = STATE_QUIT;
                        }
                        else {
                            if (current_layer == LAYER_SETTINGS || current_layer == LAYER_HOST_NEWSAVE || current_layer == LAYER_JOIN || current_layer == LAYER_HOST_SAVE){
                                current_layer--;
                            }
                            else if (current_layer == LAYER_PLAY || current_layer == LAYER_HOST || current_layer == LAYER_HOST_OLDSAVE){
                                current_layer -= 2;
                            }
                        }
                    }
                    //Otherwise cancel writing
                    else {

                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed){
                for (int i = 0; i < MAX_DEFAULTBUTTONS_CLICK; i++){
                    if (Buttons_Menu.ClickButtons[i].focus){
                        current_layer = Buttons_Menu.ClickButtons[i].getNextLayer();
                        i = MAX_DEFAULTBUTTONS_CLICK;
                        std::cout<<current_layer<<std::endl;
                    }
                }
            }
        }
        //END EVENTS/INPUT

        //START UPDATE
        cursor.update();
        //Update MenuButtons
        for (int i = 0; i < MAX_DEFAULTBUTTONS_CLICK; i++){
            Buttons_Menu.ClickButtons[i].update(cursor.returnPosition(), cursor.pressed);
        }
        //END UPDATE

    }//END OF MENU LOOP

    return next_state;
}

//Sets up the buttons for the menu
void Application::initializeMenu(void){
    /////////////////////////////////////////////////////////////////////
    //Load default Buttons which are always displayed
    
    int Click_layer[MAX_DEFAULTBUTTONS_CLICK] = {LAYER_BASE,LAYER_BASE, LAYER_BASE, 
                                                        LAYER_PLAY, LAYER_PLAY,
                                                        LAYER_HOST, LAYER_HOST, 
                                                        LAYER_HOST_SAVE, LAYER_HOST_SAVE
                                                        };
    std::string Click_displayText[MAX_DEFAULTBUTTONS_CLICK] = { "Play", "Settings", "Exit", 
                                                    "Host IPAdress", "Host Port", 
                                                    "Online", "Local", 
                                                    "Create Save", "Load Save"};
    int Click_nextLayer[MAX_DEFAULTBUTTONS_CLICK] = {LAYER_PLAY, LAYER_SETTINGS, Layer_leave,
                                                    LAYER_HOST, LAYER_JOIN, 
                                                    LAYER_HOST_SAVE, LAYER_HOST_SAVE,
                                                    LAYER_HOST_NEWSAVE, LAYER_HOST_OLDSAVE};
    int Click_maxButtons[MAX_DEFAULTBUTTONS] = {3, 3, 3,
                                                2,2,
                                                2,2,
                                                2,2};    
    for (int i = 0; i < MAX_DEFAULTBUTTONS_CLICK; i++){
        Buttons_Menu.ClickButtons[i].init(Click_displayText[i],0, Click_layer[i], i, Click_maxButtons[i], Click_nextLayer[i]);
        Buttons_Menu.ClickButtons[i].text.setFont(gameFont);
        Buttons_Menu.ClickButtons[i].updatePosition(resolution);
    }                                    


    std::string Write_displayText[MAX_DEFAULTBUTTONS_CLICK] = {"Host IPAdress", "Host Port",
                                                        "New Save"};

    int Write_layer[MAX_DEFAULTBUTTONS_WRITING] = {LAYER_JOIN, LAYER_JOIN,
                                                    LAYER_HOST_NEWSAVE};
    int Write_nextLayer[MAX_DEFAULTBUTTONS_CLICK] = {Layer_final, Layer_final,
                                                        Layer_final};                      
    int Write_maxButtons[MAX_DEFAULTBUTTONS] = {2,2,
                                                1};
    for (int i = 0; i < MAX_DEFAULTBUTTONS_WRITING; i++){

    }

    //LOAD NON DEFAULT BUTTONS AKA SAVEFILE BUTTONS
}

////////////////////////////////////////////
//BUTTON CLASS
////////////////////////////////////////////

void Menu_Button_Click::init(std::string display_text, int textureIdx, int currentlayer, int buttonIdx, int maxButtonIdx,int followUpLayer){
    stringText = display_text;
    layer = currentlayer;
    nextLayer = followUpLayer;
    idx = buttonIdx;
    maxIdx = maxButtonIdx;
    focus = false;

    text.setString(stringText);
    text.setCharacterSize(42);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Style::Regular);
}

void Menu_Button_Click::updatePosition(sf::Vector2u res){
    float x = (res.x/2) - (text.getLocalBounds().width/2);
    float y = ((res.y/(maxIdx+1)) * (idx + 1)) - (text.getLocalBounds().height/2);
    text.setPosition(x, y);
}

int Menu_Button_Click::getLayer(void){
    return layer;
}

int Menu_Button_Click::getNextLayer(void){
    return nextLayer;
}

void Menu_Button_Click::draw(sf::RenderWindow *window){
    window->draw(text);
}

void Menu_Button_Click::update(sf::Vector2f mouseposition, bool click){
    if (mouseposition.x > text.getPosition().x && mouseposition.x < (text.getPosition().x + text.getLocalBounds().width) && mouseposition.y > text.getPosition().y && mouseposition.y < text.getPosition().y+text.getLocalBounds().height){
        text.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
        focus = true;
    }
    else {
        if (focus){
            focus = false;
            text.setStyle(sf::Text::Style::Regular);
        }
    }
}