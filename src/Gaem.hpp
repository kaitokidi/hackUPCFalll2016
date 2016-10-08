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

#define PAJARITO_RADIO 80
#define RAIO_SPEED 50

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
  int timerms[MAX_PAJA];
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
  int timerms[MAX_RAIO];
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
  int x=0;
  for (int i=0;i<int(s.size());i++)
    x=10*x+s[i]-'0';
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
  for (unsigned int i = 0; i < vs.size(); ++i) {
    int nextPajarito = GaemData__GetNewIDPajarito();
    gd->pajaritos.x[nextPajarito]  = my_stoi(vs[i++].second);
    gd->pajaritos.y[nextPajarito]  = my_stoi(vs[i++].second);
    gd->pajaritos.vx[nextPajarito] = my_stoi(vs[i++].second);
    gd->pajaritos.vy[nextPajarito] = my_stoi(vs[i++].second);
    gd->pajaritos.p[nextPajarito] = Pajarito(my_stoi(vs[i++].second));
  }
}

void UpdateRaio(GaemData* gd, int id, int dt_milis) {
  Pajaritos* p = &gd->pajaritos;
  Raios* r = &gd->raios;
  if (r->done[id]) return;
  r->timerms[id] += dt_milis;
  sf::Vector2i ini(r->x[id], r->y[id]);
  sf::Vector2i dest = ini * dt_milis * RAIO_SPEED;
  dest.x /= 1000;
  dest.y /= 1000;
  bool onGoal = false;
  // Mirar si golpea con un pajarito
  for (unsigned int i = 0; i <= ID__Raio || onGoal; ++i) {
    if (i == r->pajaritoID[id]) continue;
    
  }
  // Mirar si golepa con un raio

}

void GaemLogic_updateGame(GaemData* gd, int dt_milis) {
  for (unsigned int i = 0; i < MAX_RAIO; ++i) {
      UpdateRaio(gd, i, dt_milis);
  }
}


#endif
