#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include "Editor.hpp"

int main(){

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "./gaem", sf::Style::Close);

    while(window.isOpen()){

        sf::Event event;
        while(window.pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    if (event.key.code == sf::Keyboard::E) {
                        Editor editor(&window);
                        editor.run();
                    }
                    break;
                default:
                    break;
            }
        }

        window.clear(sf::Color(255,251,239));

        window.display();
    }
}
