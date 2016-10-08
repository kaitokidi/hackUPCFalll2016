#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "Editor.hpp"
#include "Gaem.hpp"

int main(){

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "./gaem", sf::Style::Close);
    window.setFramerateLimit(60);
		GaemData* data;
		data = (GaemData*) malloc(sizeof(GaemData));
		memset(data, 0, sizeof(GaemData));

		loadLevel(data, "text");

		sf::Clock clock;
    while(window.isOpen()) {
				sf::Time time = clock.restart();
				float deltaTime = time.asSeconds();
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

				GaemLogic_updateGame(data, deltaTime, &window);

        window.clear(sf::Color(255,251,239));

        window.display();
    }
}
