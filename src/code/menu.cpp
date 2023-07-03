#include "application.hpp"
#include "menu.hpp"


int Application::menu(void){
    bool menu_running = true;
    int next_state = 0;

    int current_layer = 0;
    int idx = 0;
    sf::Color c[2] = {sf::Color::Black, sf::Color::Red};
    while (menu_running){
        //Draw
        window.clear(c[idx]);
        window.display();
        //Manage events
        sf::Event event;
        while (window.pollEvent(event)){
            //Close if the window is closed
            if (event.type == sf::Event::Closed){
                window.close();
                menu_running = false;
                next_state = STATE_QUIT;
            }
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape){
                    window.close();
                    menu_running = false;
                    next_state = STATE_QUIT;
                }
                if (event.key.code == sf::Keyboard::Space){
                    if (idx == 0){
                        idx = 1;
                    }
                    else {
                        idx = 0;
                    }
                }
            }
        }
        //Update
        mousePosition = sf::Mouse::getPosition();
    }
    return next_state;
}