#include "characters.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>

struct Configs {
  int ZombieHP = 0;
  int DragonHP = 0;
  int KnightHP = 0;
  int Zombiedmg = 0;
  int Dragondmg = 0;
  int Knightdmg = 0;
  int rows = 0;
  int cols = 0;
  int medhp = 0;
  Configs();
};

class Map {
 public:
  friend class Game;

  Map();
  ~Map();
  void loadMap();
  void drawMap();

 private:
  std::map<Point, std::shared_ptr<GameObject>> objects;
  Configs conf;
  int rows = 0, cols = 0;
};

class Game {
 public:
  Game();
  void initGame();
  void startGame();
  void endGame(bool);

 private:
  Map map;
  bool win = false;
};
