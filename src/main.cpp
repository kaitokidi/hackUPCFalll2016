#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "Editor.hpp"
#include "Gaem.hpp"
#include "Portada.hpp"
#include "GaemRenderer.hpp"

int main(){
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "./gaem", sf::Style::Close);
  window.setFramerateLimit(60);

  Portada p;
  p.display(&window, "../res/pngs/backgroundMENU.png");

  GaemData* data;
  data = (GaemData*) malloc(sizeof(GaemData));
  memset(data, 0, sizeof(GaemData));

  // Graphics stuff
  pajaritoTextrure.loadFromFile("../res/pngs/pinchito.png");
  blurrerino.loadFromFile("../res/shaders/blurrerino.frag", sf::Shader::Type::Fragment);
  glowerino.loadFromFile("../res/shaders/glowerino.frag", sf::Shader::Type::Fragment);
  glowerino.setParameter("textureSize", sf::Vector2f(window.getSize()));
  glowerino.setParameter("size", 2.f);

  GaemData__music.music[0].openFromFile("../res/bajo1.wav");
  GaemData__music.music[1].openFromFile("../res/melodia1.wav");
  GaemData__music.music[2].openFromFile("../res/agudos1.wav");

  for (int i = 0; i < 3; ++i) {
    GaemData__music.music[i].setLoop(true);
    GaemData__music.music[i].setVolume(0);
    GaemData__music.music[i].play();
  }

  raios.create(window.getSize().x, window.getSize().y);

  loadLevel(data, GaemData__currentLvl);

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
        if (event.key.code == sf::Keyboard::N) {
          loadLevel(data, ++GaemData__currentLvl);
        }
        break;
        default:
        break;
      }
    }

    GaemLogic_updateGame(data, deltaTime, &window);

    window.clear(sf::Color::Black);
    GaemRenderer__Render(data, &window);
    window.display();
  }
}
