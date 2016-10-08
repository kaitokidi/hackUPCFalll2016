#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

class Editor {
  
public:

    Editor(sf::RenderWindow* pwindow);

    void run();
    
private:
    
    bool readyForSecondClick;
    bool mousePressed; 
    bool textActive;
    bool visibility;
    
    std::string state;
    
    int brushType;
    int uselessVariable;

    sf::Text text;
    sf::Text textType;
    sf::Text textVisibility;
    
    sf::Font font;
    sf::RenderWindow* window;

    std::vector<int> types;
    std::vector<sf::CircleShape> circles;
    std::vector<sf::CircleShape> littleCircle;
};