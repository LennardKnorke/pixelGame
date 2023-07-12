#include "application.hpp"
#include "menu.hpp"


int Application::menu(void){
    bool menu_running = true;
    bool active_mouse = true;
    int next_state = STATE_QUIT;
    bool active_input = false;
    initializeMenu();

    int current_layer = LAYER_BASE;
    std::cout << "Reached Menu" << std::endl;
    while (menu_running){
        //START DRAWING
        window.clear(sf::Color::Black);
        //Draw default buttons
        for (int i = 0; i < MAX_DEFAULTBUTTONS; i++){
        }
        //Draw non-default buttons

        //Draw Mouse
        /*
        if (active_mouse){
            window.draw(cursor.sprite[cursor.activeSprite]);
        }
        */
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

                    if (!active_input){
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
                }
            }
        }
        //END EVENTS/INPUT

        //START UPDATE
        cursor.update();
        //Update MenuButtons
        for (int i = 0; i < MAX_DEFAULTBUTTONS; i++){
        }
        //END UPDATE

    }//END OF MENU LOOP

    return next_state;
}

//Sets up the buttons for the menu
void Application::initializeMenu(void){
    /////////////////////////////////////////////////////////////////////
    int dButtons_layer[MAX_DEFAULTBUTTONS] = {LAYER_BASE,LAYER_BASE, LAYER_BASE, 
                                                LAYER_PLAY, LAYER_PLAY, 
                                                LAYER_JOIN, LAYER_JOIN, 
                                                LAYER_HOST, LAYER_HOST, 
                                                LAYER_HOST_SAVE, LAYER_HOST_SAVE, 
                                                LAYER_HOST_NEWSAVE};
    std::string displayText[MAX_DEFAULTBUTTONS] = { "Play", "Settings", "Exit", 
                                                    "Host", "Join",
                                                    "Host IPAdress", "Host Port", 
                                                    "Online", "Local", 
                                                    "Create Save", "Load Save", 
                                                    "New Save"};
    int dButtons_nextLayer[MAX_DEFAULTBUTTONS] = {LAYER_PLAY, LAYER_SETTINGS, Layer_leave,
                                                    LAYER_HOST, LAYER_JOIN, 
                                                    Layer_final, Layer_final, 
                                                    LAYER_HOST_SAVE, LAYER_HOST_SAVE,
                                                    LAYER_HOST_NEWSAVE, LAYER_HOST_OLDSAVE,
                                                    Layer_final};
    int max_button_perLayer[MAX_DEFAULTBUTTONS] = {3, 3, 3,
                                                    2, 2,
                                                    2,2,
                                                    2,2,
                                                    2,2,
                                                    1};
    for (int i = 0; i < MAX_DEFAULTBUTTONS; i++){

    }

    //LOAD NON DEFAULT BUTTONS AKA SAVEFILE BUTTONS
}

////////////////////////////////////////////
//BUTTON CLASS
////////////////////////////////////////////


