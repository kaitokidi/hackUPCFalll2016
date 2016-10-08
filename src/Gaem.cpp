#include "Gaem.hpp"
#include <dirent.h>

int GaemData__currentLvl = 1;

MusicStage GaemData__music;

float dot(sf::Vector2f v1, sf::Vector2f v2) {
  return v1.x * v2.y - v1.y * v2.x;
}

int ID__Pajarito = -1;
int GaemData__GetNewIDPajarito(){
  assert(ID__Pajarito < MAX_PAJA);
  return ++ID__Pajarito;
}

void GaemData_ResetIDPajarito() {
  ID__Pajarito = -1;
}

int ID__Raio = -1;
int GaemData__GetNewIDRaio(){
  assert(ID__Raio < MAX_PAJA);
  return ++ID__Raio;
}

void GaemData_ResetIDRaio() {
  ID__Raio = -1;
}

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

void loadLevel(GaemData* gd, int nLvl) {
  GaemData_ResetIDPajarito();
  GaemData_ResetIDRaio();
  gd->fade = 0;
  gd->restarting = false;

  DIR *dir;
  struct dirent *ent;
  std::string path;
  if ((dir = opendir ("../lvls/")) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      path = ent->d_name;
      if (path.find("level") != std::string::npos) {
        path.erase(0, 5);
        if (my_stoi(path) == nLvl) break;
      }
    }
    closedir (dir);
  } else {
    perror ("Could not open directory");
    exit(EXIT_FAILURE);
  }
  std::cout << path << std::endl;
  memset(gd,0,sizeof(GaemData));
  // pasar del fichero a vector de strings
  std::ifstream fci("../lvls/level"+path);
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
  for (unsigned int i = 0; i < vs.size(); i += 6) {
    int nextPajarito = GaemData__GetNewIDPajarito();
    gd->pajaritos.p[nextPajarito] = Pajarito(my_stoi(vs[i].second));
    gd->pajaritos.x[nextPajarito]  = my_stoi(vs[i+1].second);
    gd->pajaritos.y[nextPajarito]  = my_stoi(vs[i+2].second);
    gd->pajaritos.vx[nextPajarito][0] = my_stoi(vs[i+3].second);
    gd->pajaritos.vy[nextPajarito][0] = my_stoi(vs[i+4].second);
    if (my_stoi(vs[i].second)>0) {
      gd->pajaritos.vx[nextPajarito][1] = my_stoi(vs[i+5].second);
      gd->pajaritos.vy[nextPajarito][1] = my_stoi(vs[i+6].second);
      if (my_stoi(vs[i].second)>1) {
        gd->pajaritos.vx[nextPajarito][2] = my_stoi(vs[i+7].second);
        gd->pajaritos.vy[nextPajarito][2] = my_stoi(vs[i+8].second);
        i += 2;
      }
      i += 2;
    }
    gd->pajaritos.toqueteable[nextPajarito] = my_stoi(vs[i+5].second);
  }
}

void GaemData__RestartLvl(GaemData* data) {
  GaemData_ResetIDRaio();
  for (int i = 0; i <= ID__Pajarito; ++i) {
    data->pajaritos.active[i] = false;
  }
  data->clicked = false;
}

sf::Vector2f getIncremento(GaemData* data, int id, float dt) {
  Pajaritos* p = &data->pajaritos;
  Raios* r = &data->raios;
  sf::Vector2i v(p->vx[r->pajaritoID[id]][r->nRaio[id]], p->vy[r->pajaritoID[id]][r->nRaio[id]]);
  float modul =  std::sqrt(v.x * v.x + v.y * v.y);
  // return sf::Vector2f (v.x / modul, v.y / modul) * r->timerms[id] * float(RAIO_SPEED) + sf::Vector2f (v.x / modul, v.y / modul) * float(RAIO_SPEED)/2.f * float(sin(r->timerms[id]*10))/2.f;
  return sf::Vector2f (v.x / modul, v.y / modul) * r->timerms[id] * float(RAIO_SPEED);
}

void UpdateRaio(GaemData* gd, int id, float dt) {
  Pajaritos* p = &gd->pajaritos;
  Raios* r = &gd->raios;
  if (r->done[id]) return;
  r->timerms[id] += dt;
  sf::Vector2i ini(r->x[id], r->y[id]);
  sf::Vector2i dest = ini + sf::Vector2i(getIncremento(gd, id, dt));
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

      for (int j = 0; j <= p->p[i]; ++j) {
        int idNewRaio = GaemData__GetNewIDRaio();
        r->x[idNewRaio] = p->x[i];
        r->y[idNewRaio] = p->y[i];
        r->pajaritoID[idNewRaio] = i;
        r->timerms[idNewRaio] = 0;
        r->done[idNewRaio] = false;
        r->nRaio[idNewRaio] = j;
      }
    }
  }
  // Mirar si golepa con un raio
  sf::Vector2f q(ini);
  sf::Vector2f s(getIncremento(gd, id, dt));
  for (unsigned int i = 0; i <= ID__Raio; ++i) {
    if (r->pajaritoID[i] == r->pajaritoID[id]) continue;
    sf::Vector2i v(p->vx[r->pajaritoID[i]][r->nRaio[i]], p->vy[r->pajaritoID[i]][r->nRaio[i]]);
    sf::Vector2f p(r->x[i], r->y[i]);

    sf::Vector2f r(getIncremento(gd, i, dt));

    float t = dot((q - p),s)/dot(r,s);
    float u = dot((q - p),r)/dot(r,s);

    // Colisionan los segmentos
    if (dot(r,s) != 0 && t >= 0 && t <= 1 && u >= 0 && u <= 1) {
      gd->restarting = true;
      gd->fade = 1;
      return;
    }
  }
}

void tendVolumenTo(sf::Music& m, int tend, float deltaTime) {
  if (m.getVolume() == tend) return;
  float diff = (tend - m.getVolume())  * deltaTime * 2;
  if      (diff > 0 && m.getVolume() + diff > 100) m.setVolume(100);
  else if (diff > 0 && m.getVolume() + diff <   0) m.setVolume(  0);
  else m.setVolume(m.getVolume() + diff);
}

void GaemLogic_updateGame(GaemData* gd, float dt_milis, sf::RenderWindow* target) {
  if (gd->restarting && gd->fade < 0) {
    loadLevel(gd, GaemData__currentLvl);
    return;
  }
  if (gd->restarting) gd->fade -= dt_milis;
  else gd->fade += dt_milis;
  sf::Vector2i mousePosition = sf::Mouse::getPosition(*target);
  bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
  if (mousePressed) {
    for (int i = 0; i <= int(ID__Pajarito); ++i) {
      if (gd->pajaritos.active[i]) continue;
      int diffx = mousePosition.x - gd->pajaritos.x[i];
      int diffy = mousePosition.y - gd->pajaritos.y[i];
      if (std::sqrt(diffx * diffx + diffy * diffy) < PAJARITO_RADIO && gd->pajaritos.toqueteable[i]) {
        if (gd->clicked) continue;
        gd->clicked = true;
        std::cout << "Clicqued" << std::endl;
        gd->pajaritos.active[i] = true;

        for (int j = 0; j <= gd->pajaritos.p[i]; ++j) {
          int idNewRaio = GaemData__GetNewIDRaio();

          gd->raios.x[idNewRaio] = gd->pajaritos.x[i];
          gd->raios.y[idNewRaio] = gd->pajaritos.y[i];
          gd->raios.pajaritoID[idNewRaio] = i;
          gd->raios.timerms[idNewRaio] = 0;
          gd->raios.done[idNewRaio] = false;
          gd->raios.nRaio[idNewRaio] = j;
        }
      }
    }
  }

  int nDone = 0;
  int id = ID__Raio;
  for (int i = 0; i <= int(ID__Raio ); ++i) {
    UpdateRaio(gd, i, dt_milis);
    nDone += gd->raios.done[i];
  }
  if (gd->restarting) {
    ID__Raio = id;
    return;
  }
  if (gd->clicked && nDone == ID__Raio + 1) {
    gd->restarting = true;
    gd->fade = 1;
  }
  int totalDone = 0;
  for (int i = 0; i <= int(ID__Pajarito); ++i) {
    totalDone += gd->pajaritos.active[i];
  }
  float pct = float(totalDone) / float(ID__Pajarito);
  int mus;
  for (mus = 0; mus < pct*MAX_MUSIC; ++mus) {
    tendVolumenTo(GaemData__music.music[mus], 100, dt_milis);
    // GaemData__music.music[i].setVolume(100);
  }
  for (;mus < MAX_MUSIC; ++mus) {
    tendVolumenTo(GaemData__music.music[mus], (!mus ? 50 : 0), dt_milis);
  }
  if (totalDone == ID__Pajarito + 1) {
    std::cout << "rand" << std::endl;
    gd->restarting = true;
    gd->fade = 1;
    ++GaemData__currentLvl;
  }
  if (gd->restarting) {
    ID__Raio = id;
  }
}
