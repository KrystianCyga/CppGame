#pragma once

#include "creature.h"


class Elf : public Creature {
public:

    Elf(Teams team_ = Teams::NONE, std::pair<int, int> localization_ = {0, 0}) :
        Creature(team_, localization_, 80, true, 4,1.2,12.0) {} // Slightly faster, ranged

    ~Elf() = default;

    void attack(Creature& target) override;
     void move(int x, int y, const Map& map) override;
     std::string getType() const override { return "Elf"; }

};

