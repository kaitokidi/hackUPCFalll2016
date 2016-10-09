#ifndef PORTADA_H
#define PORTADA_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Portada {

private:
    sf::Sprite s;
    sf::Texture t;
//    sf::Button b;
    bool open;

    sf::Texture playTexture;
    sf::Sprite play;
    sf::Texture rmrfTexture;
    sf::Sprite rmrf;
    sf::Texture creditzText;
    sf::Sprite creditz;


public:
    Portada();
    ~Portada();
    void display(sf::RenderWindow* window);
    bool display(sf::RenderWindow* window, std::string pathImage);
    void credits(sf::RenderWindow* window, std::string pathImage);

};

#endif // PORTADA_H
