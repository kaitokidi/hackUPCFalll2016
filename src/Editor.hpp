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
    
    bool mousePressed; 

    int uselessVariable;
    
    sf::RenderWindow* window;

    std::vector<sf::CircleShape> circles;
    std::vector<sf::CircleShape> littleCircle;
};