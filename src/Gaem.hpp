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
#include <SFML/Audio.hpp>

#define PAJARITO_RADIO 40
#define RAIO_SPEED 300

extern int GaemData__currentLvl;

enum Pajarito {
  simple = 0,
  doble = 1,
  triple = 2,
  feature,
  Pajarito_Qtty
};

float dot(sf::Vector2f v1, sf::Vector2f v2);

#define MAX_PAJA 128
struct Pajaritos {
  int x[MAX_PAJA];
  int y[MAX_PAJA];
  int vx[MAX_PAJA][3];
  int vy[MAX_PAJA][3];
  Pajarito p[MAX_PAJA];
  bool active[MAX_PAJA];
  float timerms[MAX_PAJA];
  int raiorID[MAX_PAJA];
  bool toqueteable[MAX_PAJA];
};

extern int ID__Pajarito;
int GaemData__GetNewIDPajarito();

void GaemData_ResetIDPajarito();

#define MAX_RAIO 64
struct Raios {
  int x[MAX_RAIO];
  int y[MAX_RAIO];
  float timerms[MAX_RAIO];
  bool done[MAX_RAIO];
  int pajaritoID[MAX_RAIO];
  int nRaio[MAX_RAIO];
};

extern int ID__Raio;
int GaemData__GetNewIDRaio();

void GaemData_ResetIDRaio();

#define FADE_TIMER 1.f

struct GaemData {
  Pajaritos pajaritos;
  Raios raios;
  bool clicked;
  float fadein;
};


#define MAX_MUSIC 3
struct MusicStage {
  sf::Music music[MAX_MUSIC];
  int vol;
};

extern MusicStage GaemData__music;


bool my_isint(std::string s);

int my_stoi(std::string s);

void loadLevel(GaemData* gd,int nLvl);

void GaemData__RestartLvl(GaemData* data);

sf::Vector2f getIncremento(GaemData* data, int id, float dt);

void UpdateRaio(GaemData* gd, int id, float dt);

void GaemLogic_updateGame(GaemData* gd, float dt_milis, sf::RenderWindow* target);


#endif
