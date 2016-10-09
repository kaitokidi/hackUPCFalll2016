#include "Portada.hpp"
#include "Gaem.hpp"

Portada::Portada() {
  open = true;
}

Portada::~Portada(){}

void Portada::display(sf::RenderWindow* window){
  open = true;
  while(open){

    sf::Event event;
    while (window->pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
        window->close();
        exit(0);
        break;
        case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape) { window->close(); exit(0); }
        break;
        case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left) {
          open = false;
        }
        default:
        break;
      }
    }

    t.loadFromFile("res/Portada.png");
    s.setTexture(t);
    s.scale(window->getSize().y/s.getGlobalBounds().height,window->getSize().y/s.getGlobalBounds().height);
    s.setPosition(window->getSize().x/2 - s.getGlobalBounds().width/2, 0);
    window->draw(s);

    window->display();

  }

}

bool Portada::display(sf::RenderWindow* window, std::string pathImage){
  open = true;
  t.loadFromFile(pathImage);
  s = sf::Sprite();
  s.setTexture(t);
  s.scale(window->getSize().y/s.getGlobalBounds().height,window->getSize().y/s.getGlobalBounds().height);
  s.setPosition(window->getSize().x/2 - s.getGlobalBounds().width/2, 0);

  playTexture.loadFromFile("../res/pngs/play-794-447.png");
  play.setTexture(playTexture);
  play.setOrigin(sf::Vector2f(playTexture.getSize()/2u));
  play.setPosition(window->getSize().x/2, window->getSize().y/2);

  rmrfTexture.loadFromFile("../res/pngs/rmrf-730-681.png");
  rmrf.setTexture(rmrfTexture);
  rmrf.setOrigin(sf::Vector2f(rmrfTexture.getSize()/2u));
  rmrf.setPosition(window->getSize().x/2, window->getSize().y*3/4);
  
  creditzText.loadFromFile("../res/pngs/qm.png");
  creditz.setTexture(creditzText);
  creditz.setOrigin(sf::Vector2f(creditzText.getSize()/2u));
  creditz.setPosition(window->getSize().x - creditzText.getSize().x, window->getSize().y - creditzText.getSize().y*1.5);

  while(open){
    sf::Event event;
    while (window->pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:             window->close();                                               break;
        case sf::Event::KeyPressed:         if (event.key.code == sf::Keyboard::Escape) window->close();   break;
        case sf::Event::MouseButtonPressed:
        {
          if (event.mouseButton.button == sf::Mouse::Left) {
            if (play.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)))) {
              open = false;
            }
            else if (play.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)))) {
              GaemData__currentLvl = 1;
            }
            else if (creditz.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)))) {
              return true;
            }
          }
        } break;
        default:
        break;
      }
    }

    window->clear();
    window->draw(s);
    window->draw(play);
    window->draw(rmrf);
    window->draw(creditz);
    window->display();
  }
    return false;
}

void Portada::credits(sf::RenderWindow* window, std::string pathImage){
  open = true;
  
  t.loadFromFile(pathImage);
  s = sf::Sprite();
  s.setTexture(t);
  s.scale(window->getSize().y/s.getGlobalBounds().height,window->getSize().y/s.getGlobalBounds().height);
  s.setPosition(window->getSize().x/2 - s.getGlobalBounds().width/2, 0);

  creditzText.loadFromFile("../res/pngs/back.png");
  creditz.setTexture(creditzText);
  creditz.setOrigin(sf::Vector2f(creditzText.getSize()/2u));
  creditz.setPosition(window->getSize().x - creditzText.getSize().x, window->getSize().y - creditzText.getSize().y*1.5);

  while(open){
    sf::Event event;
    while (window->pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:             window->close();                                               break;
        case sf::Event::KeyPressed:         if (event.key.code == sf::Keyboard::Escape) window->close();   break;
        case sf::Event::MouseButtonPressed:
        {
          if (event.mouseButton.button == sf::Mouse::Left) {
            if (creditz.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)))) {
              return;
            }
          }
        } break;
        default:
        break;
      }
    }

    window->clear();
    window->draw(s);
    window->draw(creditz);
    window->display();
  }
}
