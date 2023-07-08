#ifndef APPLICATION_HPP
#define APPLICATON_HPP
#include "stdlibs.hpp"
#include "menu.hpp"


#define MAX_LENGTH_KEY 14
#define STATE_MENU 1
#define STATE_GAME 2
#define STATE_QUIT 0 

#define nr_cursor_textures 2
//Simple structure to sort infos of EVERY item that is displayed in the game
typedef struct rectangle {
    sf::FloatRect size;
    sf::Vector2i position;
} rectangle;
//only to keep structure for increasing number of textures
typedef struct Textures {
    sf::Texture cursors[nr_cursor_textures];
    sf::Texture menu[nr_menu_textures];
    //sf::Texture character[nr_character_textures];
    //sf::
} Textures;

class base_sprite {
    public:
    rectangle rec;
};

class CursorSprite : public base_sprite {
    public:
    bool click;
    sf::Sprite sprite[nr_cursor_textures];
    int activeSprite;
    CursorSprite(void);
    void changeSprite(int i);
    void update(void);
};

class Application{
    public:
    Application(void);
    //Window related variables
    sf::RenderWindow window;
    sf::Vector2u resolution;
    CursorSprite cursor;
    
    //////
    //Grafik Variables
    sf::Font gameFont;
    Textures textures;
    
    //Menu related
    Button menuButtons_defaults[MAX_DEFAULTBUTTONS];

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
    bool loadAssets(void);
    bool loadTextures(void);
    void connectTexturesWClasses(void);
    //Orga Stuff
    void loadSettings(const std::string &filename);
    void createSettings(const std::string &filename);
    void saveSettings(const std::string &filename);

    bool fileExists(const std::string &filename);

    bool RectangleCollision(rectangle rec1, rectangle rec2);
    //Menu functions
    int menu(void);
    void initializeMenu(void);


    //game(client) functions
    int game(void);


    //?server functions?
};


#endif //APPLICATION_HPP