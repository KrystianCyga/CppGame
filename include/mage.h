#pragma once

#include "creature.h"

const int MAX_MANA = 100;
const int MANA_REGEN_RATE = 5;

class Mage : public Creature {
private:
    int _mana;

public:
    Mage(Teams team_ = Teams::NONE, std::pair<int, int> localization_ = {0, 0}) :
        Creature(team_, localization_, 75, true, 5,1.0,15.0), _mana(MAX_MANA) {}

    ~Mage() = default;

    void attack(Creature& target) override;
    void regenerateMana();
    int getMana() const { return _mana; }
    void setMana(int mana) { _mana = mana; }
     std::string getType() const override { return "Mage"; }
};
