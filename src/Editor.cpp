#include "Editor.hpp"
#include <cmath>

/*
 Readme how to use it
 
 from the game press E to go to the editor.
 
 once in the editor
     
 if you click C + W
     Writtemode command
     you enter in writte mode, you can now writte the number of the level tah you want it saved into.
     press C + W again when you are done to exit this mode.
     You can see the current name in red in the top left corner.
     if you exit and enter writte mode the name will be reseted to level.
     
 if you click C + T
     TypeChange command
     you will change the type of the brush (you will create items from the next type) 
     You can see the type displayed in green in the top left corner.
     
 if you click C + S
     Save command
     you will save the file with the name written on writte mode. 
     If no name is given it will be saved in a file named level.
     This levels will be written on ../lvls/nameOfTheLevel
     
 if you click C + U 
     Undo command
     the last element added will be deleted.
 
 if you click C + V
     toggle if the next item placed can be touched or not.
     
 if you are in type == 0
     you will draw basic pins.
     click the left mouse button on the place you want it to be placed. 
     after releasing it, move the mouse in the direction you want the pin to face.
     
 if you are in type == 1
     you will draw double pins.
     Same as the basic pins (type == 0).
     Then click on the direction you want the second spike.
     
 */

Editor::Editor(sf::RenderWindow* pwindow){
    window = pwindow;
    mousePressed = false;
    if (!font.loadFromFile("../res/font.otf")) {
        std::cout << "font not loaded" << std::endl;
    }
    textActive = false;
    text.setString("level");
    window->setKeyRepeatEnabled(false);
    brushType = 0;
    
    textType.setFont(font);
    textType.setCharacterSize(24); 
    textType.setColor(sf::Color(0,255,0));
    textType.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textType.setPosition(0,30);
    textType.setString("0");

    textVisibility.setFont(font);
    textVisibility.setCharacterSize(24); 
    textVisibility.setColor(sf::Color(0,0,255));
    textVisibility.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textVisibility.setPosition(0,60);    
    textVisibility.setString("1");
    
    readyForSecondClick = false;
    
    state = "iddle";
    
    visibility = true;
}
    
void Editor::run(){
    
    while(window->isOpen()){
    

        sf::Event event;
        while(window->pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    window->close();
                    exit(0);
                case  sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        return;
                    }
                    if (event.key.code == sf::Keyboard::W){
                        text.setFont(font);
                        text.setCharacterSize(24); 
                        text.setColor(sf::Color::Red);
                        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
                        textActive = !textActive;
                        if(textActive) text.setString("level");
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && event.key.code == sf::Keyboard::U) {
                        if(!mousePressed){
                            if(circles.size() > 0){
                                int type = types[types.size()-1];
                                types.pop_back();
                                circles.pop_back();
                                littleCircle.pop_back();
                                if(type == 1) littleCircle.pop_back();
                            }
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && event.key.code == sf::Keyboard::T) {
                        ++brushType;
                        if(brushType >= 1) brushType = 0;
                        textType.setString(std::to_string(brushType));
                        
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && event.key.code == sf::Keyboard::V) {
                        visibility = !visibility;
                        textVisibility.setString(std::to_string(visibility));
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && event.key.code == sf::Keyboard::S) {
                        
                        std::ofstream aux("../lvls/"+text.getString());
                        text.setString("Saved as -> " + text.getString() );
                        if(aux.is_open()){
                            size_t littleCircleindex = 0;
                            for(size_t i = 0; i < circles.size() && littleCircleindex < littleCircle.size(); ++i){
                                
                                auto c = circles[i];
                                auto lc = littleCircle[littleCircleindex];
                                int type = types[i];
                                int vis = visibilities[i];
                                if(type == 0) aux << type << " " << c.getPosition().x << " " << c.getPosition().y << " " << std::floor(lc.getPosition().x - c.getPosition().x) << " " << std::floor(lc.getPosition().y - c.getPosition().y) << " " << vis << '\n';
                                if(type == 1) {
                                    ++littleCircleindex;
                                    auto lc2 = littleCircle[littleCircleindex];
                                    aux << type << " " << c.getPosition().x << " " << c.getPosition().y << " " << std::floor(lc.getPosition().x - c.getPosition().x) << " " << std::floor(lc.getPosition().y - c.getPosition().y) << " " << std::floor(lc2.getPosition().x - c.getPosition().x) << " " << std::floor(lc2.getPosition().y - c.getPosition().y) << " " << vis << '\n';
                                }
                                
                                littleCircleindex++;
                            }
                            aux.close();
                        }
                    }
                    case sf::Event::TextEntered:
                        if (textActive && event.text.unicode < 58 && event.text.unicode > 47 && event.text.unicode != 119 && event.text.unicode != 87)
                        {
                            std::string str = text.getString();
                            str += static_cast<char>(event.text.unicode);
                            text.setString(str);
                        }
                    break;
                default:
                    break;
            }
        }   
        
        float mouse_x, mouse_y;
        mouse_x = sf::Mouse::getPosition(*window).x; 
        mouse_y = sf::Mouse::getPosition(*window).y;

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(!mousePressed){
                mousePressed = true;
                if(state == "iddle"){ //82 de mida 
                    
                    sf::CircleShape auxCircle(40, 79);
                    auxCircle.setOrigin(40,40);
                    auxCircle.setFillColor(sf::Color(0,0,0));
                    auxCircle.setPosition(mouse_x, mouse_y);
                    circles.push_back(auxCircle);
                    types.push_back(brushType);
                    visibilities.push_back(visibility);
                    
                    state = "mouseClicked";
                }
                if(state == "waitingSecondClick"){
                    sf::CircleShape auxCircle(2, 20);
                    auxCircle.setOrigin(2,2);
                    auxCircle.setFillColor(sf::Color(255,0,0));
                    auto p1 = sf::Vector2f(circles[circles.size()-1].getPosition());
                    auto p2 = sf::Vector2f(mouse_x, mouse_y);
                    auto vector = sf::Vector2f(p2.x-p1.x, p2.y-p1.y);
                    float module = std::sqrt(vector.x*vector.x + vector.y*vector.y);
                    sf::Vector2f unitVector(vector.x/module, vector.y/module);
                    sf::Vector2f finalPoint(unitVector.x*82, unitVector.y*82);
                    auxCircle.setPosition(p1.x+finalPoint.x, p1.y+finalPoint.y);
                    littleCircle.push_back(auxCircle);
                    
                    state = "iddle";
                }
            }
        }
        else { 
            mousePressed = false;
            if( (brushType == 0 || brushType == 1) && state == "mouseClicked"){
                sf::CircleShape auxCircle(2, 20);
                auxCircle.setOrigin(2,2);
                auxCircle.setFillColor(sf::Color(255,0,0));
                auto p1 = sf::Vector2f(circles[circles.size()-1].getPosition());
                auto p2 = sf::Vector2f(mouse_x, mouse_y);
                auto vector = sf::Vector2f(p2.x-p1.x, p2.y-p1.y);
                float module = std::sqrt(vector.x*vector.x + vector.y*vector.y);
                sf::Vector2f unitVector(vector.x/module, vector.y/module);
                sf::Vector2f finalPoint(unitVector.x*82, unitVector.y*82);
                auxCircle.setPosition(p1.x+finalPoint.x, p1.y+finalPoint.y);
                littleCircle.push_back(auxCircle);
                
                state = "iddle";
                if(brushType == 1) state = "waitingSecondClick";
            } 
            
        }
        
        window->clear(sf::Color(255,251,239));
        int littleCircleindex = 0;
        //std::cout << "drawing" << std::endl;
        
        if(littleCircle.size() <= 0){
            for(size_t i = 0; i < circles.size(); ++i) window->draw(circles[i]);
        }
        
        for(size_t i = 0; i < circles.size() && littleCircleindex < littleCircle.size(); ++i){
            
            window->draw(circles[i]);
       
            window->draw(littleCircle[littleCircleindex]);
            sf::Vertex line[] =
                {                    
                      sf::Vertex(circles[i].getPosition()),
                      sf::Vertex(littleCircle[littleCircleindex].getPosition())
                };
            window->draw(line, 2, sf::Lines);
            
            if(types[i] == 1){
                ++littleCircleindex;
                if(littleCircleindex < littleCircle.size()){
                    window->draw(littleCircle[littleCircleindex]);
                    sf::Vertex line[] =
                        {                    
                            sf::Vertex(circles[i].getPosition()),
                            sf::Vertex(littleCircle[littleCircleindex].getPosition())
                        };
                    window->draw(line, 2, sf::Lines);           
                }
            }
            
            ++littleCircleindex;
        }

        
        window->draw(text);
        window->draw(textType);
        window->draw(textVisibility);
        sf::CircleShape auxCircle(40, 79);
        auxCircle.setOrigin(40,40);
        auxCircle.setFillColor(sf::Color(0,0,0,100));
        auxCircle.setPosition(mouse_x, mouse_y);
        window->draw(auxCircle);
        
        window->display();
    }
    
}