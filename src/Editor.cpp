#include "Editor.hpp"

Editor::Editor(sf::RenderWindow* pwindow){
    window = pwindow;
    mousePressed = false;
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
                    if (event.key.code == sf::Keyboard::S) {
                        //save
                    }
                default:
                    break;
            }
        }   

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(!mousePressed){
                mousePressed = true;
                float mouse_x, mouse_y;
                mouse_x = sf::Mouse::getPosition(*window).x; 
                mouse_y = sf::Mouse::getPosition(*window).y;
                
                sf::CircleShape auxCircle (50, 50);
                auxCircle.setCenter(25,25);
                auxCircle.setColor(sf::Color(0,0,0));
                auxCircle.setPosition(mouse_x, mouse_y);
                circles.push_back(auxCircle);
            }
        }
        else { 
            mousePressed = false;
        }
        
        window->clear(sf::Color(255,251,239));

        for(size_t i = 0; i < circles.size(); ++i){
            window->draw(circles[i]);
        }
        
        window->display();
    }
    
}