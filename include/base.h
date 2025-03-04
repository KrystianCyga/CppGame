#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <queue>
#include <map>
#include "map"

#ifndef BAZE_H
#define BAZE_H

enum class Teams {
    NONE = 0,
    GREEN = 1,
    RED = 2,
    BLACK = 3,
    BLUE = 4,
    YELLOW = 5,
    WHITE = 6,
    PURPLE = 7
};


class Base {
private:
    std::pair<int, int> _localization;
    Teams _team;
    unsigned int _hp;
    bool _isAlive;

public:
    Base(Teams team_ ,std::pair<int, int> localization_, int hp_ = 1000);
    ~Base() = default;

    Teams getTeam() const { return _team; }
    void setTeam(Teams team) { _team = team; }

    std::pair<int, int> getLocalization() const { return _localization; }
    void setLocalization(std::pair<int, int> localization) { _localization = localization; }

    unsigned int getHp() const { return _hp; }
    bool isAlive() const { return _isAlive; }
    void setHp(unsigned int hp) { _hp = hp; }


    void takeDamage(unsigned int damage);
};

#endif