#ifndef __GAEMLOGIC_HPP__
#define __GAEMLOGIC_HPP__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#define MAX_PAJA 64

struct GaemData {
  int x[MAX_PAJA];
  int y[MAX_PAJA];
  int vx[MAX_PAJA];
  int vy[MAX_PAJA];
};

void loadLevel(GaemData* gd, std::string path) {
  
}

void GaemLogic_updateGame(GaemData* gd, int dt_milis) {



}


#endif
