#include <curses.h>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <thread>
#include "game.hpp"

Configs::Configs() {
    std::ifstream a("config.txt");
    a >> KnightHP >> ZombieHP >> DragonHP >> Knightdmg >> Zombiedmg >> Dragondmg >> rows >> cols >> medhp;
    a.close();
}

Map::Map() : conf(){}

Map::~Map() {
    for (auto it = objects.begin(); it != objects.end(); it++) {
        it->second = std::shared_ptr<GameObject>();
    }
}

void Map::loadMap() {
    std::ifstream b("map.txt");
    rows = conf.rows;
    cols = conf.cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char ch;
            b >> ch;
            Point p(j, i);
            switch (ch) {
                case 'K': {
                    objects[p] = std::make_shared<Knight>(p, conf.KnightHP, conf.Knightdmg);
                    break;
                }
                case 'P': {
                    objects[p] = std::make_shared<Princess>(p);
                    break;
                }
                case '#': {
                    objects[p] = std::make_shared<Wall>(p);
                    break;
                }
                case 'Z': {
                    objects[p] = std::make_shared<Zombie>(p, conf.ZombieHP, conf.Zombiedmg);
                    break;
                }
                case 'D': {
                    objects[p] = std::make_shared<Dragon>(p, conf.DragonHP, conf.Dragondmg);
                    break;
                }
                case 'M': {
                    objects[p] = std::make_shared<MedKit>(p, conf.medhp);
                    break;
                }
                default: {
                    break;
                }
            }

        }
    }
    b.close();
}

void Map::drawMap() {
    clear();
    int row, col;
    getmaxyx(stdscr, row, col);

    std::shared_ptr<Knight> k;
    for (auto& ob : objects){
        if (std::dynamic_pointer_cast<Knight>(ob.second)) {
            k = std::dynamic_pointer_cast<Knight>(ob.second);
            break;
        }
    }


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int dist = abs(k->getPos().x - j) + abs(k->getPos().y - i);
            if (dist > 8) {
                mvaddch(i, j, '@');
            }
            else {
                auto it = objects.find(Point(j, i));
                if (it != objects.end()){
                    mvaddch(i, j, it->second->getSym());
                }
            }
        }
    }

    int player_hp = 0;
    for (const auto& ob : objects) {
        if (std::dynamic_pointer_cast<Knight>(ob.second)) {
            player_hp = ob.second->getHp();
            break;
        }
    }

    mvprintw(rows + 2, 5, "Player hp: %i", player_hp);

    refresh();
}

Game::Game() : map() {
    map.loadMap();
}

void Game::initGame() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    nodelay(stdscr, true);

    int row, col;
    getmaxyx(stdscr, row, col);

    const char* welcome = "Welcome";
    mvprintw(row / 2, (col - strlen(welcome)) / 2, "%s", welcome);

    const char* start = "Press 'S' to start";
    mvprintw(row - 2, 0, "%s", start);

    const char* exit = "Press 'Q' to quit";
    mvprintw(row - 1, 0, "%s", exit);

    while (true) {
        int command = getch();
        if (command == 'S' || command == 's') {
            startGame();
            endwin();
            break;
        }
        else if (command == 'Q' || command == 'q') {
            endwin();
            break;
        }
    }
}


void Game::startGame() {
    std::chrono::milliseconds interval(150);
    nodelay(stdscr, true);

    while (true) {
        for (auto iter = map.objects.begin(); iter != map.objects.end();){
            if (iter->second->getHp() <= 0) {
                iter = map.objects.erase(iter);
            }
            else {
                iter++;
            }
        }
        for (const auto& ob : map.objects) {
            Point pos = ob.second->move(map.objects);
            if (std::dynamic_pointer_cast<Knight>(ob.second)) {
                if (ob.second->reach_end) {
                    endGame(true);
                    return;
                }
                else if (ob.second->getHp() <= 0) {
                    endGame(false);
                    return;
                }
            }

        }
        map.drawMap();
        std::this_thread::sleep_for(interval);
    }
}

void Game::endGame(bool success) {
    int row, col;
    getmaxyx(stdscr, row, col);
    const char * result;
    if (success) {
        result = "You won!";
    }
    else {
        result = "You lost :(";
    }
    clear();
    mvprintw(row / 2, (col - strlen(result)) / 2, "%s", result);
    const char* exit_msg = "Press 'Q' to quit";
    mvprintw(row - 1, 0, "%s", exit_msg);
    while (true) {
        int command = getch();
        if (command == 'Q' || command == 'q') {
            break;
        }
    }


}