#include "Editor.hpp"
#include <cmath>

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
    textType.setColor(sf::Color::Green);
    textType.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textType.setPosition(0,30);
    textType.setString("0");
    readyForSecondClick = false;
    
    state = "iddle";
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
                                circles.pop_back();
                                littleCircle.pop_back();
                            }
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && event.key.code == sf::Keyboard::T) {
                        ++brushType;
                        if(brushType >= 10) brushType = 0;
                        textType.setString(std::to_string(brushType));
                        
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && event.key.code == sf::Keyboard::S) {
                        std::ofstream aux("../lvls/"+text.getString());
                        if(aux.is_open()){
                            size_t littleCircleindex = 0;
                            for(size_t i = 0; i < circles.size() && littleCircleindex < littleCircle.size(); ++i){
                                
                                auto c = circles[i];
                                auto lc = littleCircle[littleCircleindex];
                                int type = types[i];
                                if(type == 0) aux << type << " " << c.getPosition().x << " " << c.getPosition().y << " " << std::floor(lc.getPosition().x - c.getPosition().x) << " " << std::floor(lc.getPosition().y - c.getPosition().y) << '\n';
                                if(type == 1) {
                                    ++littleCircleindex;
                                    auto lc2 = littleCircle[littleCircleindex];
                                    aux << type << " " << c.getPosition().x << " " << c.getPosition().y << " " << std::floor(lc.getPosition().x - c.getPosition().x) << " " << std::floor(lc.getPosition().y - c.getPosition().y) << " " << std::floor(lc2.getPosition().x - c.getPosition().x) << " " << std::floor(lc2.getPosition().y - c.getPosition().y) << '\n';
                                }
                                
                                littleCircleindex++;
                            }
                            aux.close();
                        }
                    }
                    case sf::Event::TextEntered:
                        // Handle ASCII characters only
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
                        std::cout << "newitera" << std::endl;
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
                                    std::cout << "butpress ready for secondclick" << std::endl;
                }
            }
        }
        else { 
            mousePressed = false;
            if( (brushType == 0 || brushType == 1) && state == "mouseClicked"){
                std::cout << "else mousepressed" << std::endl;
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
        std::cout << "drawing" << std::endl;
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
        sf::CircleShape auxCircle(40, 79);
        auxCircle.setOrigin(40,40);
        auxCircle.setFillColor(sf::Color(0,0,0,100));
        auxCircle.setPosition(mouse_x, mouse_y);
        window->draw(auxCircle);
        
        window->display();
                std::cout << "--" << std::endl;
    }
    
}