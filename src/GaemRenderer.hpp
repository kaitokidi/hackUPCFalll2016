#ifndef __GAEMRENDERER_HPP__
#define __GAEMRENDERER_HPP__

#include "Gaem.hpp"

// para shaderino
sf::Shader blurrerino;
sf::Shader glowerino;

sf::RenderTexture raios;
sf::Texture pajaritoTextrure;

void GaemRenderer__Render(GaemData* data, sf::RenderWindow* window) {
  sf::Sprite pajarito;
  pajarito.setTexture(pajaritoTextrure, true);
  pajarito.setOrigin(sf::Vector2f(pajaritoTextrure.getSize()/2u));
  // Legasida
  for (int i = 0; i <= ID__Pajarito; ++i) {
    // sf::CircleShape shape;
    sf::Vector2f circlePos(data->pajaritos.x[i], data->pajaritos.y[i]);
    pajarito.setPosition(circlePos);
    sf::Color color = (data->pajaritos.toqueteable[i] ? sf::Color::Blue : sf::Color::Red);
    pajarito.setColor(color);
    for (int j = 0; j <= data->pajaritos.p[i]; ++j) {
      float deltaX = data->pajaritos.vx[i][j];
      float deltaY = data->pajaritos.vy[i][j];
      float angle = atan2(deltaY, deltaX) * 180 / M_PI;
      pajarito.setRotation(angle + 90);
      window->draw(pajarito, &blurrerino);
    }
  }

  raios.clear(sf::Color::Transparent);

  for (int i = 0; i <= ID__Raio; ++i) {
    sf::Vector2i ini(data->raios.x[i], data->raios.y[i]);
    sf::Vector2i dest = ini + sf::Vector2i(getIncremento(data, i, data->raios.timerms[i]));
    sf::Vertex line[] = {
      sf::Vertex(sf::Vector2f(ini)),
      sf::Vertex(sf::Vector2f(dest))
    };
    line[0].color = sf::Color::Yellow;
    line[1].color = sf::Color::Yellow;

    raios.draw(line, 2, sf::Lines);
  }
  raios.display();

  sf::Sprite raiosConjuntos;
  raiosConjuntos.setTexture(raios.getTexture());

  window->draw(raiosConjuntos,&glowerino);
  // new Shit

}


#endif
