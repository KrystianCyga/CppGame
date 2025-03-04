#include "creature.h"

#ifndef KNIGHT_H
#define KNIGHT_H

class Knight : public Creature {
public:   
    Knight(Teams team_ = Teams::NONE, std::pair<int, int> localization_ = {0, 0}) :
        Creature(team_, localization_, 150, true, 1,0.9,15.0) {} // More health, slower

    ~Knight() = default;
     std::string getType() const override { return "Knight"; }
};

#endif
