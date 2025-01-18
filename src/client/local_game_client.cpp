#include "application.hpp"
#include "local_game_client.hpp"

GAME_STATE Application::gameLoop(void){
    // init local client game and start server thread
    Local_Client_Game *game;

    if (mode == gameMode::Single || mode == gameMode::Host){
        // set up host adress
        loadingTools.port = findFreePort();
        loadingTools.ip = sf::IpAddress::getLocalAddress();

        // Put together command line argument
        std::string arguments = "Server.exe";
        arguments += " " + std::to_string(loadingTools.port);
        arguments += " " + loadingTools.ip.toString();
        arguments += " " + loadingTools.chosenSave.path;
        if (mode == gameMode::Single){
            arguments += " " + std::to_string(1);
        }
        else {
            arguments += " " + std::to_string(4);
        }
        arguments += " " + std::to_string(DEV_MODE);

        // Start server process
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        //Ignore the syntax error?!
        if (!CreateProcess(NULL, (LPSTR)arguments.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)){
            std::cerr << "\tError starting server" << std::endl;
            return GAME_STATE::QUIT;
        }
        if (DEV_MODE){
            std::cout << "\tServer started" << std::endl;
        }
        

    }

    game = new Local_Client_Game(&window, cursor, settings, &assets, &DEV_MODE, loadingTools.port, loadingTools.ip.toString());
    GAME_STATE next = game->run();

    delete game;
    return next;
}


Local_Client_Game::Local_Client_Game(sf::RenderWindow *win, Cursor *cur, settings_class *set, asset_manager *ass, bool *dev, unsigned short host_port, std::string host_ip){
    window = win;
    cursor = cur;
    settings = set;
    assets = ass;
    DEV_MODE = dev;

    host_adress.port = host_port;
    host_adress.ip = sf::IpAddress(host_ip);

    state = CONNECTING_;
    loadingThread = new sf::Thread(Local_Client_Game::loadingScreen_t, this);
    
    if (*DEV_MODE){
        std::cout << "LOCAL GAME CLIENT INITIALIZED" << std::endl;
        std::cout << "\tHost IP: " << host_adress.ip.toString() << std::endl;
        std::cout << "\tHost Port: " << host_adress.port << std::endl;
    }
}

GAME_STATE Local_Client_Game::run(void){
    // Attempt to connect to host
    window->setActive(false);
    loadingThread->launch();
    connectToServer();
    loadingThread->wait();
    window->setActive(true);

    // Exit gracefully if connecting and loading failed
    if (state != local_states::RUNNING_){
        if (DEV_MODE){
            std::cout << "\tFailed to connect or load World data from host.\n";
        }
        return GAME_STATE::MENU;
    }
    if (DEV_MODE){
        std::cout << "\tLoading Complete" << std::endl;
    }
    // Start gameloop
    
    inGameLayer layer = inGameLayer::game;
    while (state==local_states::RUNNING_){
        draw(layer, *window);

        handleUserInput(layer, *window);

        updateClient();

        sf::Event ev;

        while(window->pollEvent(ev)){
            if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                state = local_states::QUIT_;
            }
        }

        cursor->update();
    }
    socket.disconnect();
    return GAME_STATE::MENU;
}

void Local_Client_Game::loadingScreen_t(void){
    window->setActive(true);

    sf::Text text;
    text.setFont(assets->gameFont);
    text.setString("Attempting to connect to host");
    text.setCharacterSize(GAMEFONT_SIZE * settings->res.scaling.x);
    text.setFillColor(sf::Color::Red);
    text.setPosition(sf::Vector2f(settings->res.width/2 - text.getGlobalBounds().width/2, settings->res.height/2 - text.getGlobalBounds().height/2));

    if (DEV_MODE){
        std::cout << "\tDrawing loading screen\n";
    }
    while (state == local_states::CONNECTING_){
        window->clear(sf::Color::Black);
        window->draw(text);
        cursor->draw(*window);
        window->display();


        cursor->update();
    }
    window->setActive(false);
}

void Local_Client_Game::connectToServer(void){
    for (auto i = 0; i< 5; i++){
        if (socket.connect(host_adress.ip, host_adress.port) == sf::Socket::Done){
            std::cout << "\tConnected to host" << std::endl;
            break;
        }
        if (i == 4){
            state = local_states::QUIT_;
            return;
        }
        std::cout << "\tFailed to connect to host. Retrying in 3 seconds" << std::endl;
        sf::sleep(sf::seconds(3));
    }
    receiveWorld();
    return;
}

void Local_Client_Game::receiveWorld(void){
    sf::Packet packet;
    packet << settings->get_userId();
    socket.send(packet);
    packet.clear();
    socket.receive(packet);
    std::string response;
    packet >> response;
    packet.clear();
    std::cout << response << std::endl;

    state = local_states::RUNNING_;
    return;
}


void Local_Client_Game::draw(inGameLayer &layer, sf::RenderWindow &window){
    if (layer == inGameLayer::game){
        window.clear(sf::Color::Blue);
    }
    else if (layer == inGameLayer::menu){
        window.clear(sf::Color::Green);
    }
    else if (layer == inGameLayer::skilltree){
        window.clear(sf::Color::Red);
    }
    cursor->draw(window);
    window.display();
}


void Local_Client_Game::handleUserInput(inGameLayer &layer, sf::RenderWindow &window){
    if (layer == inGameLayer::game){
        handleGameInput(layer, window);
    }
    else if (layer == inGameLayer::menu){
        handleMenuInput(layer, window);
    }
    else if (layer == inGameLayer::skilltree){
        handleSkilltreeInput(layer, window);
    }
}
void Local_Client_Game::handleGameInput(inGameLayer &layer, sf::RenderWindow &window){
    sf::Event ev;
    while (window.pollEvent(ev)){
        if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            layer = inGameLayer::menu;
        }
    }
    return;
}
void Local_Client_Game::handleMenuInput(inGameLayer &layer, sf::RenderWindow &window){
    sf::Event ev;
    while (window.pollEvent(ev)){
        if (ev.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            layer = inGameLayer::game;
        }
    }
    return;
}


void Local_Client_Game::updateClient(void){
    return;
}