#include "base.h"
#include <iostream>

Base::Base(Teams team_, std::pair<int, int> localization_, int hp_) :
    _isAlive(true), _team(team_), _hp(hp_), _localization(localization_) {}

void Base::takeDamage(unsigned int damage) {
    _hp -= damage;
    std::cout << "Base of team " << static_cast<int>(_team) << " took " << damage << " damage.  New HP: " << _hp << std::endl;
    if (_hp <= 0) {
        _hp = 0;
        std::cout << "Base of team " << static_cast<int>(_team) << " has been destroyed!" << std::endl;
        _isAlive=false;
    }
}