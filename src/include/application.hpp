#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"

#define MAX_LENGTH_KEY 14
#define STATE_MENU 1
#define STATE_GAME 2
#define STATE_QUIT 0 
class Application{
    public:
    Application(void);
    private:
    //Window related variables
    sf::RenderWindow window;
    sf::Vector2u resolution;
    sf::Vector2i mousePosition = sf::Mouse::getPosition();

    sf::Font gameFont;
    sf::Text menuText;
    //Socket variables
    sf::IpAddress publicAdress = sf::IpAddress::getPublicAddress();
    sf::IpAddress localAdress = sf::IpAddress::getLocalAddress();
    //Application variables
    int applicationState;   //0=quit, 1= run menu, 2=run game
    int errorHandling;      //0= no error...
    bool applicationFocused;
    char userKey[MAX_LENGTH_KEY + 1];

    ////////////////FUNCTIONS////////////////
    //Boot ups
    void initWindow(void);
    void initSettings(void);

    //Orga Stuff
    void loadSettings(const std::string &filename);
    void createSettings(const std::string &filename);
    void saveSettings(const std::string &filename);

    bool fileExists(const std::string &filename);

    //Game Loop
    int menu(void);
    void updateMenu(void);
    int game(void);
};


#endif //APPLICATION_HPP