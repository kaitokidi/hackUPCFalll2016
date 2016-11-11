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
   /*     sf::ContextSettings settings;
    settings.antialiasingLevel = 8;*/
    
  sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "./gaem");
  window.setFramerateLimit(60);

  Portada p;
  bool credits = p.display(&window, "../res/pngs/backgroundMENU.png");
  while (credits) {
    p.credits(&window, "../res/pngs/backgroundcreditz.png");
    credits = p.display(&window, "../res/pngs/backgroundMENU.png");
  }
  
  GaemData* data;
  data = (GaemData*) malloc(sizeof(GaemData));
  memset(data, 0, sizeof(GaemData));

  // Graphics stuff
  pajaritoTextrure.loadFromFile("../res/pngs/pinchito.png");
  blurrerino.loadFromFile("../res/shaders/blurrerino.frag", sf::Shader::Type::Fragment);
  glowerino.loadFromFile("../res/shaders/glowerino.frag", sf::Shader::Type::Fragment);
  glowerino.setParameter("textureSize", sf::Vector2f(window.getSize()));
  glowerino.setParameter("size", 2.f);

  GaemData__music.music[1].openFromFile("../res/bajo2.wav");
  GaemData__music.music[2].openFromFile("../res/melodia2.wav");
  GaemData__music.music[0].openFromFile("../res/porencimadelbajo2.wav");

  for (int i = 0; i < 3; ++i) {
    GaemData__music.music[i].setLoop(true);
    GaemData__music.music[i].setVolume((!i ? 50 : 0));
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

    window.clear(sf::Color(255,251,239));
    GaemRenderer__Render(data, &window);
    window.display();
  }
}
