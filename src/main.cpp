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

		loadLevel(data, "../lvls/level4");

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

        window.clear(sf::Color::Black);

        for (int i = 0; i <= ID__Pajarito; ++i) {
          sf::CircleShape shape;
          sf::Vector2f circlePos(data->pajaritos.x[i], data->pajaritos.y[i]);
          shape.setPosition(circlePos);
          shape.setRadius(PAJARITO_RADIO);
          shape.setFillColor(sf::Color::Blue);
          shape.setOrigin(PAJARITO_RADIO,PAJARITO_RADIO);
          window.draw(shape);
          for (int j = 0; j <= data->pajaritos.p[i]; ++j) {
            sf::CircleShape triangle(10, 3);
            triangle.setOrigin(10, 10);
            float deltaX = data->pajaritos.vx[i][j];
            float deltaY = data->pajaritos.vy[i][j];
            float angle = atan2(deltaY, deltaX) * 180 / M_PI;
            triangle.setRotation(angle + 90);
            float modul =  std::sqrt(deltaX * deltaX + deltaY * deltaY);
            sf::Vector2f vu = sf::Vector2f (deltaX / modul, deltaY / modul);
            sf::Vector2f offset(vu * float(PAJARITO_RADIO));
            triangle.setPosition(circlePos + offset);
            window.draw(triangle);
          }
        }

        for (int i = 0; i <= ID__Raio; ++i) {
          sf::Vector2i ini(data->raios.x[i], data->raios.y[i]);
          sf::Vector2i v(data->pajaritos.vx[data->raios.pajaritoID[i]][data->raios.nRaio[i]], data->pajaritos.vy[data->raios.pajaritoID[i]][data->raios.nRaio[i]]);
          float modul =  std::sqrt(v.x * v.x + v.y * v.y);
          sf::Vector2f vu = sf::Vector2f (v.x / modul, v.y / modul);
          sf::Vector2i dest = ini + sf::Vector2i(vu * data->raios.timerms[i] * float(RAIO_SPEED));
          sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(ini)),
            sf::Vertex(sf::Vector2f(dest))
          };
          window.draw(line, 2, sf::Lines);
        }


        window.display();
    }
}
