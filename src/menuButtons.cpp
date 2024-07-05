#include "menuButtons.hpp"

///////////////////////
//BUTTON CLASS(es)
///////////////////////



//pretending we define the parent class functions
button::button(short i, std::string tex, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  float pos[2], sf::Font &font){
    // Init variables
    layer = currentLayer;
    layer_next = followLayer;
    stringText = tex;
    idx = i;
    
    // Set visual properties
    text.setString(stringText);
    text.setFillColor(sf::Color::White);
    text.setFont(font);
    text.setCharacterSize(GAMEFONT_SIZE);
    text.setStyle(sf::Text::Style::Regular);

    position[0] = pos[0] - (text.getGlobalBounds().getSize().x/2.0);
    position[1] = pos[1] - (text.getGlobalBounds().getSize().y/2.0);
    text.setPosition(position[0], position[1]);

    // Control Variables
    focus = false;
};
void button::draw(sf::RenderWindow &window){
    window.draw(text);
};
void button::update(sf::Vector2f mousePos){
    // Check if the mouse is hovering over the button

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
};
button::~button(){};



// CLICK BUTTONS
ClickButton::ClickButton(short i, std::string text, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  float pos[2], sf::Font &font, clickbuttonTypes type)
: button(i, text, currentLayer, followLayer, pos, font){
    // Call parent constructor and set type
    this->type = type;
};
ClickButton::~ClickButton(void){};


void ClickButton::update(sf::Vector2f mousePos){
    button::update(mousePos);
}



textButton::textButton(short i, std::string tex, mainMenuLayerId currentLayer, mainMenuLayerId followLayer,  float pos[2], sf::Font &font, textbuttonTypes type)
: button(i, tex, currentLayer, followLayer, pos, font){
    this->type = type;
    ogPosition[0] = text.getPosition().x;
    ogPosition[1] = text.getPosition().y;

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
        if (text.getFillColor() != sf::Color::Green){
            text.setFillColor(sf::Color::Green);
        }
    }
}



void textButton::resetInput(void){
    text.setString(stringText);
    text.setPosition(ogPosition[0], ogPosition[1]);
    text.setFillColor(sf::Color::Red);
    userText = "";
    activeInput = false;
}



void textButton::addInput(sf::Uint32 input, sf::Vector2u res){
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
            if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z') || (input >= '0' && input <= '9') || input == ' '){
                userText += input;
            }
        }
        text.setString(userText);
        text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);    

    }
}



void textButton::delLastInput(sf::Vector2u res){
    if (userText.size() != 0){
        userText.erase(userText.size() - 1, 1);
        text.setString(userText);
        text.setPosition((res.x/2.0) - (text.getLocalBounds().width/2.0), text.getPosition().y);
    }   
}
