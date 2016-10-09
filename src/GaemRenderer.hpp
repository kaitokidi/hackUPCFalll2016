#ifndef __GAEMRENDERER_HPP__
#define __GAEMRENDERER_HPP__

#include "Gaem.hpp"

// para shaderino
sf::Shader blurrerino;
sf::Shader glowerino;

sf::RenderTexture raios;
sf::Texture pajaritoTextrure;

float getAngle(const sf::Vector2f &orig,const sf::Vector2f &des) {
    return std::atan2(des.y - orig.y, des.x - orig.x)*180/(M_PI);
}

float getModule(const sf::Vector2f &orig, const sf::Vector2f &des) {
    return std::sqrt(std::pow(std::abs(des.x-orig.x), 2) + std::pow(std::abs(des.y-orig.y), 2));
}

void GaemRenderer__Render(GaemData* data, sf::RenderWindow* window)
{
static sf::Shader* txader = NULL;
    static sf::Clock clk;
    if (txader == NULL)
    {
        txader = new sf::Shader();
        if(! blurrerino.loadFromFile("../res/shaders/blurrerino.frag", sf::Shader::Type::Fragment))std::cout << "failed loading blurrerino" << std::endl;
        txader->loadFromFile("../res/shaders/shaderino.frag", sf::Shader::Type::Fragment);
        clk.restart();
    }
    txader->setParameter("time", clk.getElapsedTime().asSeconds());

  sf::Sprite pajarito;
  pajarito.setTexture(pajaritoTextrure, true);
  pajarito.setOrigin(sf::Vector2f(pajaritoTextrure.getSize()/2u));
  // Legasida

  for (int i = 0; i <= ID__Pajarito; ++i)
  {
    // sf::CircleShape shape;
    sf::Vector2f circlePos(data->pajaritos.x[i], data->pajaritos.y[i]);
    pajarito.setPosition(circlePos);
    sf::Color color = (data->pajaritos.toqueteable[i] ? sf::Color::Blue : sf::Color::Red);
    pajarito.setColor(color);
    for (int j = 0; j <= data->pajaritos.p[i]; ++j)
    {
      float deltaX = data->pajaritos.vx[i][j];
      float deltaY = data->pajaritos.vy[i][j];
      float angle = atan2(deltaY, deltaX) * 180 / M_PI;
      pajarito.setRotation(angle + 90);
        txader->setParameter("pos", circlePos);
        txader->setParameter("istouch", data->pajaritos.toqueteable[i]);
        txader->setParameter("fade", data->fade);
      window->draw(pajarito, txader);
    }
  }

    sf::Vector2f pos(
            float(data->pajaritos.x[0]) / window->getSize().x,
            float(data->pajaritos.y[0]) / window->getSize().y
            );
    
//    for (int i = 0; i < ID__Pajarito; ++i)
    {
   //     txader->setParameter("pos", pos);
    }
/*
    sf::Sprite spr;
  window->draw(spr, txader);
*/

  raios.clear(sf::Color::Transparent);

  for (int i = 0; i <= ID__Raio; ++i)
  {
    sf::Vector2f ini(data->raios.x[i], data->raios.y[i]);
    sf::Vector2f dest = ini + sf::Vector2f(getIncremento(data, i, data->raios.timerms[i]));
    sf::Vertex line[] =
    {
      sf::Vertex(sf::Vector2f(ini)),
      sf::Vertex(sf::Vector2f(dest))
    };
    line[0].color = sf::Color::Yellow;
    line[1].color = sf::Color::Yellow;

    //raios.draw(line, 2, sf::Lines);
   

    float rectangleWidth = 6;
    float module = getModule(ini,dest);
    sf::RectangleShape rline(sf::Vector2f(module, rectangleWidth));
    rline.setOrigin(0,rectangleWidth/2);
    float angle = getAngle(ini, dest);
    rline.setPosition(ini.x,ini.y);
    rline.rotate(angle);
    rline.setFillColor(sf::Color(20,20,20));
    
    raios.draw(rline);
  }
  ///
  raios.display();

  sf::Sprite raiosConjuntos;
  raiosConjuntos.setTexture(raios.getTexture());

  window->draw(raiosConjuntos, &blurrerino);


  // new Shit

  /*spoiler*/
}


#endif
