#include "menuButtons.hpp"

///////////////////////
//BUTTON CLASS(es)
///////////////////////



//pretending we define the parent class functions
button::button(short i, short max_i, std::string tex, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  unsigned int width, unsigned int height, sf::Vector2f scale, sf::Font &font){
    // Init variables
    layer = currentLayer;
    layer_next = followLayer;
    stringText = tex;
    idx = i;
    max_idx = max_i;
    // Set visual properties
    text = new sf::Text(font, stringText, GAMEFONT_SIZE * scale.y);
    text->setFillColor(sf::Color::White);
    text->setStyle(sf::Text::Style::Regular);

    changeResolution(width, height, scale);

    // Control Variables
    focus = false;
};

void button::update(sf::Vector2f mousePos){
    if (text->getGlobalBounds().contains(mousePos)){
        if (!focus){
            focus = true;
            text->setStyle(sf::Text::Style::Underlined);
            text->setFillColor(sf::Color::Red);
        }
    }    
    else {
        if (focus){
            focus = false;
            text->setStyle(sf::Text::Style::Regular);
            text->setFillColor(sf::Color::White);
        }
    }
};
button::~button(void){};

void button::changeResolution(unsigned int width, unsigned int height, sf::Vector2f scale){
    text->setCharacterSize(GAMEFONT_SIZE * scale.y);
    
    float x = (width - text->getLocalBounds().size.x) / 2.0;
    float y = ((height / float(max_idx + 1)) * (idx + 1)) - (text->getLocalBounds().size.y / 2.0);
    
    text->setPosition(sf::Vector2f(x, y));
};

// CLICK BUTTONS
ClickButton::ClickButton(short i, short max_i, std::string text, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  unsigned int width, unsigned int height, sf::Vector2f scale, sf::Font &font, clickbuttonTypes type)
: button(i, max_i, text, currentLayer, followLayer, width, height, scale, font){
    // Call parent constructor and set type
    this->type = type;
};
ClickButton::~ClickButton(void){};


void ClickButton::update(sf::Vector2f mousePos){
    button::update(mousePos);
}



textButton::textButton(short i, short max_i,std::string tex, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  unsigned int width, unsigned int height, sf::Vector2f scale, sf::Font &font, textbuttonTypes type)
: button(i, max_i, tex, currentLayer, followLayer, width, height, scale, font){
    this->type = type;
    ogPosition[0] = text->getPosition().x;
    ogPosition[1] = text->getPosition().y;

    if (type == textbuttonTypes::adress){
        if (i == 0){
            maxChars = maxInputLengths::ip;
        }
        else {
            maxChars = maxInputLengths::port;
        }
    }
    else if (type == textbuttonTypes::newSafe){
        maxChars = maxInputLengths::SaveName;
    }
}


textButton::~textButton(void){};


void textButton::update(sf::Vector2f mousePos){
    if (!activeInput){
        button::update(mousePos);
    }
    else {
        if (text->getFillColor() != sf::Color::Green){
            text->setFillColor(sf::Color::Green);
        }
    }
}



void textButton::resetInput(void){
    text->setString(stringText);
    text->setPosition(sf::Vector2f(ogPosition[0], ogPosition[1]));
    text->setFillColor(sf::Color::Red);
    userText = "";
    activeInput = false;
}



void textButton::addInput(char input, sf::Vector2u res){
    if (userText.size() < maxChars){
        if (type == textbuttonTypes::adress){
            // Allows numbers for port and ip adress
            if (input >= '0' && input <= '9'){
                userText += input;
            }
            // Allow dots for the ip adress only
            else if (input == '.' && idx == 0){
                userText += input;
            }
        }
        else if (type == textbuttonTypes::newSafe){
            // Allow letters, numbers and spaces for the save name
            if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z') || (input >= '0' && input <= '9') || (input == ' ' && userText.size() != 0)){
                userText += input;
            }
        }
        text->setString(userText);
        text->setPosition(sf::Vector2f((res.x/2.0) - (text->getLocalBounds().size.x/2.0), text->getPosition().y));    

    }
}



void textButton::delLastInput(sf::Vector2u res){
    if (userText.size() != 0){
        userText.erase(userText.size() - 1, 1);
        text->setString(userText);
        text->setPosition(sf::Vector2f((res.x/2.0) - (text->getLocalBounds().size.x/2.0), text->getPosition().y));
    }   
}
