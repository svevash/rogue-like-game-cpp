#include <map>
#include <memory>
#include <cassert>

class GameObject;
class Character;
class Wall;
class Knight;
class Zombie;
class Dragon;
class Princess;
class MedKit;
class Projectile;


struct Point {
  int x;
  int y;
  Point(){}
  virtual ~Point() = default;
  Point(int xx, int yy) : x(xx), y(yy) {}
  bool operator<(const Point& rhs) const {
      if (x < rhs.x) {
          return true;
      } else {
          return x == rhs.x && y < rhs.y;
      }
  }
};


class GameObject {
 protected:
  Point pos;
  char sym;
  int hp;

 public:
  GameObject(Point p, char s, int hp_);
  virtual ~GameObject() = default;
  Point getPos();
  char getSym();
  void setPos(Point p);
  void setSym(char s);
  int getHp();
  void setHp(int hp_);
  virtual Point move(std::map<Point, std::shared_ptr<GameObject>> &objects) = 0;
  virtual void collide(GameObject*) = 0;
  virtual void collide(Wall*) = 0;
  virtual void collide(Knight*) = 0;
  virtual void collide(Zombie*) = 0;
  virtual void collide(Dragon*) = 0;
  virtual void collide(Princess*) = 0;
  virtual void collide(MedKit*) = 0;
  virtual void collide(Projectile*) = 0;
  bool reach_end = false;
};


class Wall : public GameObject {
 public:
  Wall(Point p);
  virtual ~Wall() = default;
  Point move(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void collide(GameObject*) override;
  void collide(Wall*) override ;
  void collide(Knight*) override ;
  void collide(Zombie*) override ;
  void collide(Dragon*) override ;
  void collide(Princess*) override ;
  void collide(MedKit*) override;
  void collide(Projectile*) override;
};


class Princess : public GameObject {
  bool reach = false;
 public:
  Princess(Point p);
  virtual ~Princess() = default;
  Point move(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void collide(GameObject*) override;
  void collide(Wall*) override ;
  void collide(Knight*) override ;
  void collide(Zombie*) override ;
  void collide(Dragon*) override ;
  void collide(Princess*) override ;
  void collide(MedKit*) override ;
  void collide(Projectile*) override;
};

class MedKit : public GameObject {
 public:
  MedKit(Point p, int hp_);
  virtual ~MedKit() = default;
  Point move(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void collide(GameObject*) override;
  void collide(Wall*) override;
  void collide(Knight*) override;
  void collide(Zombie*) override;
  void collide(Dragon*) override;
  void collide(Princess*) override;
  void collide(MedKit*) override ;
  void collide(Projectile*) override;
};


class Character : public GameObject {
 protected:
  int dmg;
  int cur_dir;

 public:
  int getDmg();
  Character(Point p, char s, int hp_, int dmg_);
  virtual ~Character() = default;
  virtual void shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) = 0;
};


class Knight : public Character {
 public:
  Knight(Point p, int hp_, int dmg_);
  virtual ~Knight() = default;
  Point move(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void collide(GameObject*) override;
  void collide(Wall*) override;
  void collide(Knight*) override;
  void collide(Zombie*) override;
  void collide(Dragon*) override;
  void collide(Princess*) override;
  void collide(MedKit*) override;
  void collide(Projectile*) override;
};


class Zombie : public Character {
 public:
  Zombie(Point p, int hp_, int dmg_);
  virtual ~Zombie() = default;
  Point move(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void collide(GameObject*) override;
  void collide(Wall*) override ;
  void collide(Knight*) override ;
  void collide(Zombie*) override ;
  void collide(Dragon*) override ;
  void collide(Princess*) override ;
  void collide(MedKit*) override ;
  void collide(Projectile*) override;
};


class Dragon : public Character {
 public:
  Dragon(Point p, int hp_, int dmg_);
  virtual ~Dragon() = default;
  Point move(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void collide(GameObject*) override;
  void collide(Wall*) override;
  void collide(Knight*) override;
  void collide(Zombie*) override;
  void collide(Dragon*) override;
  void collide(Princess*) override;
  void collide(MedKit*) override ;
  void collide(Projectile*) override;
};

class Projectile : public Character {
 public:

  Projectile(Point p, int dmg_, int dir);
  virtual ~Projectile() = default;
  Point move(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) override;
  void collide(GameObject*) override;
  void collide(Wall*) override;
  void collide(Knight*) override;
  void collide(Zombie*) override;
  void collide(Dragon*) override;
  void collide(Princess*) override;
  void collide(MedKit*) override;
  void collide(Projectile*) override;
};

