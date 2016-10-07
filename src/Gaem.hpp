#ifndef __GAEMLOGIC_HPP__
#define __GAEMLOGIC_HPP__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#define MAX_PAJA 64

enum Pajarito {
  bug,
  feature,
  Pajarito_Qtty
};

struct GaemData {
  int x[MAX_PAJA];
  int y[MAX_PAJA];
  int vx[MAX_PAJA];
  int vy[MAX_PAJA];
};

void loadLevel(GaemData* gd, std::string path) {
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
    int x, y, vx, vy;
    Pajarito pajarito;
  }
}

void GaemLogic_updateGame(GaemData* gd, int dt_milis) {



}


#endif
