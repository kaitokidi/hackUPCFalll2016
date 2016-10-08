#ifndef __GAEMLOGIC_HPP__
#define __GAEMLOGIC_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <string.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#define PAJARITO_RADIO 40
#define RAIO_SPEED 100

enum Pajarito {
  bug,
  feature,
  Pajarito_Qtty
};

#define MAX_PAJA 64
struct Pajaritos {
  int x[MAX_PAJA];
  int y[MAX_PAJA];
  int vx[MAX_PAJA];
  int vy[MAX_PAJA];
  Pajarito p[MAX_PAJA];
  bool active[MAX_PAJA];
  float timerms[MAX_PAJA];
  int raiorID[MAX_PAJA];
};

static int ID__Pajarito = -1;
int GaemData__GetNewIDPajarito(){
  assert(ID__Pajarito < MAX_PAJA);
  return ++ID__Pajarito;
}

void GaemData_ResetIDPajarito() {
  ID__Pajarito = -1;
}

#define MAX_RAIO 64
struct Raios {
  int x[MAX_RAIO];
  int y[MAX_RAIO];
  float timerms[MAX_RAIO];
  bool done[MAX_RAIO];
  int pajaritoID[MAX_RAIO];
};

static int ID__Raio = -1;
int GaemData__GetNewIDRaio(){
  assert(ID__Raio < MAX_PAJA);
  return ++ID__Raio;
}

void GaemData_ResetIDRaio() {
  ID__Raio = -1;
}

struct GaemData {
  Pajaritos pajaritos;
  Raios raios;
};

bool my_isint(std::string s)
{
  for (int i=0;i<int(s.size());i++)
    if (not (s[i]>='0' and s[i]<='9'))
      return false;
  return true;
}

int my_stoi(std::string s) {
  //haber estudiado si pasas un nullptr, un string vacio, o un rico NULL;
  int x=0;
  bool negativo = s[0] == '-';
  for (int i=negativo;i<int(s.size());i++)
    x=10*x+s[i]-'0';
  if (negativo) x *= -1;
  return x;
}


void loadLevel(GaemData* gd, std::string path) {
  GaemData_ResetIDPajarito();
  GaemData_ResetIDRaio();
  memset(gd,0,sizeof(GaemData));
  // pasar del fichero a vector de strings
  std::ifstream fci(path);
  if (not fci.is_open()) {
    std::cout<<"ERROR opening " << path << std::endl;
    exit(0);
  }
  std::vector<std::pair<int,std::string> > vs;
  {
    int numlinea=0;
    std::string linea;
    while (getline(fci,linea)) {
      numlinea++;
      {
        for (int i=0;i<int(linea.size())-1;i++) {
          if (linea[i]=='/' and linea[i+1]=='/') {
            linea=linea.substr(0,i);
            break;
          }
        }
      }
      std::istringstream ci(linea);
      {
        std::string s;
        while(ci>>s)
        vs.push_back(std::pair<int,std::string> (numlinea,s));
      }
    }
  }
  for (unsigned int i = 0; i < vs.size(); i += 5) {
    int nextPajarito = GaemData__GetNewIDPajarito();
    gd->pajaritos.p[nextPajarito] = Pajarito(my_stoi(vs[i].second));
    gd->pajaritos.x[nextPajarito]  = my_stoi(vs[i+1].second);
    gd->pajaritos.y[nextPajarito]  = my_stoi(vs[i+2].second);
    gd->pajaritos.vx[nextPajarito] = my_stoi(vs[i+3].second);
    gd->pajaritos.vy[nextPajarito] = my_stoi(vs[i+4].second);
  }
}

void UpdateRaio(GaemData* gd, int id, float dt) {
  Pajaritos* p = &gd->pajaritos;
  Raios* r = &gd->raios;
  if (r->done[id]) return;
  r->timerms[id] += dt;
  sf::Vector2i ini(r->x[id], r->y[id]);
  sf::Vector2i v(p->vx[r->pajaritoID[id]], p->vy[r->pajaritoID[id]]);
  float modul =  std::sqrt(v.x * v.x + v.y * v.y);
  sf::Vector2f vu = sf::Vector2f (v.x / modul, v.y / modul);
  sf::Vector2i dest = ini + sf::Vector2i(vu * r->timerms[id] * float(RAIO_SPEED));
  // std::cout << "Soy el rayo " << id << " y estoy en " << dest.x << " " << dest.y << " " << r->timerms[id] << " " << dt << std::endl;
  // Mirar si golpea con un pajarito
  for (unsigned int i = 0; i <= ID__Pajarito; ++i) {
    if (i == r->pajaritoID[id]) continue;
    sf::Vector2i pajaritoPos(p->x[i], p->y[i]);
    sf::Vector2i v = dest - pajaritoPos;
    float dist = std::sqrt(v.x * v.x + v.y * v.y);
    if (dist < PAJARITO_RADIO) {
      r->done[id] = true;
      if (p->active[i]) continue;
      p->active[i] = true;

      int idNewRaio = GaemData__GetNewIDRaio();

      r->x[idNewRaio] = p->x[i];
      r->y[idNewRaio] = p->y[i];
      r->pajaritoID[idNewRaio] = i;
      r->timerms[idNewRaio] = 0;
      r->done[idNewRaio] = false;
    }
  }
  // Mirar si golepa con un raio
  for (unsigned int i = 0; i <= ID__Raio; ++i) {
    if (i == id) continue;
    
  }

}

void GaemLogic_updateGame(GaemData* gd, float dt_milis, sf::RenderWindow* target) {
  sf::Vector2i mousePosition = sf::Mouse::getPosition(*target);
  bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
  if (mousePressed) {
    for (int i = 0; i <= int(ID__Pajarito); ++i) {
      if (gd->pajaritos.active[i]) continue;
      int diffx = mousePosition.x - gd->pajaritos.x[i];
      int diffy = mousePosition.y - gd->pajaritos.y[i];
      if (std::sqrt(diffx * diffx + diffy * diffy) < PAJARITO_RADIO) {
        std::cout << "Clicqued" << std::endl;
        gd->pajaritos.active[i] = true;

        int idNewRaio = GaemData__GetNewIDRaio();

        gd->raios.x[idNewRaio] = gd->pajaritos.x[i];
        gd->raios.y[idNewRaio] = gd->pajaritos.y[i];
        gd->raios.pajaritoID[idNewRaio] = i;
        gd->raios.timerms[idNewRaio] = 0;
        gd->raios.done[idNewRaio] = false;
      }
    }
  }

  int totalDone = 0;
  for (int i = 0; i <= int(ID__Raio); ++i) {
      UpdateRaio(gd, i, dt_milis);
      totalDone += gd->raios.done[i];
  }
  if (totalDone == ID__Pajarito) std::cout << "WIN" << std::endl;

}


#endif
