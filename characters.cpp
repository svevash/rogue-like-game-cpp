#include <curses.h>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "characters.hpp"

Point GameObject::getPos() {
    return pos;
}

void GameObject::setSym(char s) {
    sym = s;
}

void GameObject::setPos(Point p) {
    pos = p;
}

char GameObject::getSym() {
    return sym;
}

int GameObject::getHp() {
    return hp;
}

void GameObject::setHp(int hp_) {
    hp = hp_;
}

GameObject::GameObject(Point p, char s, int hp_) : pos(p), sym(s), hp(hp_) {}

Character::Character(Point p, char s, int hp_, int dmg_) : GameObject(p, s, hp_), dmg(dmg_) {}

int Character::getDmg() {
    return dmg;
}

Wall::Wall(Point p) : GameObject(p, '#', 1) {}

Princess::Princess(Point p) : GameObject(p, 'P', 1) {}

MedKit::MedKit(Point p, int hp_) : GameObject(p, 'M', hp_) {}

Knight::Knight(Point p, int hp_, int dmg_) : Character(p, 'K', hp_, dmg_) {}

Zombie::Zombie(Point p, int hp_, int dmg_) : Character(p, 'Z', hp_, dmg_) {}

Dragon::Dragon(Point p, int hp_, int dmg_) : Character(p, 'D', hp_, dmg_) {}

Projectile::Projectile(Point p, int dmg_, int dir) : Character(p, '*', 5, dmg_) {
    cur_dir = dir;
    // up
    if (dir == 0) {
        setSym('^');
    }
    // right
    else if (dir == 1) {
        setSym('>');
    }
    // down
    else if (dir == 2) {
        setSym('v');
    }
    //left
    else {
        setSym('<');
    }
};

Point Wall::move(std::map<Point, std::shared_ptr<GameObject>> &objects) {
    return getPos();
}

Point Princess::move(std::map<Point, std::shared_ptr<GameObject>> &objects) {
    return getPos();
}

Point MedKit::move(std::map<Point, std::shared_ptr<GameObject> > &objects) {
    return getPos();
}

Point Knight::move(std::map<Point, std::shared_ptr<GameObject>> &objects) {
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    int command = getch();
    if (command == ' ') {
        shoot(objects);
        while (getch() > 0) {}
        return getPos();
    }
    int up = -1 * (command == KEY_UP);
    int down = (command == KEY_DOWN);
    int left = -1 * (command == KEY_LEFT);
    int right = (command == KEY_RIGHT);

    while (getch() > 0) {}

    Point old_pos = pos;
    Point new_pos(old_pos.x + left + right, old_pos.y + up + down);

    if (up != 0) {
        cur_dir = 0;
    }
    else if (down != 0) {
        cur_dir = 2;
    }
    else if (right != 0) {
        cur_dir = 1;
    }
    else if (left != 0) {
        cur_dir = 3;
    }

    auto it = objects.find(new_pos);
    if (it == objects.end()) {
        auto k = objects[old_pos];
        objects.erase(old_pos);
        objects[new_pos] = k;
        pos = new_pos;
    }
    else {
        collide(it->second.get());
    }

    return getPos();
}

Point Zombie::move(std::map<Point, std::shared_ptr<GameObject>> &objects) {
    Point old_pos = pos;
    Point new_pos;
    switch (random() % 5) {
        case 0: {
            new_pos = Point(old_pos.x - 1, old_pos.y);
            break;
        }
        case 1: {
            new_pos = Point(old_pos.x + 1, old_pos.y);
            break;
        }
        case 2: {
            new_pos = Point(old_pos.x, old_pos.y + 1);
            break;
        }
        case 3: {
            new_pos = Point(old_pos.x, old_pos.y - 1);
            break;
        }
        case 4: {
            new_pos = Point(old_pos.x, old_pos.y + 1);
            break;
        }
        default: {
            new_pos = old_pos;
            break;
        }
    }
    auto it = objects.find(new_pos);
    if (it == objects.end()) {
        auto k = objects[old_pos];
        objects.erase(old_pos);
        objects[new_pos] = k;
        pos = new_pos;
    }
    else {
        collide(it->second.get());
    }
    return getPos();
}

Point Dragon::move(std::map<Point, std::shared_ptr<GameObject>> &objects) {
    Point old_pos = pos;
    Point new_pos;
    switch (random() % 5) {
        case 0: {
            new_pos = Point(old_pos.x - 1, old_pos.y);
            cur_dir = 4;
            break;
        }
        case 1: {
            new_pos = Point(old_pos.x + 1, old_pos.y);
            cur_dir = 1;
            break;
        }
        case 2: {
            new_pos = Point(old_pos.x, old_pos.y + 1);
            cur_dir = 3;
            break;
        }
        case 3: {
            new_pos = Point(old_pos.x, old_pos.y - 1);
            cur_dir = 0;
            break;
        }
        case 4: {
            shoot(objects);
            return getPos();
        }
        default: {
            new_pos = old_pos;
            break;
        }
    }
    auto it = objects.find(new_pos);
    if (it == objects.end()) {
        auto k = objects[old_pos];
        objects.erase(old_pos);
        objects[new_pos] = k;
        pos = new_pos;
    }
    else {
        collide(it->second.get());
    }
    return getPos();
}


Point Projectile::move(std::map<Point, std::shared_ptr<GameObject>> &objects) {
    Point old_pos = pos;
    Point new_pos;
        // up
        if (cur_dir == 0) {
            new_pos = Point(pos.x, pos.y - 1);
        }
            // right
        else if (cur_dir == 1) {
            new_pos = Point(pos.x + 1, pos.y);
        }
            // down
        else if (cur_dir == 2) {
            new_pos = Point(pos.x, pos.y + 1);
        }
            //left
        else {
            new_pos = Point(pos.x - 1, pos.y);
        }

    auto it = objects.find(new_pos);
    if (it != objects.end()) {
        collide(it->second.get());
    }
    else {
        auto k = objects[old_pos];
        objects.erase(old_pos);
        objects[new_pos] = k;
        pos = new_pos;
    }

    return getPos();
}



void Princess::collide(GameObject* g) {
    g->collide(this);
}
void Princess::collide(Wall*) {}
void Princess::collide(Knight* k) {
    k->reach_end = true;
}
void Princess::collide(Zombie*) {}
void Princess::collide(Dragon*) {}
void Princess::collide(Princess*) {}
void Princess::collide(MedKit *) {}
void Princess::collide(Projectile *p) {
    p->setHp(0);
}


void Knight::collide(Princess*) {
    reach_end = true;
}
void Knight::collide(GameObject* g) {
    g->collide(this);
}
void Knight::collide(Wall*) {}
void Knight::collide(Knight*) {}
void Knight::collide(Zombie* z) {
    z->setHp(z->getHp() - dmg);
    hp -= z->getDmg();
}
void Knight::collide(Dragon* d) {
    d->setHp(d->getHp() - dmg);
    hp -= d->getDmg();
}
void Knight::collide(MedKit* m) {
    hp += m->getHp();
    m->setHp(0);
}
void Knight::collide(Projectile *p) {
    hp -= p->getHp();
    p->setHp(0);
}



void Zombie::collide(Princess*) {}
void Zombie::collide(GameObject* g) {
    g->collide(this);
}
void Zombie::collide(Wall*) {}
void Zombie::collide(Knight* k) {
    k->setHp(k->getHp() - dmg);
}
void Zombie::collide(Zombie*) {}
void Zombie::collide(Dragon*) {}
void Zombie::collide(MedKit *) {}
void Zombie::collide(Projectile *p) {
    hp -= p->getHp();
    p->setHp(0);
}



void Wall::collide(Princess*) {}
void Wall::collide(GameObject* g) {
    g->collide(this);
}
void Wall::collide(Wall*) {}
void Wall::collide(Knight*) {}
void Wall::collide(Zombie*) {}
void Wall::collide(Dragon*) {}
void Wall::collide(MedKit *) {}
void Wall::collide(Projectile *p) {
    p->setHp(0);
}



void Dragon::collide(Princess*) {}
void Dragon::collide(GameObject* g) {
    g->collide(this);
}
void Dragon::collide(Wall*) {}
void Dragon::collide(Knight* k) {
    k->setHp(k->getHp() - dmg);
}
void Dragon::collide(Zombie*) {}
void Dragon::collide(Dragon*) {}
void Dragon::collide(MedKit *) {}
void Dragon::collide(Projectile *p) {
    hp -= p->getHp();
    p->setHp(0);
}



void MedKit::collide(Princess*) {}
void MedKit::collide(GameObject* g) {
    g->collide(this);
}
void MedKit::collide(Wall*) {}
void MedKit::collide(Knight* k) {
    k->setHp(k->getHp() + hp);
    hp = 0;
}
void MedKit::collide(Zombie*) {}
void MedKit::collide(Dragon*) {}
void MedKit::collide(MedKit *) {}
void MedKit::collide(Projectile *p) {
    hp = 0;
    p->setHp(0);
}



void Projectile::collide(Princess*) {
    hp = 0;
}
void Projectile::collide(GameObject* g) {
    g->collide(this);
}
void Projectile::collide(Wall*) {
    hp = 0;
}
void Projectile::collide(Knight* k) {
    k->setHp(k->getHp() - dmg);
    hp = 0;
}
void Projectile::collide(Zombie* z) {
    z->setHp(z->getHp() - dmg);
    hp = 0;
}
void Projectile::collide(Dragon* d) {
    d->setHp(d->getHp() - dmg);
    hp = 0;
}
void Projectile::collide(MedKit *m) {
    hp = 0;
    m->setHp(0);
}
void Projectile::collide(Projectile *p) {
    hp = 0;
    p->setHp(0);
}

void Zombie::shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) {}
void Projectile::shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) {}

void Knight::shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) {
    Point p;
    // up
    if (cur_dir == 0) {
        p = Point(pos.x, pos.y - 1);
    }
        // right
    else if (cur_dir == 1) {
        p = Point(pos.x + 1, pos.y);
    }
        // down
    else if (cur_dir == 2) {
        p = Point(pos.x, pos.y + 1);
    }
        //left
    else {
        p = Point(pos.x - 1, pos.y);
    }

    auto it = objects.find(p);
    if (it == objects.end()) {
        objects[p] = std::make_shared<Projectile>(p, 5, cur_dir);
    }
}

void Dragon::shoot(std::map<Point, std::shared_ptr<GameObject>> &objects) {
    Point p;
    // up
    if (cur_dir == 0) {
        p = Point(pos.x, pos.y - 1);
    }
        // right
    else if (cur_dir == 1) {
        p = Point(pos.x + 1, pos.y);
    }
        // down
    else if (cur_dir == 2) {
        p = Point(pos.x, pos.y + 1);
    }
        //left
    else {
        p = Point(pos.x - 1, pos.y);
    }

    auto it = objects.find(p);
    if (it == objects.end()) {
        objects[p] = std::make_shared<Projectile>(p, 5, cur_dir);
    }
}